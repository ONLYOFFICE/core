# README #

Android x2t app example. 3d party build under linux only!

For run app you must:

1) Download all 3party libraries from folder ../core/Common/3dParty
2) Once! Run gradle task preBuildTask().
    That task download last NDK 17c with deprecated GCC (GCC needed only for openssl).
    Build all 3d party dependencies with that NDK
3) Build tests