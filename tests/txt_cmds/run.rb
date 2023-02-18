#!/usr/bin/env ruby
# run.rb
# Copyright (C) 2023
# Squidpie

require 'redis'
require 'json'

$stdout.sync = true

$rx = "#strauss-chat-msg-rx";
$tx = "#strauss-chat-msg-tx";

$json_hash = {
"channel_login"=>"test_channel",
    "channel_id"=>"0",
    "message_text"=>"!test plus garbage",
    "is_action"=>false,
    "sender"=>{"id"=>"0","login"=>"test_user","name"=>"Test_User"},
    "badge_info"=>[],"badges"=>[],"bits"=>nil,
    "name_color"=>{"r"=>250,"g"=>133,"b"=>191},
    "emotes"=>[],
    "message_id"=>"e91a372d-92b6-4678-9160-f67cf96f434d",
    "server_timestamp"=>"2023-02-17T05:28:42.924Z",
    "source"=>{"tags"=>{"returning-chatter"=>"0","subscriber"=>"0","tmi-sent-ts"=>"1676611722924","badges"=>"","room-id"=>"0","emotes"=>"","mod"=>"0","badge-info"=>"","user-type"=>"","display-name"=>"Test_User","user-id"=>"0","turbo"=>"0","color"=>"#FA85BF","client-nonce"=>"94bf09d02aeea68fa7a25de3267f94e1","first-msg"=>"0","flags"=>"","id"=>"e91a372d-92b6-4678-9160-f67cf96f434d"},"prefix":{"Full":{"nick"=>"test_user","user"=>"test_user","host"=>"test_user.tmi.twitch.tv"}},"command"=>"PRIVMSG","params"=>["#test_channel","!test plus garbage"]}};

$test_msg = $json_hash.to_json()

redis = Redis.new(url: "redis://localhost:6379")

def client(redis)
    begin
        puts "::TEST OUTPUT:: Starting Squidbot-TxtCmds Integration Test";
        redis.subscribe($tx) do |on|
            on.subscribe do |channel, subscriptions|
                puts "::TEST OUTPUT:: Subscribed to #{channel} (#{subscriptions} subscriptions)";
            end

            on.message do |channel, msg|
                puts "::TEST OUTPUT:: Received #{channel} - [#{msg}]";
                if msg != "0xdeadbeef"
                    puts "::TEST OUTPUT:: TEST FAILED : MESSAGE MISMATCH";
                    exit(-1)
                end
                exit(0)
            end
        end
    rescue=>error
        puts "::TEST OUTPUT:: REDIS SUBSCRIBE FAILED - #{error}";
        exit(-1)
    end
end

def publisher(redis)
    begin
        puts "::TEST OUTPUT:: Sending Test Command";
        redis.publish($rx, $test_msg)
    rescue=>error
        puts "::TEST OUTPUT:: REDIS PUBLISH FAILED - #{error}";
        exit(-1)
    end
end

client = Thread.new { client(redis) }
sleep(1)
publisher = Thread.new { publisher(redis) }
publisher.join()
client.join()
