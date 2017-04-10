#!/bin/sh
#
# This script needs to be called from testrun.sh script
#

##########################################################################
##########################################################################
##########################################################################
echo "--- testEnc started for xmlsec-$crypto library ($timestamp)"
echo "--- LD_LIBRARY_PATH=$LD_LIBRARY_PATH" 
echo "--- log file is $logfile"
echo "--- testEnc started for xmlsec-$crypto library ($timestamp)" >> $logfile
echo "--- LD_LIBRARY_PATH=$LD_LIBRARY_PATH" >> $logfile

##########################################################################
##########################################################################
##########################################################################
echo "--------- Positive Testing ----------"

##########################################################################
#
# aleksey-xmlenc-01
#
##########################################################################

execEncTest $res_success \
    "" \
    "aleksey-xmlenc-01/enc-des3cbc-keyname" \
    "tripledes-cbc" \
    "--keys-file $topfolder/keys/keys.xml" \
    "--keys-file $keysfile --binary-data $topfolder/aleksey-xmlenc-01/enc-des3cbc-keyname.data" \
    "--keys-file $keysfile"

execEncTest $res_success \
    "" \
    "aleksey-xmlenc-01/enc-des3cbc-keyname2" \
    "tripledes-cbc" \
    "--keys-file $topfolder/keys/keys.xml" \
    "--keys-file $keysfile --binary-data $topfolder/aleksey-xmlenc-01/enc-des3cbc-keyname2.data" \
    "--keys-file $keysfile"

execEncTest $res_success \
    "" \
    "aleksey-xmlenc-01/enc-aes128cbc-keyname" \
    "aes128-cbc" \
    "--keys-file $topfolder/keys/keys.xml" \
    "--keys-file $keysfile --binary-data $topfolder/aleksey-xmlenc-01/enc-aes128cbc-keyname.data" \
    "--keys-file $keysfile"

execEncTest $res_success \
    "" \
    "aleksey-xmlenc-01/enc-aes192cbc-keyname" \
    "aes192-cbc" \
    "--keys-file $topfolder/keys/keys.xml" \
    "--keys-file $keysfile --binary-data $topfolder/aleksey-xmlenc-01/enc-aes192cbc-keyname.data" \
    "--keys-file $keysfile"

execEncTest $res_success \
    "" \
    "aleksey-xmlenc-01/enc-aes192cbc-keyname-ref" \
    "aes192-cbc" \
    "--keys-file $topfolder/keys/keys.xml"

execEncTest $res_success \
    "" \
    "aleksey-xmlenc-01/enc-aes256cbc-keyname" \
    "aes256-cbc" \
    "--keys-file $topfolder/keys/keys.xml" \
    "--keys-file $keysfile --binary-data $topfolder/aleksey-xmlenc-01/enc-aes256cbc-keyname.data" \
    "--keys-file $keysfile"

execEncTest $res_success \
    "" \
    "aleksey-xmlenc-01/enc-des3cbc-keyname-content" \
    "tripledes-cbc" \
    "--keys-file $topfolder/keys/keys.xml" \
    "--keys-file $keysfile --xml-data $topfolder/aleksey-xmlenc-01/enc-des3cbc-keyname-content.data --node-id Test" \
    "--keys-file $keysfile"

execEncTest $res_success \
    "" \
    "aleksey-xmlenc-01/enc-des3cbc-keyname-element" \
    "tripledes-cbc" \
    "--keys-file $topfolder/keys/keys.xml" \
    "--keys-file $keysfile --xml-data $topfolder/aleksey-xmlenc-01/enc-des3cbc-keyname-element.data --node-id Test" \
    "--keys-file $keysfile"

execEncTest $res_success \
    "" \
    "aleksey-xmlenc-01/enc-des3cbc-keyname-element-root" \
    "tripledes-cbc" \
    "--keys-file $topfolder/keys/keys.xml" \
    "--keys-file $keysfile --xml-data $topfolder/aleksey-xmlenc-01/enc-des3cbc-keyname-element-root.data --node-id Test" \
    "--keys-file $keysfile"

