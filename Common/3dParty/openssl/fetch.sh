#!/bin/bash

SCRIPT=$(readlink -f "$0" || grealpath "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

cd "$SCRIPTPATH"

if [ ! -d openssl ]; then
  git clone https://github.com/openssl/openssl.git
fi
