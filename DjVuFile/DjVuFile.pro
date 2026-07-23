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

QT       -= core gui

VERSION = 1.0.0.3
TARGET = DjVuFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

DEFINES += DJVU_USE_DYNAMIC_LIBRARY

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, PdfFile)

DEFINES -= \
    UNICODE \
    _UNICODE

!core_windows {
    DEFINES += \
    HAVE_UNISTD_H \
    HAVE_MBSTATE_T \
    GCONTAINER_NO_MEMBER_TEMPLATES="1" \
    HAS_WCHAR \
    HAVE_WCHAR_H \
    UNIX \
    HAVE_STDINCLUDES
}
core_android {
    DEFINES += HAS_MBSTATE="1"
}

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

SOURCES += DjVu.cpp \
    DjVuFileImplementation.cpp \
    libdjvu/Arrays.cpp \
    libdjvu/atomic.cpp \
    libdjvu/BSByteStream.cpp \
    libdjvu/BSEncodeByteStream.cpp \
    libdjvu/ByteStream.cpp \
    libdjvu/DataPool.cpp \
    libdjvu/debug.cpp \
    libdjvu/DjVmDir.cpp \
    libdjvu/DjVmDir0.cpp \
    libdjvu/DjVmDoc.cpp \
    libdjvu/DjVmNav.cpp \
    libdjvu/DjVuAnno.cpp \
    libdjvu/DjVuDocEditor.cpp \
    libdjvu/DjVuDocument.cpp \
    libdjvu/DjVuDumpHelper.cpp \
    libdjvu/DjVuErrorList.cpp \
    libdjvu/DjVuFile.cpp \
    libdjvu/DjVuFileCache.cpp \
    libdjvu/DjVuGlobal.cpp \
    libdjvu/DjVuGlobalMemory.cpp \
    libdjvu/DjVuImage.cpp \
    libdjvu/DjVuInfo.cpp \
    libdjvu/DjVuMessage.cpp \
    libdjvu/DjVuMessageLite.cpp \
    libdjvu/DjVuNavDir.cpp \
    libdjvu/DjVuPalette.cpp \
    libdjvu/DjVuPort.cpp \
    libdjvu/DjVuText.cpp \
    libdjvu/DjVuToPS.cpp \
    libdjvu/GBitmap.cpp \
    libdjvu/GContainer.cpp \
    libdjvu/GException.cpp \
    libdjvu/GIFFManager.cpp \
    libdjvu/GMapAreas.cpp \
    libdjvu/GOS.cpp \
    libdjvu/GPixmap.cpp \
    libdjvu/GRect.cpp \
    libdjvu/GScaler.cpp \
    libdjvu/GSmartPointer.cpp \
    libdjvu/GString.cpp \
    libdjvu/GThreads.cpp \
    libdjvu/GUnicode.cpp \
    libdjvu/GURL.cpp \
    libdjvu/IFFByteStream.cpp \
    libdjvu/IW44EncodeCodec.cpp \
    libdjvu/IW44Image.cpp \
    libdjvu/JB2EncodeCodec.cpp \
    libdjvu/JB2Image.cpp \
    libdjvu/JPEGDecoder.cpp \
    libdjvu/MMRDecoder.cpp \
    libdjvu/MMX.cpp \
    libdjvu/UnicodeByteStream.cpp \
    libdjvu/XMLParser.cpp \
    libdjvu/XMLTags.cpp \
    libdjvu/ZPCodec.cpp

HEADERS += DjVu.h \
    DjVuFileImplementation.h \
    libdjvu/Arrays.h \
    libdjvu/atomic.h \
    libdjvu/BSByteStream.h \
    libdjvu/ByteStream.h \
    libdjvu/DataPool.h \
    libdjvu/debug.h \
    libdjvu/DjVmDir.h \
    libdjvu/DjVmDir0.h \
    libdjvu/DjVmDoc.h \
    libdjvu/DjVmNav.h \
    libdjvu/DjVuAnno.h \
    libdjvu/DjVuDocEditor.h \
    libdjvu/DjVuDocument.h \
    libdjvu/DjVuDumpHelper.h \
    libdjvu/DjVuErrorList.h \
    libdjvu/DjVuFile.h \
    libdjvu/DjVuFileCache.h \
    libdjvu/DjVuGlobal.h \
    libdjvu/DjVuImage.h \
    libdjvu/DjVuInfo.h \
    libdjvu/DjVuMessage.h \
    libdjvu/DjVuMessageLite.h \
    libdjvu/DjVuNavDir.h \
    libdjvu/DjVuPalette.h \
    libdjvu/DjVuPort.h \
    libdjvu/DjVuText.h \
    libdjvu/DjVuToPS.h \
    libdjvu/GBitmap.h \
    libdjvu/GContainer.h \
    libdjvu/GException.h \
    libdjvu/GIFFManager.h \
    libdjvu/GMapAreas.h \
    libdjvu/GOS.h \
    libdjvu/GPixmap.h \
    libdjvu/GRect.h \
    libdjvu/GScaler.h \
    libdjvu/GSmartPointer.h \
    libdjvu/GString.h \
    libdjvu/GThreads.h \
    libdjvu/GURL.h \
    libdjvu/IFFByteStream.h \
    libdjvu/IW44Image.h \
    libdjvu/JB2Image.h \
    libdjvu/JPEGDecoder.h \
    libdjvu/MMRDecoder.h \
    libdjvu/MMX.h \
    libdjvu/UnicodeByteStream.h \
    libdjvu/XMLParser.h \
    libdjvu/XMLTags.h \
    libdjvu/ZPCodec.h

#DEFINES += BUILDING_WASM_MODULE