execEncTest $res_success \
    "" \
    "aleksey-xmlenc-01/enc-des3cbc-aes192-keyname" \
    "tripledes-cbc kw-aes192" \
    "--keys-file $topfolder/keys/keys.xml --enabled-key-data key-name,enc-key" \
    "--keys-file $keysfile  --session-key des-192  --binary-data $topfolder/aleksey-xmlenc-01/enc-des3cbc-aes192-keyname.data" \
    "--keys-file $keysfile"

##########################################################################
#
# merlin-xmlenc-five
#
##########################################################################

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-data-aes128-cbc" \
    "aes128-cbc" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml --binary-data $topfolder/merlin-xmlenc-five/encrypt-data-aes128-cbc.data" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml"

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-content-tripledes-cbc" \
    "tripledes-cbc" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml --enabled-key-data key-name --xml-data $topfolder/merlin-xmlenc-five/encrypt-content-tripledes-cbc.data --node-id Payment" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml"

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-content-aes256-cbc-prop" \
    "aes256-cbc" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml --enabled-key-data key-name --xml-data $topfolder/merlin-xmlenc-five/encrypt-content-aes256-cbc-prop.data --node-id Payment" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml"

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-element-aes192-cbc-ref" \
    "aes192-cbc" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml"

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-element-aes128-cbc-rsa-1_5" \
    "aes128-cbc rsa-1_5" \
    "$priv_key_option $topfolder/merlin-xmlenc-five/rsapriv.$priv_key_format --pwd secret" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml --session-key aes-128 $priv_key_option $topfolder/merlin-xmlenc-five/rsapriv.$priv_key_format --xml-data $topfolder/merlin-xmlenc-five/encrypt-element-aes128-cbc-rsa-1_5.data --node-id Purchase --pwd secret"  \
    "$priv_key_option $topfolder/merlin-xmlenc-five/rsapriv.$priv_key_format --pwd secret"

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-data-tripledes-cbc-rsa-oaep-mgf1p" \
    "tripledes-cbc rsa-oaep-mgf1p" \
    "$priv_key_option $topfolder/merlin-xmlenc-five/rsapriv.$priv_key_format --pwd secret" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml --session-key des-192 $priv_key_option $topfolder/merlin-xmlenc-five/rsapriv.$priv_key_format --binary-data $topfolder/merlin-xmlenc-five/encrypt-data-tripledes-cbc-rsa-oaep-mgf1p.data --pwd secret"  \
    "$priv_key_option $topfolder/merlin-xmlenc-five/rsapriv.$priv_key_format --pwd secret"

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-data-aes256-cbc-kw-tripledes" \
    "aes256-cbc kw-tripledes" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml --session-key aes-256 --binary-data $topfolder/merlin-xmlenc-five/encrypt-data-aes256-cbc-kw-tripledes.data" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml"

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-content-aes128-cbc-kw-aes192" \
    "aes128-cbc kw-aes192" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml --session-key aes-128 --node-name urn:example:po:PaymentInfo --xml-data $topfolder/merlin-xmlenc-five/encrypt-content-aes128-cbc-kw-aes192.data" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml"

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-data-aes192-cbc-kw-aes256" \
    "aes192-cbc kw-aes256" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml --session-key aes-192 --binary-data $topfolder/merlin-xmlenc-five/encrypt-data-aes192-cbc-kw-aes256.data" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml"

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-element-tripledes-cbc-kw-aes128" \
    "tripledes-cbc kw-aes128" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml  --session-key des-192 --node-name urn:example:po:PaymentInfo --xml-data $topfolder/merlin-xmlenc-five/encrypt-element-tripledes-cbc-kw-aes128.data" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml"

execEncTest $res_success \
    "" \
    "merlin-xmlenc-five/encrypt-element-aes256-cbc-retrieved-kw-aes256" \
    "aes256-cbc kw-aes256" \
    "--keys-file $topfolder/merlin-xmlenc-five/keys.xml" 


#merlin-xmlenc-five/encrypt-data-tripledes-cbc-rsa-oaep-mgf1p-sha256.xml

