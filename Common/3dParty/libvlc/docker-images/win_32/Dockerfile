FROM registry.videolan.org/vlc-debian-win32-3.0:20211008142723

CMD cd /vlc && \
    mkdir -p build && \
    cd build && \
    ../extras/package/win32/build.sh -a i686 -z && \
    cd win32 && \
    make package-win-common
