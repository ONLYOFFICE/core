#===============================================================================
#
# Builds a Boost framework for iOS, iOS Simulator
# Creates a set of universal libraries that can be used on an iOS and in the
# iOS simulator.
#
# To configure the script, define:
#    BOOST_VERSION:   Which version of Boost to build (e.g. 1.72.0)
#    BOOST_VERSION2:  Same as BOOST_VERSION, but with _ instead of . (e.g. 1_72_0)
#    BOOST_LIBS:      Which Boost libraries to build
#    MIN_IOS_VERSION: Minimum iOS Target Version (e.g. 8.0)
#    IOS_SDK_VERSION: iOS SDK version (e.g. 9.0)
#
# If a boost tarball does not exist in the current directory, this script will
# attempt to download the version specified by BOOST_VERSION. You may also
# manually place a matching tarball in the current directory and the script
# will use that.
#
#===============================================================================

#BOOST_LIBS="date_time filesystem system regex"
BOOST_LIBS="regex"

BUILD_IOS=
CLEAN=

BOOST_VERSION=1.72.0
BOOST_VERSION2=1_72_0
MIN_IOS_VERSION=8.0
IOS_SDK_VERSION=`xcodebuild BITCODE_GENERATION_MODE="bitcode" ENABLE_BITCODE="YES" OTHER_CFLAGS="-fembed-bitcode" -showsdks | grep iphoneos | \
    egrep "[[:digit:]]+\.[[:digit:]]+" -o | tail -1`
OSX_SDK_VERSION=`xcodebuild BITCODE_GENERATION_MODE="bitcode" ENABLE_BITCODE="YES" OTHER_CFLAGS="-fembed-bitcode" -showsdks | grep macosx | \
    egrep "[[:digit:]]+\.[[:digit:]]+" -o | tail -1`

XCODE_ROOT=`xcode-select -print-path`

# The EXTRA_CPPFLAGS definition works around a thread race issue in
# shared_ptr. I encountered this historically and have not verified that
# the fix is no longer required. Without using the posix thread primitives
# an invalid compare-and-swap ARM instruction (non-thread-safe) was used for the
# shared_ptr use count causing nasty and subtle bugs.
#
# Should perhaps also consider/use instead: -BOOST_SP_USE_PTHREADS
EXTRA_CPPFLAGS="-DBOOST_AC_USE_PTHREADS -DBOOST_SP_USE_PTHREADS -g -DNDEBUG \
    -std=c++11 -stdlib=libc++ -fvisibility=hidden -fvisibility-inlines-hidden -fembed-bitcode"
EXTRA_IOS_CPPFLAGS="$EXTRA_CPPFLAGS -mios-version-min=$MIN_IOS_VERSION"
EXTRA_OSX_CPPFLAGS="$EXTRA_CPPFLAGS"

TARBALLDIR=`pwd`
SRCDIR=$TARBALLDIR

BOOST_TARBALL=$TARBALLDIR/boost_$BOOST_VERSION2.tar.bz2
BOOST_SRC=$SRCDIR/boost_${BOOST_VERSION2}
PREFIXDIR=$SRCDIR/build/ios

#===============================================================================
# Functions
#===============================================================================

usage()
{
cat << EOF
usage: $0 options
Build Boost for iOS, iOS Simulator

OPTIONS:
    -h, -\? | --help
        Display these options and exit.

    --boost-version
        Specify which version of Boost to build. Defaults to $BOOST_VERSION.

    -ios
        Build for the iOS platform. May be used in conjunction with -osx.
        If neither -ios nor -osx are specified, both are built.
    
    --ios-sdk [num]
        Specify the iOS SDK version to build with. Defaults to $IOS_SDK_VERSION.

    --min-ios-version [num]
        Specify the minimum iOS version to target.  Defaults to $MIN_IOS_VERSION.

    --clean
        Just clean up build artifacts, but don't actually build anything.

    --no-clean
        Do not clean up existing build artifacts before building.

EOF
}

abort()
{
    echo
    echo "Aborted: $@"
    exit 1
}

die()
{
    usage
    exit 1
}

missingParameter()
{
    echo $1 requires a parameter
    die
}

unknownParameter()
{
    if [[ -n $2 &&  $2 != "" ]]; then
        echo Unknown argument \"$2\" for parameter $1.
    else
        echo Unknown argument $1
    fi
    die
}

parseArgs()
{
    while [ "$1" != "" ]; do
        case $1 in
            -h | -\?)
                usage
                exit
                ;;

            -ios)
                BUILD_IOS=1
                ;;

            --boost-version)
                if [ -n $2 ]; then
                    BOOST_VERSION=$2
                    BOOST_VERSION2="${BOOST_VERSION//./_}"
                    BOOST_TARBALL=$TARBALLDIR/boost_$BOOST_VERSION2.tar.bz2
                    BOOST_SRC=$SRCDIR/boost_${BOOST_VERSION2}
                    shift
                else
                    missingParameter $1
                fi
                ;;

            --ios-sdk)
                if [ -n $2 ]; then
                    IOS_SDK_VERSION=$2
                    shift
                else
                    missingParameter $1
                fi
                ;;

            --min-ios-version)
                if [ -n $2 ]; then
                    MIN_IOS_VERSION=$2
                    shift
                else
                    missingParameter $1
                fi
                ;;

            --clean)
                CLEAN=1
                ;;

            *)
                unknownParameter $1
                ;;
        esac

        shift
    done
}

doneSection()
{
    echo
    echo "Done"
    echo "================================================================="
    echo
}

#===============================================================================

cleanup()
{
    echo Cleaning everything

    rm -rf $BOOST_SRC/iphone-build
    rm -rf $BOOST_SRC/iphonesim-build

    doneSection
}

