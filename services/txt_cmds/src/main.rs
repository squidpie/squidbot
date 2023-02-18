/*
main.rs
Copyright (C) 2023
Squidpie
*/

use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use chrono::Utc;

use strausslib::config::{FileReader, SerialRead};
use strausslib::redisclient::{RedisClient, RedisWrapper};
use strausslib::twitch::chat::{
    TwitchMsg, STRAUSS_CHAT_MSG_RX_REDIS_CH, STRAUSS_CHAT_MSG_TX_REDIS_CH,
};

// TODO: Name something TenTacle for Moonfire.

#[derive(Debug, Serialize, Deserialize, Clone)]
struct TxtCmdConfigEntry {
    cooldown: i64,
    response: String,
}

type CmdMap = HashMap<String, TxtCmdConfigEntry>;

#[derive(Debug, Serialize, Deserialize, Clone)]
struct TxtCmdConfig {
    commands: CmdMap,
}

trait TxtCmdConfigLoader {
    fn load(yaml_reader: Box<dyn SerialRead<TxtCmdConfig>>) -> Self;
}

impl TxtCmdConfigLoader for TxtCmdConfig {
    fn load(yaml_reader: Box<dyn SerialRead<Self>>) -> Self {
        yaml_reader.read(String::from("txt_cmds.yml"))
    }
}

#[derive(Debug)]
struct TxtCmd {
    cooldown_expires_at: i64,
    config: TxtCmdConfigEntry,
}

struct TxtCmdControl {
    cmds: HashMap<String, TxtCmd>,
    redis: Box<dyn RedisClient>,
}

impl TxtCmdControl {
    async fn new() -> Self {
        let yaml_reader = Box::new(FileReader {});
        let config = TxtCmdConfig::load(yaml_reader);
        let redis = Box::new(RedisWrapper::new(STRAUSS_CHAT_MSG_RX_REDIS_CH.to_owned()).await);
        let mut cmds: HashMap<String, TxtCmd> = HashMap::new();
        for entry in config.commands {
            let c = TxtCmd {
                cooldown_expires_at: Utc::now().timestamp(),
                config: entry.1,
            };
            println!("::DEBUG OUTPUT::CMD INSERT=({:?}, {c:#?})", entry.0);
            cmds.insert(entry.0, c);
        }
        TxtCmdControl { cmds, redis }
    }

    async fn process_msg(&mut self, msg: String) {
        let words: Vec<&str> = msg.split_whitespace().collect();
        let key_vec: Vec<&str> = words[0].split("!").collect();
        let key = key_vec[1];
        if let Some(cmd) = self.cmds.get_mut(key) {
            if cmd.cooldown_expires_at > Utc::now().timestamp() {
                println!("::DEBUG OUTPUT:: COOLDOWN NOT EXPIRED {:#?}", cmd.cooldown_expires_at)
            } else {
                let cooldown = chrono::Duration::seconds(cmd.config.cooldown);
                cmd.cooldown_expires_at = (Utc::now() + cooldown).timestamp();
                self.redis
                    .publish(
                        STRAUSS_CHAT_MSG_TX_REDIS_CH.to_owned(),
                        cmd.config.response.clone(),
                    )
                    .await
                    .expect("<TxtCmdControl>::ERROR::FAILED TO PUBLISH");
            }
        } else {
            println!("::DEBUG OUTPUT:: NO KEY FOUND {key:?}")
        };
    }

    async fn recv_msg(&mut self) -> Result<String, Box<dyn std::error::Error>> {
        let json = self.redis.pubsub().await;
        let full_msg: TwitchMsg =
            serde_json::from_str(&json)?;//.expect("<TxtCmds>::ERROR::FAILED TO DECODE JSON");
        Ok(full_msg.message_text)
    }
}

