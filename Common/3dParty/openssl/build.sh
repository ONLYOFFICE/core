#!/bin/bash

export PATH=`pwd`/depot_tools:"$PATH"

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd "$SCRIPTPATH"/openssl

perl ./Configure linux-64
./config
make
