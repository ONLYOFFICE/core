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

CONFIG -= qt
QT -= core gui

TARGET   = PptxTxtConverterTest
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
DEFINES += DONT_WRITE_EMBEDDED_FONTS

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)
LIBS += -L$$CORE_BOOST_LIBS

SOURCES += \
    $$CORE_ROOT_DIR/Common/OfficeFileFormatChecker2.cpp \
    $$CORE_ROOT_DIR/X2tConverter/src/cextracttools.cpp \
    $$CORE_ROOT_DIR/X2tConverter/src/ASCConverters.cpp

HEADERS += \
    $$CORE_ROOT_DIR/Common/OfficeFileFormatChecker.h \
    $$CORE_ROOT_DIR/X2tConverter/src/cextracttools.h \
    $$CORE_ROOT_DIR/X2tConverter/src/ASCConverters.h

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lVbaFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lOdfFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lDocFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPptFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lRtfFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lTxtXmlFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lBinDocument
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lPPTXFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lDocxFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lXlsbFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lXlsFormatLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCompoundFileLib
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, kernel_network, Fb2File, PdfFile, HtmlFile2, EpubFile, XpsFile, OFDFile, DjVuFile, doctrenderer, DocxRenderer, IWorkFile, HWPFile)

SOURCES += \
    main.cpp
