#===============================================================================
#
# Builds a Boost framework for iOS, iOS Simulator, and OSX.
# Creates a set of universal libraries that can be used on an iOS and in the
# iOS simulator. Then creates a pseudo-framework to make using boost in Xcode
# less painful.
#
# To configure the script, define:
#    BOOST_VERSION:   Which version of Boost to build (e.g. 1.58.0)
#    BOOST_VERSION2:  Same as BOOST_VERSION, but with _ instead of . (e.g. 1_58_0)
#    BOOST_LIBS:      Which Boost libraries to build
#    MIN_IOS_VERSION: Minimum iOS Target Version (e.g. 8.0)
#    IOS_SDK_VERSION: iOS SDK version (e.g. 9.0)
#    OSX_SDK_VERSION: OSX SDK version (e.g. 10.11)
#
# If a boost tarball does not exist in the current directory, this script will
# attempt to download the version specified by BOOST_VERSION. You may also
# manually place a matching tarball in the current directory and the script
# will use that.
#
#===============================================================================

BOOST_LIBS="atomic chrono date_time exception filesystem program_options random signals system test thread regex"

BUILD_IOS=
BUILD_OSX=
CLEAN=
NO_CLEAN=
NO_FRAMEWORK=

BOOST_VERSION=1.58.0
BOOST_VERSION2=1_58_0
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
IOSOUTPUTDIR=$TARBALLDIR/ios
OSXOUTPUTDIR=$TARBALLDIR/osx
IOSBUILDDIR=$IOSOUTPUTDIR/build
OSXBUILDDIR=$OSXOUTPUTDIR/build
PREFIXDIR=$IOSOUTPUTDIR/prefix
IOSFRAMEWORKDIR=$IOSOUTPUTDIR/framework
OSXFRAMEWORKDIR=$OSXOUTPUTDIR/framework

BOOST_TARBALL=$TARBALLDIR/boost_$BOOST_VERSION2.tar.bz2
BOOST_SRC=$SRCDIR/boost_${BOOST_VERSION2}

ARM_DEV_CMD="xcrun --sdk iphoneos"
SIM_DEV_CMD="xcrun --sdk iphonesimulator"
OSX_DEV_CMD="xcrun --sdk macosx"

#===============================================================================
# Functions
#===============================================================================

usage()
{
cat << EOF
usage: $0 options
Build Boost for iOS, iOS Simulator, and OS X

OPTIONS:
    -h, -\? | --help
        Display these options and exit.

    --boost-version
        Specify which version of Boost to build. Defaults to $BOOST_VERSION.

    -ios
        Build for the iOS platform. May be used in conjunction with -osx.
        If neither -ios nor -osx are specified, both are built.

    -osx
        Build for the OS X platform. May be used in conjunction with -ios.
        If neither -ios nor -osx are specified, both are built.
    
    --ios-sdk [num]
        Specify the iOS SDK version to build with. Defaults to $IOS_SDK_VERSION.

    --min-ios-version [num]
        Specify the minimum iOS version to target.  Defaults to $MIN_IOS_VERSION.

    --osx-sdk [num]
        Specify the OS X SDK version to build with. Defaults to $OSX_SDK_VERSION.

    --no-framework
        Do not create the framework.

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

            -osx)
                BUILD_OSX=1
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

            --osx-sdk)
                 if [ -n $2 ]; then
                    OSX_SDK_VERSION=$2
                    shift
                else
                    missingParameter $1
                fi
                ;;

            --clean)
                CLEAN=1
                ;;

            --no-clean)
                NO_CLEAN=1
                ;;

            --no-framework)
                NO_FRAMEWORK=1
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
    rm -rf $BOOST_SRC/osx-build
    rm -rf $IOSOUTPUTDIR
    rm -rf $OSXOUTPUTDIR

    doneSection
}

#===============================================================================

downloadBoost()
{
    if [ ! -s $BOOST_TARBALL ]; then
        echo "Downloading boost ${BOOST_VERSION}"
        curl -L -o $BOOST_TARBALL \
            http://sourceforge.net/projects/boost/files/boost/${BOOST_VERSION}/boost_${BOOST_VERSION2}.tar.bz2/download
    fi

    doneSection
}

#===============================================================================

