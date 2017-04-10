#!/bin/sh 

module=$1
version=$2
build_root=/tmp
rpm_root=/usr/src/redhat

./autogen.sh --prefix=/usr --sysconfdir=/etc
make dist
mv $module-$version.tar.gz $rpm_root/SOURCES
rpm -ba $module.spec

