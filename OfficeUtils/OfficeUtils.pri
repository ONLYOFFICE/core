# Copyright (C) Ascensio System SIA, 2009-2026
#
# This program is a free software product. You can redistribute it and/or
# modify it under the terms of the GNU Affero General Public License (AGPL)
# version 3 as published by the Free Software Foundation, together with the
# additional terms provided in the LICENSE file.
#
# This program is distributed WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
# details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
#
# You can contact Ascensio System SIA by email at info@onlyoffice.com
# or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
# LV-1050, Latvia, European Union.
#
# The interactive user interfaces in modified versions of the Program
# are required to display Appropriate Legal Notices in accordance with
# Section 5 of the GNU AGPL version 3.
#
# No trademark rights are granted under this License.
#
# All non-code elements of the Product, including illustrations,
# icon sets, and technical writing content, are licensed under the
# Creative Commons Attribution-ShareAlike 4.0 International License:
# https://creativecommons.org/licenses/by-sa/4.0/legalcode
#
# This license applies only to such non-code elements and does not
# modify or replace the licensing terms applicable to the Program's
# source code, which remains licensed under the GNU Affero General
# Public License v3.
#
# SPDX-License-Identifier: AGPL-3.0-only

core_linux {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
}
core_mac {
    QMAKE_CXXFLAGS += -Wall -Wno-ignored-qualifiers
    DEFINES += unix
}

build_zlib_as_sources {
    DEFINES += BUILD_ZLIB_AS_SOURCES
}

INCLUDEPATH += \
    $$PWD/src/zlib-1.2.11/contrib/minizip \
    $$PWD/src/zlib-1.2.11 \
    $$PWD/src

SOURCES +=  \
    $$PWD/src/OfficeUtils.cpp \
    $$PWD/src/ZipBuffer.cpp \
    $$PWD/src/ZipUtilsCP.cpp \
    $$PWD/src/zlib_addon.c \
    $$PWD/src/zlib-1.2.11/contrib/minizip/ioapi.c \
    $$PWD/src/zlib-1.2.11/contrib/minizip/miniunz.c \
    $$PWD/src/zlib-1.2.11/contrib/minizip/minizip.c \
    $$PWD/src/zlib-1.2.11/contrib/minizip/mztools.c \
    $$PWD/src/zlib-1.2.11/contrib/minizip/unzip.c \
    $$PWD/src/zlib-1.2.11/contrib/minizip/zip.c \
    $$PWD/src/zlib-1.2.11/contrib/minizip/ioapibuf.c
core_windows {
SOURCES +=  \
    $$PWD/src/zlib-1.2.11/contrib/minizip/iowin32.c
}
build_all_zlib {
SOURCES += \
    $$PWD/src/zlib-1.2.11/adler32.c \
    $$PWD/src/zlib-1.2.11/compress.c \
    $$PWD/src/zlib-1.2.11/crc32.c \
    $$PWD/src/zlib-1.2.11/deflate.c \
    $$PWD/src/zlib-1.2.11/gzclose.c \
    $$PWD/src/zlib-1.2.11/gzlib.c \
    $$PWD/src/zlib-1.2.11/gzread.c \
    $$PWD/src/zlib-1.2.11/gzwrite.c \
    $$PWD/src/zlib-1.2.11/infback.c \
    $$PWD/src/zlib-1.2.11/inffast.c \
    $$PWD/src/zlib-1.2.11/inflate.c \
    $$PWD/src/zlib-1.2.11/inftrees.c \
    $$PWD/src/zlib-1.2.11/trees.c \
    $$PWD/src/zlib-1.2.11/uncompr.c \
    $$PWD/src/zlib-1.2.11/zutil.c
}	
	
HEADERS +=  \
    $$PWD/src/OfficeUtilsCommon.h \
    $$PWD/src/OfficeUtils.h \
    $$PWD/src/ZipBuffer.h \
    $$PWD/src/ZipFolder.h \
    $$PWD/src/ZipUtilsCP.h \
    $$PWD/src/zlib_addon.h \
    $$PWD/src/zlib-1.2.11/contrib/minizip/crypt.h \
    $$PWD/src/zlib-1.2.11/contrib/minizip/ioapi.h \
    $$PWD/src/zlib-1.2.11/contrib/minizip/mztools.h \
    $$PWD/src/zlib-1.2.11/contrib/minizip/unzip.h \
    $$PWD/src/zlib-1.2.11/contrib/minizip/zip.h \
    $$PWD/src/zlib-1.2.11/contrib/minizip/ioapibuf.h