unpackBoost()
{
    [ -f "$BOOST_TARBALL" ] || abort "Source tarball missing."

    echo Unpacking boost into $SRCDIR...

    [ -d $SRCDIR ]    || mkdir -p $SRCDIR
    [ -d $BOOST_SRC ] || ( cd $SRCDIR; tar xfj $BOOST_TARBALL )
    [ -d $BOOST_SRC ] && echo "    ...unpacked as $BOOST_SRC"

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

    mv $BOOST_SRC/tools/build/src/user-config.jam $BOOST_SRC/tools/build/src/user-config.jam-bk

    if [[ -n $BUILD_IOS ]]; then
        cat >> $BOOST_SRC/tools/build/src/user-config.jam <<EOF
using darwin : ${IOS_SDK_VERSION}~iphone
: $XCODE_ROOT/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -arch armv7 \
    -arch armv7s -arch arm64 $EXTRA_IOS_CPPFLAGS
: <striper> <root>$XCODE_ROOT/Platforms/iPhoneOS.platform/Developer
: <architecture>arm <target-os>iphone
;
using darwin : ${IOS_SDK_VERSION}~iphonesim
: g++ -arch i386 -arch x86_64 $EXTRA_IOS_CPPFLAGS
: <striper> <root>$XCODE_ROOT/Platforms/iPhoneSimulator.platform/Developer
: <architecture>x86 <target-os>iphone
;
EOF
    fi

    if [[ -n $BUILD_OSX ]]; then
        cat >> $BOOST_SRC/tools/build/src/user-config.jam <<EOF
using darwin : ${OSX_SDK_VERSION}
: g++ -arch i386 -arch x86_64 $EXTRA_OSX_CPPFLAGS
: <striper> <root>$XCODE_ROOT/Platforms/MacOSX.platform/Developer
: <architecture>x86 <target-os>darwin
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

    if [[ -n $BUILD_OSX ]]; then
        echo building Boost for OSX
        ./b2 -j16 --build-dir=osx-build --stagedir=osx-build/stage toolset=clang \
            cxxflags="-std=c++11 -stdlib=libc++ -arch i386 -arch x86_64" \
            linkflags="-stdlib=libc++" link=static threading=multi \
            macosx-version=${OSX_SDK_VERSION} stage

        # If we are only building for OS X and we are outputting a framework,
        # then we need to install this one so we can copy the headers
        if [[ -z $BUILD_IOS && -z $NO_FRAMEWORK ]]; then
            PREFIXDIR=$OSXBUILDDIR/prefix
            ./b2 -j16 --build-dir=osx-build --stagedir=osx-build/stage \
                --prefix=$PREFIXDIR toolset=clang \
                cxxflags="-std=c++11 -stdlib=libc++ -arch i386 -arch x86_64" \
                linkflags="-stdlib=libc++" link=static threading=multi \
                macosx-version=${OSX_SDK_VERSION} install
        fi

        doneSection
    fi
}

#===============================================================================