#merlin-xmlenc-five/encrypt-element-aes256-cbc-carried-kw-aes256.xml
#merlin-xmlenc-five/decryption-transform-except.xml
#merlin-xmlenc-five/decryption-transform.xml

#merlin-xmlenc-five/encrypt-element-aes256-cbc-kw-aes256-dh-ripemd160.xml
#merlin-xmlenc-five/encrypt-content-aes192-cbc-dh-sha512.xml
#merlin-xmlenc-five/encsig-hmac-sha256-dh.xml
#merlin-xmlenc-five/encsig-hmac-sha256-kw-tripledes-dh.xml

##########################################################################
#
# 01-phaos-xmlenc-3
#
##########################################################################

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-element-3des-kt-rsa1_5" \
    "tripledes-cbc rsa-1_5" \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret" \
    "--session-key des-192 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-element-3des-kt-rsa1_5.data --node-name http://example.org/paymentv2:CreditCard"  \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret"

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-element-3des-kt-rsa_oaep_sha1" \
    "tripledes-cbc rsa-oaep-mgf1p" \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret" \
    "--session-key des-192 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-element-3des-kt-rsa_oaep_sha1.data --node-name http://example.org/paymentv2:CreditCard"  \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret"

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-element-aes128-kt-rsa1_5" \
    "aes128-cbc rsa-1_5" \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret" \
    "--session-key aes-128 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-element-aes128-kt-rsa1_5.data --node-name http://example.org/paymentv2:CreditCard"  \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret"

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-element-aes128-kt-rsa_oaep_sha1" \
    "aes128-cbc rsa-oaep-mgf1p" \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret" \
    "--session-key aes-128 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-element-aes128-kt-rsa_oaep_sha1.data --node-name http://example.org/paymentv2:CreditCard"  \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret"

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-element-aes192-kt-rsa_oaep_sha1" \
    "aes192-cbc rsa-oaep-mgf1p" \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret" \
    "--session-key aes-192 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-element-aes192-kt-rsa_oaep_sha1.data --node-name http://example.org/paymentv2:CreditCard"  \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret"

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-text-aes192-kt-rsa1_5" \
    "aes192-cbc rsa-1_5" \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret" \
    "--session-key aes-192 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-text-aes192-kt-rsa1_5.data --node-name http://example.org/paymentv2:CreditCard"  \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret"

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-content-aes256-kt-rsa1_5" \
    "aes256-cbc rsa-1_5" \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret" \
    "--session-key aes-256 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-content-aes256-kt-rsa1_5.data --node-name http://example.org/paymentv2:CreditCard"  \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret"

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-text-aes256-kt-rsa_oaep_sha1" \
    "aes256-cbc rsa-oaep-mgf1p" \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret" \
    "--session-key aes-256 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-text-aes256-kt-rsa_oaep_sha1.data --node-name http://example.org/paymentv2:CreditCard"  \
    "$priv_key_option $topfolder/01-phaos-xmlenc-3/rsa-priv-key.$priv_key_format --pwd secret"

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-element-3des-kw-3des" \
    "tripledes-cbc kw-tripledes" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" \
    "--session-key des-192 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-element-3des-kw-3des.data --node-name http://example.org/paymentv2:CreditCard" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" 

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-content-aes128-kw-3des" \
    "aes128-cbc kw-tripledes" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" \
    "--session-key aes-128 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-content-aes128-kw-3des.data --node-name http://example.org/paymentv2:CreditCard" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" 

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-element-aes128-kw-aes128" \
    "aes128-cbc kw-aes128" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" \
    "--session-key aes-128 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-element-aes128-kw-aes128.data --node-name http://example.org/paymentv2:CreditCard" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" 

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-element-aes128-kw-aes256" \
    "aes128-cbc kw-aes256" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" \
    "--session-key aes-128 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-element-aes128-kw-aes256.data --node-name http://example.org/paymentv2:CreditCard" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" 

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-content-3des-kw-aes192" \
    "tripledes-cbc kw-aes192" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" \
    "--session-key des-192 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-content-3des-kw-aes192.data --node-name http://example.org/paymentv2:CreditCard" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" 

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-content-aes192-kw-aes256" \
    "aes192-cbc kw-aes256" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" \
    "--session-key aes-192 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-content-aes192-kw-aes256.data --node-name http://example.org/paymentv2:CreditCard" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" 

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-element-aes192-kw-aes192" \
    "aes192-cbc kw-aes192" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" \
    "--session-key aes-192 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-element-aes192-kw-aes192.data --node-name http://example.org/paymentv2:CreditCard" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" 

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-element-aes256-kw-aes256" \
    "aes256-cbc kw-aes256" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" \
    "--session-key aes-256 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-element-aes256-kw-aes256.data --node-name http://example.org/paymentv2:CreditCard" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" 

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-text-3des-kw-aes256" \
    "tripledes-cbc kw-aes256" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" \
    "--session-key des-192 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-text-3des-kw-aes256.data --node-name http://example.org/paymentv2:CreditCard"  \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml"

