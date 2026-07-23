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

VERSION = 1.0.0.1
TARGET = OFDFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

DEFINES += OFD_USE_DYNAMIC_LIBRARY \
           CRYPTOPP_DISABLE_ASM

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, PdfFile)

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

INCLUDEPATH += \
	$$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include \
	$$CORE_ROOT_DIR/DesktopEditor/freetype-2.10.4/include/freetype

HEADERS += \
	OFDFile.h \
	src/Action.h \
	src/Annotation.h \
	src/Bookmark.h \
	src/Content/ImageObject.h \
	src/Content/PageBlock.h \
	src/Content/PathObject.h \
	src/IOFDElement.h \
	src/OFDFile_Private.h \
	src/Base.h \
	src/Content/Content.h \
	src/Content/GraphicUnit.h \
	src/Content/IPageBlock.h \
	src/Content/Layer.h \
	src/Content/TextObject.h \
	src/Document.h \
	src/OutlineElem.h \
	src/Page.h \
	src/Res.h \
	src/Types/Color.h \
	src/Types/ColorSpace.h \
	src/Types/CommonData.h \
	src/Types/CompositeGraphicUnit.h \
	src/Types/DrawParam.h \
	src/Types/Font.h \
	src/Types/MultiMedia.h \
	src/Types/PageArea.h \
	src/Types/PenSettings.h \
	src/Types/Region.h \
	src/Types/Signature.h \
	src/Types/TemplatePage.h \
	src/Utils/CFontChecker.h \
	src/Utils/Types.h \
	src/Utils/Utils.h \
	src/Utils/XmlReader.h

SOURCES += \
	OFDFile.cpp \
	src/Action.cpp \
	src/Annotation.cpp \
	src/Bookmark.cpp \
	src/Content/ImageObject.cpp \
	src/Content/PageBlock.cpp \
	src/Content/PathObject.cpp \
	src/OFDFile_Private.cpp \
	src/Base.cpp \
	src/Content/Content.cpp \
	src/Content/GraphicUnit.cpp \
	src/Content/Layer.cpp \
	src/Content/TextObject.cpp \
	src/Document.cpp \
	src/OutlineElem.cpp \
	src/Page.cpp \
	src/Res.cpp \
	src/Types/Color.cpp \
	src/Types/ColorSpace.cpp \
	src/Types/CommonData.cpp \
	src/Types/CompositeGraphicUnit.cpp \
	src/Types/DrawParam.cpp \
	src/Types/Font.cpp \
	src/Types/MultiMedia.cpp \
	src/Types/PageArea.cpp \
	src/Types/PenSettings.cpp \
	src/Types/Region.cpp \
	src/Types/Signature.cpp \
	src/Types/TemplatePage.cpp \
	src/Utils/CFontChecker.cpp \
	src/Utils/Types.cpp \
	src/Utils/XmlReader.cpp
