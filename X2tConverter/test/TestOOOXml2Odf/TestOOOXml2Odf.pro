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

QT       -= core
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += core_static_link_libstd

X2T_PATH = $$PWD/../..
CORE_ROOT_DIR = $$PWD/../../../../core
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
include($$CORE_ROOT_DIR/Common/3dParty/googletest/googletest.pri)

DESTDIR = $$PWD/build


INCLUDEPATH += $$CORE_ROOT_DIR/OdfFile
INCLUDEPATH += $$CORE_ROOT_DIR/OdfFile/Common
INCLUDEPATH += $$CORE_ROOT_DIR/Common
INCLUDEPATH += $$CORE_ROOT_DIR/OOXML/PPTXFormat/Logic


LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib 
LIBS += -L$$CORE_BOOST_LIBS

win32 {
LIBS += -lgdi32\
-luser32
}

ADD_DEPENDENCY(kernel)
ADD_DEPENDENCY(kernel_network)
ADD_DEPENDENCY(graphics)

ADD_DEPENDENCY(OdfFormatLib)
ADD_DEPENDENCY(PPTXFormatLib)
ADD_DEPENDENCY(BinDocument)
ADD_DEPENDENCY(DocxFormatLib)
ADD_DEPENDENCY(XlsbFormatLib)
ADD_DEPENDENCY(RtfFormatLib)
ADD_DEPENDENCY(CompoundFileLib)

ADD_DEPENDENCY(DocFormatLib XlsFormatLib PPTFormatLib VbaFormatLib)
ADD_DEPENDENCY(HtmlFile2)
ADD_DEPENDENCY(UnicodeConverter)


core_linux {
    LIBS += -Wl,-unresolved-symbols=ignore-in-shared-libs
    LIBS += -ldl
}

SOURCES += \
    test.cpp \
    $$CORE_ROOT_DIR/Common/OfficeFileFormatChecker2.cpp

DEFINES += DONT_WRITE_EMBEDDED_FONTS
