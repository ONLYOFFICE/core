#!/bin/sh
#
# This script needs to be called from testrun.sh script
#

##########################################################################
##########################################################################
##########################################################################
echo "--- testKeys started for xmlsec-$crypto library ($timestamp) ---"
echo "--- LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
echo "--- log file is $logfile"
echo "--- testKeys started for xmlsec-$crypto library ($timestamp) ---" >> $logfile
echo "--- LD_LIBRARY_PATH=$LD_LIBRARY_PATH" >> $logfile

# cleanup crypto config folder
mkdir -p $crypto_config
rm -rf $crypto_config/*

# remove old keys file and copy NSS DB files if needed
rm -rf $keysfile
if [ "z$crypto" = "znss" ] ; then
    cp -f $nssdbfolder/*.db $crypto_config
fi

##########################################################################
##########################################################################
##########################################################################
echo "--------- Positive Testing ----------"
execKeysTest $res_success \
    "hmac" \
    "test-hmac-sha1" \
    "hmac-192"

execKeysTest $res_success \
    "rsa " \
    "test-rsa      " \
    "rsa-1024"

execKeysTest $res_success \
    "dsa " \
    "test-dsa      " \
    "dsa-1024"

execKeysTest $res_success \
    "des " \
    "test-des      " \
    "des-192 "

execKeysTest $res_success \
    "aes " \
    "test-aes128   " \
    "aes-128 "

execKeysTest $res_success \
    "aes " \
    "test-aes192   " \
    "aes-192 "

execKeysTest $res_success \
    "aes " \
    "test-aes256   " \
    "aes-256 "

##########################################################################
##########################################################################
##########################################################################
echo "--- testKeys finished ---" >> $logfile
echo "--- testKeys finished ---"
echo "--- detailed log is written to  $logfile ---"
