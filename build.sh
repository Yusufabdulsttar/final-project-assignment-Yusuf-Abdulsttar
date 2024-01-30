#!/bin/bash
# Script to build image for qemu.

git submodule init
git submodule sync
git submodule update

# local.conf won't exist until this step on first execution
source poky/oe-init-build-env

CONFLINE="MACHINE = \"qemuarm64\""

cat conf/local.conf | grep "${CONFLINE}" > /dev/null
local_conf_info=$?

if [ $local_conf_info -ne 0 ];then
	echo "Append ${CONFLINE} in the local.conf file"
	echo ${CONFLINE} >> conf/local.conf
	
else
	echo "${CONFLINE} already exists in the local.conf file"
fi


bitbake-layers show-layers | grep "meta-my-shell" > /dev/null
layer_info=$?

if [ $layer_info -ne 0 ];then
	echo "Adding meta-my-shell layer"
	bitbake-layers add-layer ../meta-my-shell
else
	echo "meta-my-shell layer already exists"
fi

set -e
bitbake core-image-my-shell
