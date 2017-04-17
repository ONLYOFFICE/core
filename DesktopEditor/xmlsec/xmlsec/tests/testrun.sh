#!/bin/sh

OS_ARCH=`uname -o`
OS_KERNEL=`uname -s`

#
# Get command line params
#
testfile="$1"
crypto="$2"
topfolder="$3"
xmlsec_app="$4"
file_format="$5"
timestamp=`date +%Y%m%d_%H%M%S`

if [ "z$OS_ARCH" = "zCygwin" ] ; then
    topfolder=`cygpath -wa "$topfolder"`
    xmlsec_app=`cygpath -a "$xmlsec_app"`
fi

#
# Prepare folders
#
if [ "z$TMPFOLDER" = "z" ] ; then
    TMPFOLDER=/tmp
fi
testname=`basename $testfile`
if [ "z$OS_ARCH" = "zCygwin" ] ; then
    tmpfile=`cygpath -wa $TMPFOLDER/$testname.$timestamp-$$.tmp`
    logfile=`cygpath -wa $TMPFOLDER/$testname.$timestamp-$$.log`
else
    tmpfile=$TMPFOLDER/$testname.$timestamp-$$.tmp
    logfile=$TMPFOLDER/$testname.$timestamp-$$.log
fi
nssdbfolder=$topfolder/nssdb

#
# Valgrind
#
valgrind_suppression="--suppressions=$topfolder/openssl.supp --suppressions=$topfolder/nss.supp"
valgrind_options="--leak-check=yes --show-reachable=yes --num-callers=32 -v"
if [ -n "$DEBUG_MEMORY" ] ; then 
    export VALGRIND="valgrind $valgrind_options"
    export REPEAT=3
    xmlsec_params="$xmlsec_params --repeat $REPEAT"
fi

#
# Setup crypto engine
#
crypto_config=$TMPFOLDER/xmlsec-crypto-config
keysfile=$crypto_config/keys.xml
if [ "z$XMLSEC_DEFAULT_CRYPTO" != "z" ] ; then
    xmlsec_params="$xmlsec_params --crypto $XMLSEC_DEFAULT_CRYPTO"
elif [ "z$crypto" != "z" ] ; then
    xmlsec_params="$xmlsec_params --crypto $crypto"
fi
xmlsec_params="$xmlsec_params --crypto-config $crypto_config"

#
# Setup keys config
#
pub_key_format=$file_format
cert_format=$file_format

#
# GCrypt/GnuTLS only supports DER format for now, others are good to go with PKCS12
#
if [ "z$crypto" != "zgcrypt" ] ; then
    priv_key_option="--pkcs12"
    priv_key_format="p12"
else
    priv_key_option="--privkey-der"
    priv_key_format="der"
    pub_key_format="der"
fi

# On Windows, one needs to specify Crypto Service Provider (CSP)
# in the pkcs12 file to ensure it is loaded correctly to be used
# with SHA2 algorithms. Worse, the CSP is different for XP and older 
# versions
if test "z$OS_ARCH" = "zCygwin" || test "z$OS_ARCH" = "zMsys" ; then
    # Samples:
    #   Cygwin	: CYGWIN_NT-5.1
    #   Msys	: MINGW32_NT-5.1
    if expr "$OS_KERNEL" : '.*_NT-5\.1' > /dev/null; then
        priv_key_suffix="-winxp"
    else
        priv_key_suffix="-win"
    fi
else
    priv_key_suffix=""
fi


#
# Misc
#
if [ -n "$PERF_TEST" ] ; then 
    xmlsec_params="$xmlsec_params --repeat $PERF_TEST"
fi

if test "z$OS_ARCH" = "zCygwin" || test "z$OS_ARCH" = "zMsys" ; then
    diff_param=-uw
else
    diff_param=-u
fi

#
# Check the command result and print it to stdout
#
res_success="success"
res_fail="fail"
printRes() {
    expected_res="$1"
    actual_res="$2"

    # convert status to string
    if [ $actual_res = 0 ]; then
        actual_res=$res_success
    else
        actual_res=$res_fail
    fi

    # check
    if [ "z$expected_res" = "z$actual_res" ] ; then
        echo "   OK"
    else
        echo " Fail"
    fi

    # memlog
    if [ -f .memdump ] ; then
        cat .memdump >> $logfile
    fi
}