#[tokio::main]
async fn main() {
    let mut txt_cmds = TxtCmdControl::new().await;
    txt_cmds
        .redis
        .publish(
            STRAUSS_CHAT_MSG_TX_REDIS_CH.to_owned(),
            "<TxtCmds>::INFO::Entering Chat".to_owned(),
        )
        .await
        .unwrap();
    loop {
        let msg = match txt_cmds.recv_msg().await {
            Ok(msg) => msg,
            Err(_) => continue
        };
        if msg.chars().nth(0).unwrap() != '!' {
            continue;
        }
        txt_cmds.process_msg(msg).await;
    }
}

#[cfg(test)]
mod tests {
    use std::thread::sleep;

    use super::*;
    use strausslib::redisclient::MockRedisWrapper;

    #[test]
    fn create_config() {
        let test_config = TxtCmdConfigEntry {
            cooldown: 1,
            response: String::from("0xdeadbeef"),
        };
        let mut commands: CmdMap = HashMap::new();
        commands.insert(String::from("test_command"), test_config);

        let dut = TxtCmdConfig {
            commands: commands.clone(),
        };
        assert_eq!(
            dut.commands["test_command"].cooldown,
            commands["test_command"].cooldown
        );
        assert_eq!(
            dut.commands["test_command"].response,
            commands["test_command"].response
        );
    }

    #[test]
    fn cooldown_expires() {
        let now = Utc::now();
        let cooldown_expires_at = now.timestamp();

        let config = TxtCmdConfigEntry {
            cooldown: 1,
            response: String::from("0xdeadbeef"),
        };

        let dut = TxtCmd {
            cooldown_expires_at,
            config,
        };
        sleep(std::time::Duration::from_secs(1));
        assert_ne!(dut.cooldown_expires_at, Utc::now().timestamp())
    }

    #[tokio::test]
    async fn process_received_message() {
        let raw_json = r##"{"channel_login":"squidbot_roboface","channel_id":"819468422","message_text":"!test_command plus garbage","is_action":false,"sender":{"id":"187848111","login":"squidpie_meowface","name":"Squidpie_Meowface"},"badge_info":[],"badges":[],"bits":null,"name_color":{"r":250,"g":133,"b":191},"emotes":[],"message_id":"e91a372d-92b6-4678-9160-f67cf96f434d","server_timestamp":"2023-02-17T05:28:42.924Z","source":{"tags":{"returning-chatter":"0","subscriber":"0","tmi-sent-ts":"1676611722924","badges":"","room-id":"819468422","emotes":"","mod":"0","badge-info":"","user-type":"","display-name":"Squidpie_Meowface","user-id":"187848111","turbo":"0","color":"#FA85BF","client-nonce":"94bf09d02aeea68fa7a25de3267f94e1","first-msg":"0","flags":"","id":"e91a372d-92b6-4678-9160-f67cf96f434d"},"prefix":{"Full":{"nick":"squidpie_meowface","user":"squidpie_meowface","host":"squidpie_meowface.tmi.twitch.tv"}},"command":"PRIVMSG","params":["#squidbot_roboface","!test_command plus garbage"]}}"##;

        let test_config = TxtCmdConfigEntry {
            cooldown: 1,
            response: String::from("0xdeadbeef"),
        };
        let mut commands: CmdMap = HashMap::new();
        commands.insert(String::from("test_command"), test_config);

        let cmd_config = TxtCmdConfig {
            commands: commands.clone(),
        };
        let mut cmds: HashMap<String, TxtCmd> = HashMap::new();
        for cmd_entry in cmd_config.commands {
            let c = TxtCmd {
                cooldown_expires_at: Utc::now().timestamp(),
                config: cmd_entry.1,
            };
            cmds.insert(cmd_entry.0, c);
        }

        let mut redis = MockRedisWrapper::default();
        redis.expect_publish().once().return_once(|_, msg| {
            assert_eq!(msg, String::from("0xdeadbeef"));
            Ok(())
        });
        redis.expect_pubsub().return_once(|| raw_json.to_owned());

        let mut dut = TxtCmdControl {
            cmds,
            redis: Box::new(redis),
        };

        let msg = dut.recv_msg().await.unwrap();
        dut.process_msg(msg).await;
    }
}
