FROM registry.videolan.org/vlc-debian-win32-3.0:20211008142723

CMD cd /vlc && \
    (patch -p1 --forward src/modules/bank.c ignore-cache-time.patch || :) && \
    (patch -p1 --forward extras/package/win32/build.sh build-win.patch || :) && \
    mkdir -p build && \
    cd build && \
    export CONTRIBFLAGS="--disable-disc --disable-net --disable-sout --disable-asdcplib --disable-chromaprint --disable-gcrypt --disable-gme --disable-goom --disable-jpeg --disable-libarchive --disable-nfs --disable-png --disable-postproc --disable-sidplay2 --disable-SDL_image --disable-caca --disable-lua --disable-luac --disable-modplug --disable-mpcdec --disable-openjpeg --disable-projectM --disable-tiff --disable-zlib --disable-zvbi --disable-fluidlite --disable-aribb24 --disable-aribb25 --disable-ass --disable-fontconfig --disable-freetype2 --disable-fribidi --disable-harfbuzz --disable-kate --disable-gettext" && \
    export CONFIGFLAGS="--disable-sout --disable-lua --disable-vlm --disable-addonmanagermodules --disable-dbus --disable-archive --disable-live555 --disable-dc1394 --disable-dv1394 --disable-linsys --disable-dvdread --disable-dvdnav --disable-nfs --disable-v4l2 --disable-vcd --disable-libcddb --disable-screen --disable-smbclient --disable-freerdp --disable-realrtsp --disable-macosx-avfoundation --disable-asdcp --disable-gme --disable-sid --disable-mod --disable-mpc --disable-postproc --disable-png --disable-jpeg --disable-x264 --disable-x26410b --disable-mfx --disable-fluidsynth --disable-fluidlite  --disable-zvbi --disable-aribsub --disable-aribb25 --disable-telx --disable-sdl-image --disable-svg --disable-caca --disable-svgdec --disable-chromaprint --disable-chromecast --disable-qt --disable-skins2 --disable-macosx --disable-sparkle --disable-ncurses --disable-goom --disable-projectm --disable-vsxu --disable-avahi --disable-udev --disable-mtp --disable-upnp --disable-microdns --disable-libgcrypt --disable-gnutls --disable-secret --disable-kwallet --disable-update-check --disable-vlc" && \
    ../extras/package/win32/build.sh -a i686 -z && \
    cd win32 && \
    make package-win-strip
