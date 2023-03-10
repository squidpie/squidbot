#!/bin/bash
# package.sh
# Copyright (C) 2023
# Squidpie
if [[ -z ${GITVERSION_SEMVER} ]]; then
    echo "Env Var GITVERSION_SEMVER not set"
    exit -1
fi
RelName="squidbot-${GITVERSION_SEMVER}"
RelRoot="packaging"
RelPath="${RelRoot}/${RelName}"

# Setup output dir
if [ -d "${RelPath}" ]; then rm -Rf ${RelPath}; fi
mkdir -p ${RelPath}

# Copy required compose files
cp docker-compose.yml ${RelPath}/
cp docker-compose.prod.yml ${RelPath}/

# Create Deployment Environment
./scripts/gen-gha-env.sh
cp .env ${RelPath}/

# Copy deploy script
cp scripts/deploy.sh ${RelPath}/

# Copy txt_cmds config
cp services/txt_cmds/txt_cmds.yml ${RelPath}/

# Create Package & Cleanup
tar czf ${RelPath}.tar.gz -C packaging ${RelName}
rm -r ${RelPath}/