#===============================================================================

inventMissingHeaders()
{
    # These files are missing in the ARM iPhoneOS SDK, but they are in the simulator.
    # They are supported on the device, so we copy them from x86 SDK to a staging area
    # to use them on ARM, too.
    echo Invent missing headers

    cp $XCODE_ROOT/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator${IOS_SDK_VERSION}.sdk/usr/include/{crt_externs,bzlib}.h $BOOST_SRC
}

#===============================================================================

updateBoost()
{
    echo Updating boost into $BOOST_SRC...
    
    if [ -f "$BOOST_SRC/tools/build/src/user-config.jam" ]; then
        mv $BOOST_SRC/tools/build/src/user-config.jam $BOOST_SRC/tools/build/src/user-config.jam-bk
    fi

    if [[ -n $BUILD_IOS ]]; then
        cat >> $BOOST_SRC/tools/build/src/user-config.jam <<EOF
using darwin : ${IOS_SDK_VERSION}~iphone
: $XCODE_ROOT/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -arch armv7 \
    -arch armv7s -arch arm64 -arch arm64e $EXTRA_IOS_CPPFLAGS
: <striper> <root>$XCODE_ROOT/Platforms/iPhoneOS.platform/Developer
: <architecture>arm <target-os>iphone
;
using darwin : ${IOS_SDK_VERSION}~iphonesim
: g++ -arch x86_64 $EXTRA_IOS_CPPFLAGS
: <striper> <root>$XCODE_ROOT/Platforms/iPhoneSimulator.platform/Developer
: <architecture>x86 <target-os>iphone
;
EOF
    fi

    doneSection
}

#===============================================================================

bootstrapBoost()
{
    cd $BOOST_SRC

    BOOST_LIBS_COMMA=$(echo $BOOST_LIBS | sed -e "s/ /,/g")
    echo "Bootstrapping (with libs $BOOST_LIBS_COMMA)"
    ./bootstrap.sh --with-libraries=$BOOST_LIBS_COMMA
    ./b2 headers

    doneSection
}

#===============================================================================

buildBoost()
{
    cd $BOOST_SRC

    if [[ -n $BUILD_IOS ]]; then
        echo Building Boost for iPhone
        # Install this one so we can copy the headers for the frameworks...
        ./b2 -j16 --build-dir=iphone-build --stagedir=iphone-build/stage \
            cxxflags="-fembed-bitcode" \
            --prefix=$PREFIXDIR toolset=darwin architecture=arm target-os=iphone \
            macosx-version=iphone-${IOS_SDK_VERSION} define=_LITTLE_ENDIAN \
            link=static stage
        ./b2 -j16 --build-dir=iphone-build --stagedir=iphone-build/stage \
            --prefix=$PREFIXDIR toolset=darwin architecture=arm \
            cxxflags="-fembed-bitcode" \
            target-os=iphone macosx-version=iphone-${IOS_SDK_VERSION} \
            define=_LITTLE_ENDIAN link=static install
        doneSection

        echo Building Boost for iPhoneSimulator
        ./b2 -j16 --build-dir=iphonesim-build --stagedir=iphonesim-build/stage \
            cxxflags="-fembed-bitcode" \
            toolset=darwin-${IOS_SDK_VERSION}~iphonesim architecture=x86 \
            target-os=iphone macosx-version=iphonesim-${IOS_SDK_VERSION} \
            link=static stage
        doneSection
    fi
}

#===============================================================================
generateOut()
{
    echo Generate Boost for iPhone
    mkdir -p $BOOST_SRC/build/ios/static
    for NAME in $BOOST_LIBS; do
        rm -f $SRCDIR/build/ios/lib/libboost_${NAME}.a
        lipo -create $BOOST_SRC/iphone-build/stage/lib/libboost_${NAME}.a \
                     $BOOST_SRC/iphonesim-build/stage/lib/libboost_${NAME}.a \
                     -o $SRCDIR/build/ios/lib/libboost_${NAME}.a
                     
        #lipo $SRCDIR/build/ios/lib/libboost_${NAME}.a -remove i386 -o $SRCDIR/build/ios/lib/libboost_${NAME}.a
        #lipo $SRCDIR/build/ios/lib/libboost_${NAME}.a -remove armv4t -o $SRCDIR/build/ios/lib/libboost_${NAME}.a
    done

    doneSection
}

#===============================================================================

restoreBoost()
{
    rm -f $BOOST_SRC/tools/build/src/user-config.jam
    if [ -f "$BOOST_SRC/tools/build/src/user-config.jam-bk" ]; then
        mv $BOOST_SRC/tools/build/src/user-config.jam-bk $BOOST_SRC/tools/build/src/user-config.jam
    fi
}

#===============================================================================
# Execution starts here
#===============================================================================

parseArgs $@

if [ -n "$CLEAN" ]; then
    cleanup
fi

if [[ -z $BUILD_IOS ]]; then
    BUILD_IOS=1
fi

format="%-20s %s\n"
printf "$format" "BUILD_IOS:" $( [[ -n $BUILD_IOS ]] && echo "YES" || echo "NO")
printf "$format" "BOOST_VERSION:" $BOOST_VERSION
printf "$format" "IOS_SDK_VERSION:" $IOS_SDK_VERSION
printf "$format" "MIN_IOS_VERSION:" $MIN_IOS_VERSION
printf "$format" "BOOST_LIBS:" "$BOOST_LIBS"
printf "$format" "BOOST_SRC:" $BOOST_SRC
printf "$format" "XCODE_ROOT:" $XCODE_ROOT
echo

inventMissingHeaders
bootstrapBoost
updateBoost
buildBoost
generateOut
restoreBoost

echo "Completed successfully"
