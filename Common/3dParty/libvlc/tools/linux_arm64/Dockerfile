FROM ubuntu:xenial-20180417

RUN apt-get update && apt-get -y upgrade && \
    apt-get install -y wget

####################################
# from vlc-ubuntu-xenial Dockerfile
ENV PATH=$PATH:/usr/sbin

RUN echo "deb http://archive.neon.kde.org/user xenial main" >> /etc/apt/sources.list.d/neon.list && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys E6D4736255751E5D && \
    (apt-get update || :) && \
    apt-get install --no-install-suggests --no-install-recommends -y \
      ant autoconf automake autopoint bison build-essential flex g++ gettext \
      git git-core libasound2-dev libavahi-client-dev libcdio-dev libdbus-1-dev \
      libdirectfb-dev libegl1-mesa-dev libfreerdp-dev libfreetype6-dev libfribidi-dev \
      libgles2-mesa-dev libgnutls28-dev libgtk2.0-dev libidn11-dev libjack-dev \
      liblircclient-dev libltdl-dev liblua5.2-dev libmtp-dev libncursesw5-dev \
      libpng-dev libpulse-dev libqt5svg5-dev libqt5x11extras5-dev librsvg2-dev \
      libsecret-1-dev libtool libtool-bin libudev-dev libupnp-dev libv4l-dev \
      libvdpau-dev libx11-dev libxcb-composite0-dev libxcb-keysyms1-dev \
      libxcb-randr0-dev libxcb-shm0-dev libxcb-xfixes0-dev libxcb-xv0-dev libxcb1-dev \
      libxext-dev libxi-dev libxinerama-dev libxml2-dev libxpm-dev libzvbi-dev \
      locales lua5.2 make pkg-config qtbase5-dev qtbase5-private-dev ragel snapcraft \
      vim xz-utils yasm zlib1g-dev libxkbcommon-x11-dev libvulkan-dev \
    && \
    apt-get clean -y && rm -rf /var/lib/apt/lists/* && \
    locale-gen en_US.UTF-8 && \
    /usr/sbin/update-locale LANG=en_US.UTF-8 && \
    echo "export PATH=\$PATH:/usr/sbin" >> /etc/profile.d/vlc_env.sh

RUN apt-get install -y wget

ENV LANG=en_US.UTF-8
####################################

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
# LIBVA
RUN apt-get purge --autoremove -y libva-dev && \
    cd /build && \
    LIBVA_VERSION=2.21.0 && \
    wget -q https://github.com/intel/libva/archive/refs/tags/$LIBVA_VERSION.tar.gz && \
    tar -xzf $LIBVA_VERSION.tar.gz && \
    cd libva-$LIBVA_VERSION && \
# change 'private-code' to 'code' for older versions of `wayland-scanner`
    sed -i 's/$(AM_V_GEN)$(WAYLAND_SCANNER) private-code < $< > $@/$(AM_V_GEN)$(WAYLAND_SCANNER) code < $< > $@/' va/wayland/Makefile.am && \
    ./autogen.sh --prefix=/usr --libdir=/usr/lib/aarch64-linux-gnu && \
    make && make install

ENV PATH=/opt/protobuf/bin:$PATH

RUN rm -rf /build/

# build libvlc
CMD cd /vlc && \
    (patch -p1 --forward src/modules/bank.c ignore-cache-time.patch || :) && \
    ./bootstrap && \
    cd contrib && \
    mkdir -p native && \
    cd native && \
    ../bootstrap --disable-disc --disable-net --disable-sout --disable-a52 --disable-asdcplib --disable-chromaprint --disable-gcrypt --disable-gme --disable-goom --disable-jpeg --disable-libarchive --disable-nfs --disable-png --disable-postproc --disable-sidplay2 --disable-dvbpsi --disable-aribb24 --disable-ass --disable-kate --disable-openjpeg --disable-SDL_image && \
# on WSL2 based docker containers the ffi build may fail, so some lines in ffi configs need to be changed
    (make .ffi || (sed -i '/$(RECONF)/d' ../src/ffi/rules.mak && sed -i 's/mv $srcdir\/config.log/cp $srcdir\/config.log/' ffi/configure)) && \
# build other contribs
    (make -j$CORES || make) && \
    cd /vlc && \
    mkdir -p build/linux_arm64 && \
    ./configure --prefix=/vlc/build/linux_arm64 --disable-sout --disable-lua --disable-vlm --disable-addonmanagermodules --disable-dbus --disable-archive --disable-linsys --disable-nfs --disable-v4l2 --disable-vcd --disable-libcddb --disable-screen --disable-freerdp --disable-macosx-avfoundation --disable-asdcp --disable-dvbpsi --disable-gme --disable-sid --disable-mod --disable-mpc --disable-swscale --disable-postproc --disable-a52 --disable-png --disable-jpeg --disable-zvbi --disable-telx --disable-sdl-image --disable-svg --disable-svgdec --disable-chromaprint --disable-chromecast --disable-qt --disable-skins2 --disable-macosx --disable-sparkle --disable-ncurses --disable-goom --disable-projectm --disable-vsxu --disable-avahi --disable-udev --disable-mtp --disable-upnp --disable-microdns --disable-libxml2 --disable-libgcrypt --disable-gnutls --disable-secret --disable-kwallet --disable-vlc --disable-fontconfig --disable-freetype --disable-fribidi --disable-harfbuzz  && \
    make && \
    make -j$CORES install-strip && \
# correct build files
    cd build/linux_arm64 && \
# change rpaths
    echo "CHANGING RPATHS..." && \
    chmod +x /vlc/change-rpaths.sh && \
    /vlc/change-rpaths.sh && \
# copy libva libraries
    (for f in $(find /usr/lib/aarch64-linux-gnu/ -name 'libva*.so*'); do cp $f lib; done;) && \
# convert all symlinks to files
    echo "CONVERTING SYMLINKS..." && \
    (for f in $(find . -type l); do cp --remove-destination $(readlink -f $f) $f; done;) && \
# delete all .la files
    find lib -name "*.la" -type f -delete && \
# generate plugins.dat
    echo "GENERATING plugins.dat..." && \
    lib/vlc/vlc-cache-gen lib/vlc/plugins
