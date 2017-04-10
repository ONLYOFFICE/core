#!/bin/sh 

# input
version=$1

# config
svn_module=xmlsec
svn_uri=svn+ssh://aleksey@svn.gnome.org/svn/$svn_module/trunk
svn_branch_uri=svn+ssh://aleksey@svn.gnome.org/svn/$svn_module/branches/$version

rpm_name=xmlsec1
rpm_root=/usr/src/redhat
remote_root=aleksey@ftp.aleksey.com:/var/ftp/pub/xmlsec/releases
build_target=i386

echo "Uploading to aleksey.com"
scp $rpm_root/SOURCES/$rpm_name-$version.tar.gz \
    $rpm_root/SRPMS/$rpm_name-$version-*.src.rpm \
    $rpm_root/RPMS/$build_target/$rpm_name-$version-*.$build_target.rpm \
    $rpm_root/RPMS/$build_target/$rpm_name-devel-$version-*.$build_target.rpm \
    $rpm_root/RPMS/$build_target/$rpm_name-openssl-$version-*.$build_target.rpm \
    $rpm_root/RPMS/$build_target/$rpm_name-openssl-devel-$version-*.$build_target.rpm \
    $rpm_root/RPMS/$build_target/$rpm_name-nss-$version-*.$build_target.rpm \
    $rpm_root/RPMS/$build_target/$rpm_name-nss-devel-$version-*.$build_target.rpm \
    $remote_root


echo "Creating SVN branch $version"
svn copy $svn_uri $svn_branch_uri -m"creating release $version branch"