unpackArchive()
{
    BUILDDIR=$1
    LIBNAME=$2

    echo "Unpacking $LIBNAME"

    if [[ -d $BUILDDIR/$LIBNAME ]]; then 
        rm $BUILDDIR/$LIBNAME/*.o
        rm $BUILDDIR/$LIBNAME/*.SYMDEF*
    else
        mkdir -p $BUILDDIR/$LIBNAME
    fi

    (
        cd $BUILDDIR/$NAME; ar -x ../../libboost_$NAME.a;
        for FILE in *.o; do
            NEW_FILE="${NAME}_${FILE}"
            mv $FILE $NEW_FILE
        done
    )
}

scrunchAllLibsTogetherInOneLibPerPlatform()
{
    cd $BOOST_SRC

    if [[ -n $BUILD_IOS ]]; then
        # iOS Device
        mkdir -p $IOSBUILDDIR/armv7/obj
        mkdir -p $IOSBUILDDIR/armv7s/obj
        mkdir -p $IOSBUILDDIR/arm64/obj

        # iOS Simulator
        mkdir -p $IOSBUILDDIR/i386/obj
        mkdir -p $IOSBUILDDIR/x86_64/obj
    fi

    if [[ -n $BUILD_OSX ]]; then
        # OSX
        mkdir -p $OSXBUILDDIR/i386/obj
        mkdir -p $OSXBUILDDIR/x86_64/obj
    fi

    ALL_LIBS=""

    echo Splitting all existing fat binaries...

    for NAME in $BOOST_LIBS; do
        if [ "$NAME" == "test" ]; then
            NAME="unit_test_framework"
        fi

        ALL_LIBS="$ALL_LIBS libboost_$NAME.a"

        if [[ -n $BUILD_IOS ]]; then
            $ARM_DEV_CMD lipo "iphone-build/stage/lib/libboost_$NAME.a" \
                -thin armv7 -o $IOSBUILDDIR/armv7/libboost_$NAME.a
            $ARM_DEV_CMD lipo "iphone-build/stage/lib/libboost_$NAME.a" \
                -thin armv7s -o $IOSBUILDDIR/armv7s/libboost_$NAME.a
            $ARM_DEV_CMD lipo "iphone-build/stage/lib/libboost_$NAME.a" \
                -thin arm64 -o $IOSBUILDDIR/arm64/libboost_$NAME.a

            $SIM_DEV_CMD lipo "iphonesim-build/stage/lib/libboost_$NAME.a" \
                -thin i386 -o $IOSBUILDDIR/i386/libboost_$NAME.a
            $SIM_DEV_CMD lipo "iphonesim-build/stage/lib/libboost_$NAME.a" \
                -thin x86_64 -o $IOSBUILDDIR/x86_64/libboost_$NAME.a
        fi

        if [[ -n $BUILD_OSX ]]; then
            $OSX_DEV_CMD lipo "osx-build/stage/lib/libboost_$NAME.a" \
                -thin i386 -o $OSXBUILDDIR/i386/libboost_$NAME.a
            $OSX_DEV_CMD lipo "osx-build/stage/lib/libboost_$NAME.a" \
                -thin x86_64 -o $OSXBUILDDIR/x86_64/libboost_$NAME.a
        fi
    done

    echo "Decomposing each architecture's .a files"

    for NAME in $BOOST_LIBS; do
        if [ "$NAME" == "test" ]; then
            NAME="unit_test_framework"
        fi

        echo "Decomposing libboost_${NAME}.a"
        if [[ -n $BUILD_IOS ]]; then
            unpackArchive "$IOSBUILDDIR/armv7/obj" $NAME
            unpackArchive "$IOSBUILDDIR/armv7s/obj" $NAME
            unpackArchive "$IOSBUILDDIR/arm64/obj" $NAME

            unpackArchive "$IOSBUILDDIR/i386/obj" $NAME
            unpackArchive "$IOSBUILDDIR/x86_64/obj" $NAME
        fi

        if [[ -n $BUILD_OSX ]]; then
            unpackArchive "$OSXBUILDDIR/i386/obj" $NAME
            unpackArchive "$OSXBUILDDIR/x86_64/obj" $NAME
        fi
    done

    echo "Linking each architecture into an uberlib ($ALL_LIBS => libboost.a )"
    if [[ -n $BUILD_IOS ]]; then
        rm $IOSBUILDDIR/*/libboost.a
    fi
    if [[ -n $BUILD_OSX ]]; then
        rm $OSXBUILDDIR/*/libboost.a
    fi

    for NAME in $BOOST_LIBS; do
        if [ "$NAME" == "test" ]; then
            NAME="unit_test_framework"
        fi

        echo $NAME

        if [[ -n $BUILD_IOS ]]; then
            echo ...armv7
            (cd $IOSBUILDDIR/armv7; $ARM_DEV_CMD ar crus libboost.a obj/$NAME/*.o; )
            echo ...armv7s
            (cd $IOSBUILDDIR/armv7s; $ARM_DEV_CMD ar crus libboost.a obj/$NAME/*.o; )
            echo ...arm64
            (cd $IOSBUILDDIR/arm64; $ARM_DEV_CMD ar crus libboost.a obj/$NAME/*.o; )

            echo ...i386
            (cd $IOSBUILDDIR/i386;  $SIM_DEV_CMD ar crus libboost.a obj/$NAME/*.o; )
            echo ...x86_64
            (cd $IOSBUILDDIR/x86_64;  $SIM_DEV_CMD ar crus libboost.a obj/$NAME/*.o; )
        fi

        if [[ -n $BUILD_OSX ]]; then
            echo ...osx-i386
            (cd $OSXBUILDDIR/i386;  $OSX_DEV_CMD ar crus libboost.a obj/$NAME/*.o; )
            echo ...osx-x86_64
            (cd $OSXBUILDDIR/x86_64;  $OSX_DEV_CMD ar crus libboost.a obj/$NAME/*.o; )
        fi
    done
}

#===============================================================================
buildFramework()
{
    : ${1:?}
    FRAMEWORKDIR=$1
    BUILDDIR=$2

    VERSION_TYPE=Alpha
    FRAMEWORK_NAME=boost
    FRAMEWORK_VERSION=A

    FRAMEWORK_CURRENT_VERSION=$BOOST_VERSION
    FRAMEWORK_COMPATIBILITY_VERSION=$BOOST_VERSION

    FRAMEWORK_BUNDLE=$FRAMEWORKDIR/$FRAMEWORK_NAME.framework
    echo "Framework: Building $FRAMEWORK_BUNDLE from $BUILDDIR..."

    rm -rf $FRAMEWORK_BUNDLE

    echo "Framework: Setting up directories..."
    mkdir -p $FRAMEWORK_BUNDLE
    mkdir -p $FRAMEWORK_BUNDLE/Versions
    mkdir -p $FRAMEWORK_BUNDLE/Versions/$FRAMEWORK_VERSION
    mkdir -p $FRAMEWORK_BUNDLE/Versions/$FRAMEWORK_VERSION/Resources
    mkdir -p $FRAMEWORK_BUNDLE/Versions/$FRAMEWORK_VERSION/Headers
    mkdir -p $FRAMEWORK_BUNDLE/Versions/$FRAMEWORK_VERSION/Documentation

    echo "Framework: Creating symlinks..."
    ln -s $FRAMEWORK_VERSION               $FRAMEWORK_BUNDLE/Versions/Current
    ln -s Versions/Current/Headers         $FRAMEWORK_BUNDLE/Headers
    ln -s Versions/Current/Resources       $FRAMEWORK_BUNDLE/Resources
    ln -s Versions/Current/Documentation   $FRAMEWORK_BUNDLE/Documentation
    ln -s Versions/Current/$FRAMEWORK_NAME $FRAMEWORK_BUNDLE/$FRAMEWORK_NAME

    FRAMEWORK_INSTALL_NAME=$FRAMEWORK_BUNDLE/Versions/$FRAMEWORK_VERSION/$FRAMEWORK_NAME

    echo "Lipoing library into $FRAMEWORK_INSTALL_NAME..."
    $ARM_DEV_CMD lipo -create $BUILDDIR/*/libboost.a -o "$FRAMEWORK_INSTALL_NAME" || abort "Lipo $1 failed"

    echo "Framework: Copying includes..."
    cp -r $PREFIXDIR/include/boost/*  $FRAMEWORK_BUNDLE/Headers/

    echo "Framework: Creating plist..."
    cat > $FRAMEWORK_BUNDLE/Resources/Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
<key>CFBundleDevelopmentRegion</key>
<string>English</string>
<key>CFBundleExecutable</key>
<string>${FRAMEWORK_NAME}</string>
<key>CFBundleIdentifier</key>
<string>org.boost</string>
<key>CFBundleInfoDictionaryVersion</key>
<string>6.0</string>
<key>CFBundlePackageType</key>
<string>FMWK</string>
<key>CFBundleSignature</key>
<string>????</string>
<key>CFBundleVersion</key>
<string>${FRAMEWORK_CURRENT_VERSION}</string>
</dict>
</plist>
EOF

    doneSection
}

#===============================================================================

restoreBoost()
{
    cp $BOOST_SRC/tools/build/src/user-config.jam-bk $BOOST_SRC/tools/build/src/user-config.jam
}

#===============================================================================
# Execution starts here
#===============================================================================

parseArgs $@

if [ -n "$CLEAN" ]; then
    cleanup
    exit
fi

if [[ -z $BUILD_IOS && -z $BUILD_OSX ]]; then
    BUILD_IOS=1
    BUILD_OSX=1
fi

format="%-20s %s\n"
printf "$format" "BUILD_IOS:" $( [[ -n $BUILD_IOS ]] && echo "YES" || echo "NO")
printf "$format" "BUILD_OSX:" $( [[ -n $BUILD_OSX ]] && echo "YES" || echo "NO")
printf "$format" "BOOST_VERSION:" $BOOST_VERSION
printf "$format" "IOS_SDK_VERSION:" $IOS_SDK_VERSION
printf "$format" "OSX_SDK_VERSION:" $OSX_SDK_VERSION
printf "$format" "MIN_IOS_VERSION:" $MIN_IOS_VERSION
printf "$format" "BOOST_LIBS:" "$BOOST_LIBS"
printf "$format" "BOOST_SRC:" $BOOST_SRC
printf "$format" "IOSBUILDDIR:" $IOSBUILDDIR
printf "$format" "OSXBUILDDIR:" $OSXBUILDDIR
printf "$format" "PREFIXDIR:" $PREFIXDIR
printf "$format" "IOSFRAMEWORKDIR:" $IOSFRAMEWORKDIR
printf "$format" "OSXFRAMEWORKDIR:" $OSXFRAMEWORKDIR
printf "$format" "XCODE_ROOT:" $XCODE_ROOT
echo

if [ -z $NO_CLEAN ]; then
    cleanup
fi

#downloadBoost
#unpackBoost
inventMissingHeaders
bootstrapBoost
updateBoost
buildBoost
scrunchAllLibsTogetherInOneLibPerPlatform

if [ -z $NO_FRAMEWORK ]; then
    if [[ -n $BUILD_IOS ]]; then
        buildFramework $IOSFRAMEWORKDIR $IOSBUILDDIR
    fi
    if [[ -n $BUILD_OSX ]]; then
        buildFramework $OSXFRAMEWORKDIR $OSXBUILDDIR
    fi
fi

restoreBoost

echo "Completed successfully"
