#!/usr/bin/env ruby
# run.rb
# Copyright (C) 2023
# Squidpie

require 'redis'
require 'json'

$stdout.sync = true

$rx = "#strauss-chat-msg-rx";
$tx = "#strauss-chat-msg-tx";
$msg_hash={"sender"=>"test_user", "message_text"=>"@squidbot_roboface 0xdeadbeef"};
$test_msg= $msg_hash.to_json()

redis = Redis.new(url: "redis://localhost:6379")

def client(redis)
    begin
        puts "::TEST OUTPUT:: Starting Squidbot-Voice Integration Test";
        redis.subscribe($tx) do |on|
            on.subscribe do |channel, subscriptions|
                puts "::TEST OUTPUT:: Subscribed to #{channel} (#{subscriptions} subscriptions)";
            end

            on.message do |channel, msg|
                puts "::TEST OUTPUT:: Received #{channel} - [#{msg}]";
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
        puts "::TEST OUTPUT:: Sending Test Message";
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