#
# Keys Manager test function
#
execKeysTest() {
    expected_res="$1"
    req_key_data="$2"
    key_name="$3"
    alg_name="$4"

    if [ -n "$XMLSEC_TEST_NAME" -a "$XMLSEC_TEST_NAME" != "$key_name" ]; then
        return
    fi

    # prepare
    rm -f $tmpfile
    old_pwd=`pwd`

    # check params
    if [ "z$expected_res" != "z$res_success" -a "z$expected_res" != "z$res_fail" ] ; then
        echo " Bad parameter: expected_res=$expected_res"
        cd $old_pwd
        return
    fi
    echo "Test: $alg_name ($expected_res)"

    # check key data
    if [ -n "$req_key_data" ] ; then
        printf "    Checking required key data                            "
        echo "$xmlsec_app check-key-data $xmlsec_params $req_key_data" >> $logfile
        $xmlsec_app check-key-data $xmlsec_params $req_key_data >> $logfile 2>> $logfile
        res=$?
        if [ $res = 0 ]; then
            echo "   OK"
        else
            echo " Skip"
            return
        fi
    fi

    # run tests
    printf "    Creating new key                                      "
    params="--gen-key:$key_name $alg_name"
    if [ -f $keysfile ] ; then
        params="$params --keys-file $keysfile"
    fi
    echo "$VALGRIND $xmlsec_app keys $params $xmlsec_params $keysfile" >>  $logfile 
    $VALGRIND $xmlsec_app keys $params $xmlsec_params $keysfile >> $logfile 2>> $logfile
    printRes $expected_res $?

    # cleanup
    cd $old_pwd
    rm -f $tmpfile
}

#
# DSig test function
#
execDSigTest() {
    expected_res="$1"
    folder="$2"
    filename="$3"
    req_transforms="$4"
    req_key_data="$5"
    params1="$6"
    params2="$7"
    params3="$8"

    if [ -n "$XMLSEC_TEST_NAME" -a "$XMLSEC_TEST_NAME" != "$filename" ]; then
        return
    fi

    # prepare
    rm -f $tmpfile
    old_pwd=`pwd`

    # check params
    if [ "z$expected_res" != "z$res_success" -a "z$expected_res" != "z$res_fail" ] ; then
        echo " Bad parameter: expected_res=$expected_res"
        cd $old_pwd
        return
    fi
    if [ -n "$folder" ] ; then
        cd $topfolder/$folder
        full_file=$filename
        echo $folder/$filename
        echo "Test: $folder/$filename in folder " `pwd` " ($expected_res)" >> $logfile
    else
        full_file=$topfolder/$filename
        echo $filename
        echo "Test: $folder/$filename ($expected_res)" >> $logfile
    fi

    # check transforms
    if [ -n "$req_transforms" ] ; then
        printf "    Checking required transforms                         "
        echo "$xmlsec_app check-transforms $xmlsec_params $req_transforms" >> $logfile
        $xmlsec_app check-transforms $xmlsec_params $req_transforms >> $logfile 2>> $logfile
        res=$?
        if [ $res = 0 ]; then
            echo "   OK"
        else
            echo " Skip"
            cd $old_pwd
            return
        fi
    fi

    # check key data
    if [ -n "$req_key_data" ] ; then
        printf "    Checking required key data                            "
        echo "$xmlsec_app check-key-data $xmlsec_params $req_key_data" >> $logfile
        $xmlsec_app check-key-data $xmlsec_params $req_key_data >> $logfile 2>> $logfile
        res=$?
        if [ $res = 0 ]; then
            echo "  OK"
        else
            echo "Skip"
            return
        fi
    fi

    # run tests
    if [ -n "$params1" ] ; then
        printf "    Verify existing signature                            "
        echo "$VALGRIND $xmlsec_app verify $xmlsec_params $params1 $full_file.xml" >> $logfile
        $VALGRIND $xmlsec_app verify $xmlsec_params $params1 $full_file.xml >> $logfile 2>> $logfile
        printRes $expected_res $?
    fi

    if [ -n "$params2" -a -z "$PERF_TEST" ] ; then
        printf "    Create new signature                                 "
        echo "$VALGRIND $xmlsec_app sign $xmlsec_params $params2 --output $tmpfile $full_file.tmpl" >> $logfile
        $VALGRIND $xmlsec_app sign $xmlsec_params $params2 --output $tmpfile $full_file.tmpl >> $logfile 2>> $logfile
        printRes $expected_res $?
    fi

    if [ -n "$params3" -a -z "$PERF_TEST" ] ; then
        printf "    Verify new signature                                 "
        echo "$VALGRIND $xmlsec_app verify $xmlsec_params $params3 $tmpfile" >> $logfile
        $VALGRIND $xmlsec_app verify $xmlsec_params $params3 $tmpfile >> $logfile 2>> $logfile
        printRes $expected_res $?
    fi

    # cleanup
    cd $old_pwd
    rm -f $tmpfile
}

