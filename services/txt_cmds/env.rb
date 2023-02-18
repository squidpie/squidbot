#!/usr/bin/env ruby
# env.rb
# Copyright (C) 2023
# Squidpie

raw_version = File.open("services/txt_cmds/Cargo.toml") do |file|
  file.find { |line| line =~ /^version = .*/ }
end
parsed_version = raw_version.split('=')[1].tr('" ', '')
puts "SQUIDBOT_TXTCMDS_PKG_VERSION=#{parsed_version}"