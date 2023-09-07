FROM registry.videolan.org/vlc-ubuntu-xenial:20181109145051

ARG CORES=8

RUN rm -f /etc/apt/sources.list.d/neon.list && \
    apt-get update && \
    apt-get purge --autoremove -y cmake meson && \
    apt-get install -y libssl-dev && \
    mkdir /build

# CMAKE
RUN cd /build && \
    CMAKE_VERSION=3.16.3 && CMAKE_VERSION_SHORT=3.16 && \
    wget -q https://cmake.org/files/v$CMAKE_VERSION_SHORT/cmake-$CMAKE_VERSION.tar.gz && \
    tar -xzf cmake-$CMAKE_VERSION.tar.gz && \
    cd cmake-$CMAKE_VERSION && \
    ./bootstrap && \
    make -j$CORES && make install
# NASM
RUN cd /build && \
    NASM_VERSION=2.14 && \
    wget --no-check-certificate -q https://nasm.us/pub/nasm/releasebuilds/$NASM_VERSION/nasm-$NASM_VERSION.tar.gz && \
    tar -xzf nasm-$NASM_VERSION.tar.gz && \
    cd nasm-$NASM_VERSION && \
    ./autogen.sh && ./configure && \
    make -j$CORES && make install
# PYTHON 3.7
RUN cd /build && \
    PYTHON_VERSION=3.7.9 && \
    wget -q https://python.org/ftp/python/$PYTHON_VERSION/Python-$PYTHON_VERSION.tgz && \
    tar -xzf Python-$PYTHON_VERSION.tgz && \
    cd Python-$PYTHON_VERSION && \
    ./configure --enable-optimizations && \
    make -j$CORES altinstall
# MESON & NINJA
RUN pip3.7 install meson ninja
# PROTOBUF
RUN cd /build && \
    PROTOBUF_VERSION=3.4.1 && \
    wget -q https://github.com/google/protobuf/releases/download/v$PROTOBUF_VERSION/protobuf-cpp-$PROTOBUF_VERSION.tar.gz && \
    tar -xzf protobuf-cpp-$PROTOBUF_VERSION.tar.gz && \
    cd protobuf-$PROTOBUF_VERSION && \
    mkdir build && \
# force include <algorithm> \
    sed -i.orig 's,#ifdef _MSC_VER,#if 1,' "src/google/protobuf/repeated_field.h" && \
    cmake  -S cmake -B build -DBUILD_SHARED_LIBS=OFF -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_EXAMPLES=OFF && \
    cmake --build build --parallel $CORES && cmake --install build --prefix /opt/protobuf

ENV PATH=/opt/protobuf/bin:$PATH

RUN rm -rf /build/

# build libvlc
CMD cd /vlc && \
    (patch -p1 --forward src/modules/bank.c ignore-cache-time.patch || :) && \
    ./bootstrap && \
    cd contrib && \
    mkdir -p native && \
    cd native && \
    ../bootstrap --disable-disc --disable-net --disable-sout && \
# on WSL2 based docker containers the ffi build may fail, so some lines in ffi configs need to be changed
    (make .ffi || (sed -i '/$(RECONF)/d' ../src/ffi/rules.mak && sed -i 's/mv $srcdir\/config.log/cp $srcdir\/config.log/' ffi/configure)) && \
# build other contribs
    (make -j$CORES || make) && \
    cd /vlc && \
    mkdir -p build/linux_64 && \
    ./configure --prefix=/vlc/build/linux_64 --disable-vlc --disable-qt --disable-skins2 --disable-sout --disable-lua --disable-vlm --disable-addonmanagermodules --disable-chromecast --disable-chromaprint --disable-zvbi --disable-telx --disable-linsys --disable-upnp && \
    make && \
    make -j$CORES install-strip && \
# correct build files
    cd build/linux_64 && \
# convert all symlinks to files
    (for f in $(find . -type l); do cp --remove-destination $(readlink -f $f) $f; done;) && \
# delete all .la files
    find lib -name "*.la" -type f -delete && \
# generate plugins.dat
    lib/vlc/vlc-cache-gen lib/vlc/plugins
