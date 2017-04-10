SET SCRIPTPATH=%~dp0
CD /D %~dp0

call hg clone https://hg.mozilla.org/projects/nspr
call hg clone https://hg.mozilla.org/projects/nss

cd nss
export USE_64=1
make nss_build_all

git clone https://github.com/openssl/openssl.git openssl
cd openssl
perl ./Configure linux-64
./config
make

#call git clone -b master https://github.com/lsh123/xmlsec.git
#download from http://www.aleksey.com/xmlsec/download/xmlsec1-1.2.23.tar.gz
# get from our git repository