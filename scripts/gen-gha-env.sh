#!/bin/bash
# gen-gha-env.sh
# Copyright (C) 2023
# Squidpie
echo "SQUIDBOT_VOICE_PKG_VERSION=`cat services/voice/version.info`
`services/txt_cmds/env.rb`" > .env