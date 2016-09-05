#!/bin/bash

BUILD_DIR=project/cryptopp.build
mkdir -p $BUILD_DIR
cd $BUILD_DIR
qmake ../cryptopp.pro
make && echo "build: OK!"
