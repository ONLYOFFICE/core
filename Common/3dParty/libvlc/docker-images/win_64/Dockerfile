FROM registry.videolan.org/vlc-debian-win64-3.0:20211008140026

CMD cd /vlc && \
    mkdir -p build && \
    cd build && \
    ../extras/package/win32/build.sh -a x86_64 -z && \
    cd win64 && \
    make package-win-common