#
# Enc test function
#
execEncTest() {
    expected_res="$1"
    folder="$2"
    filename="$3"
    req_transforms="$4"
    params1="$5"
    params2="$6"
    params3="$7"

    if [ -n "$XMLSEC_TEST_NAME" -a "$XMLSEC_TEST_NAME" != "$filename" ]; then
        return
    fi

    # prepare
    rm -f $tmpfile $tmpfile.2
    old_pwd=`pwd`

    # check params
    if [ "z$expected_res" != "z$res_success" -a "z$expected_res" != "z$res_fail" ] ; then
        echo " Bad parameter: expected_res=$expected_res"
        cd $old_pwd
        return
    fi
    if [ -n "$folder" ] ; then
        cd $topfolder/$folder
        full_file=$filename
        echo $folder/$filename
        echo "Test: $folder/$filename in folder " `pwd` " ($expected_res)" >> $logfile
    else
        full_file=$topfolder/$filename
        echo $filename
        echo "Test: $folder/$filename ($expected_res)" >> $logfile
    fi

    # check transforms
    if [ -n "$req_transforms" ] ; then
        printf "    Checking required transforms                         "
        echo "$xmlsec_app check-transforms $xmlsec_params $req_transforms" >> $logfile
        $xmlsec_app check-transforms $xmlsec_params $req_transforms >> $logfile 2>> $logfile
        res=$?
        if [ $res = 0 ]; then
            echo "   OK"
        else
            echo " Skip"
            return
        fi
    fi

    # run tests
    if [ -n "$params1" ] ; then
        rm -f $tmpfile
        printf "    Decrypt existing document                            "
        echo "$VALGRIND $xmlsec_app decrypt $xmlsec_params $params1 $full_file.xml" >>  $logfile 
        $VALGRIND $xmlsec_app decrypt $xmlsec_params $params1 $full_file.xml > $tmpfile 2>> $logfile
        res=$?
        if [ $res = 0 ]; then
            diff $diff_param $full_file.data $tmpfile >> $logfile 2>> $logfile
            printRes $expected_res $?
        else
            printRes $expected_res $res
        fi
    fi

    if [ -n "$params2" -a -z "$PERF_TEST" ] ; then
        rm -f $tmpfile
        printf "    Encrypt document                                     "
        echo "$VALGRIND $xmlsec_app encrypt $xmlsec_params $params2 --output $tmpfile $full_file.tmpl" >>  $logfile 
        $VALGRIND $xmlsec_app encrypt $xmlsec_params $params2 --output $tmpfile $full_file.tmpl >> $logfile 2>> $logfile
        printRes $expected_res $?
    fi

    if [ -n "$params3" -a -z "$PERF_TEST" ] ; then 
        rm -f $tmpfile.2
        printf "    Decrypt new document                                 "
        echo "$VALGRIND $xmlsec_app decrypt $xmlsec_params $params3 --output $tmpfile.2 $tmpfile" >>  $logfile
        $VALGRIND $xmlsec_app decrypt $xmlsec_params $params3 --output $tmpfile.2 $tmpfile >> $logfile 2>> $logfile
        res=$?
        if [ $res = 0 ]; then
            diff $diff_param $full_file.data $tmpfile.2 >> $logfile 2>> $logfile
            printRes $expected_res $?
        else
            printRes $expected_res $res
        fi
    fi

    # cleanup
    cd $old_pwd
    rm -f $tmpfile $tmpfile.2
}

# prepare
rm -rf $tmpfile $tmpfile.2 tmpfile.3

# run tests
source "$testfile"

# cleanup
rm -rf $tmpfile $tmpfile.2 tmpfile.3

