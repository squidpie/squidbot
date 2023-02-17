#!/bin/bash
# gen-dev-env.sh
# Copyright (C) 2023
# Squidpie
echo \
"SQUIDBOT_ROOT_DIR=$(pwd)
SQUIDBOT_BUILD_DIR=target/debug
GITVERSION_SEMVER=$(docker run --rm -v "$(pwd):/repo" gittools/gitversion:5.6.6 /repo | \
  python3 -c "import sys, json; print(json.load(sys.stdin)['SemVer'])")
SQUIDBOT_VOICE_PKG_VERSION=`cat services/voice/version.info`
`services/txt_cmds/env.rb`" > .env

set -a && source .env && set +a