execEncTest $res_success \
    "" \
    "01-phaos-xmlenc-3/enc-text-aes128-kw-aes192" \
    "aes128-cbc kw-aes192" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml" \
    "--session-key aes-128 --keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-key-data key-name --xml-data $topfolder/01-phaos-xmlenc-3/enc-text-aes128-kw-aes192.data --node-name http://example.org/paymentv2:CreditCard"  \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml"

#01-phaos-xmlenc-3/enc-element-3des-ka-dh.xml
#01-phaos-xmlenc-3/enc-element-aes128-ka-dh.xml
#01-phaos-xmlenc-3/enc-element-aes192-ka-dh.xml
#01-phaos-xmlenc-3/enc-element-aes256-ka-dh.xml

#01-phaos-xmlenc-3/enc-element-3des-kt-rsa_oaep_sha256.xml
#01-phaos-xmlenc-3/enc-element-3des-kt-rsa_oaep_sha512.xml

##########################################################################
#
# test dynamicencryption
#
##########################################################################
if [ -n "$XMLSEC_TEST_NAME" -a "$XMLSEC_TEST_NAME" = "enc-dynamic" ]; then
echo "Dynamic encryption template"
printf "    Encrypt template                                     "
echo "$VALGRIND $xmlsec_app encrypt-tmpl $xmlsec_params --keys-file $keysfile --output $tmpfile" >> $logfile
$VALGRIND $xmlsec_app encrypt-tmpl $xmlsec_params --keys-file $keysfile --output $tmpfile >> $logfile 2>> $logfile
printRes $res_success $?
printf "    Decrypt document                                     "
echo "$VALGRIND $xmlsec_app decrypt $xmlsec_params $keysfile $tmpfile" >> $logfile
$VALGRIND $xmlsec_app decrypt $xmlsec_params --keys-file $keysfile $tmpfile >> $logfile 2>> $logfile
printRes $res_success $?
fi


##########################################################################
##########################################################################
##########################################################################
echo "--------- Negative Testing: Following tests MUST FAIL ----------"
echo "--- detailed log is written to  $logfile" 
execEncTest $res_fail \
    "" \
    "01-phaos-xmlenc-3/bad-alg-enc-element-aes128-kw-3des" \
    "" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml"

execEncTest $res_fail \
    "" \
    "aleksey-xmlenc-01/enc-aes192cbc-keyname-ref" \
    "" \
    "--keys-file $topfolder/keys/keys.xml --enabled-cipher-reference-uris empty" 

execEncTest $res_fail \
    "" \
    "01-phaos-xmlenc-3/enc-content-aes256-kt-rsa1_5" \
    "" \
    "--keys-file $topfolder/01-phaos-xmlenc-3/keys.xml --enabled-retrieval-method-uris empty"

rm -rf $tmpfile

##########################################################################
##########################################################################
##########################################################################
echo "--- testEnc finished" >> $logfile
echo "--- testEnc finished"
echo "--- detailed log is written to  $logfile"

