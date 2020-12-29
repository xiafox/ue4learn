#!/bin/sh

TARGET_DIR=$(dirname $(readlink -f "${BASH_SOURCE[0]}"))

CLUSTER_CONTROLLER="python ${TARGET_DIR}/kbe/tools/server/pycluster/cluster_controller.py"

#${CLUSTER_CONTROLLER} startprocess logger     cid gus ip
#${CLUSTER_CONTROLLER} startprocess interfaces cid gus ip
#${CLUSTER_CONTROLLER} startprocess dbmgr      cid gus ip
#${CLUSTER_CONTROLLER} startprocess baseappmgr cid gus ip
#${CLUSTER_CONTROLLER} startprocess cellappmgr cid gus ip
#${CLUSTER_CONTROLLER} startprocess loginapp   cid gus ip
#${CLUSTER_CONTROLLER} startprocess baseapp    cid gus ip
#${CLUSTER_CONTROLLER} startprocess cellapp    cid gus ip

#${CLUSTER_CONTROLLER} startprocess logger     10000000000001 10001 127.0.0.1
#${CLUSTER_CONTROLLER} startprocess interfaces 13000000000001 13001 127.0.0.1
#${CLUSTER_CONTROLLER} startprocess dbmgr      01000000000001 01001 127.0.0.1
#${CLUSTER_CONTROLLER} startprocess baseappmgr 03000000000001 03001 127.0.0.1
#${CLUSTER_CONTROLLER} startprocess cellappmgr 04000000000001 04001 127.0.0.1
#${CLUSTER_CONTROLLER} startprocess baseapp    06000000000001 06001 127.0.0.1
#${CLUSTER_CONTROLLER} startprocess cellapp    05000000000001 05001 127.0.0.1
#${CLUSTER_CONTROLLER} startprocess loginapp   02000000000001 02001 127.0.0.1

echo "modify this file to set you custom start solution and remove this message on first run."
