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

VERSION = 1.0.0.4
TARGET = DocxRenderer
TEMPLATE = lib

CONFIG += c++11
CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

DEFINES += DOCXRENDERER_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(UnicodeConverter, kernel, graphics)

# Flag for disable full document creation. Enabled in pdf editor
#CONFIG += disable_full_document_creation

core_windows {
LIBS += \
	-lgdi32 \
	-ladvapi32 \
	-luser32 \
	-lshell32
}

HEADERS += \
	$$PWD_ROOT_DIR/src/logic/elements/BaseItem.h \
	$$PWD_ROOT_DIR/src/logic/elements/ContText.h \
	$$PWD_ROOT_DIR/src/logic/elements/Paragraph.h \
	$$PWD_ROOT_DIR/src/logic/elements/Shape.h \
	$$PWD_ROOT_DIR/src/logic/elements/Table.h \
	$$PWD_ROOT_DIR/src/logic/elements/TextLine.h \
	$$PWD_ROOT_DIR/src/logic/managers/ExternalImageStorage.h \
	$$PWD_ROOT_DIR/src/logic/managers/FontStyleManager.h \
	$$PWD_ROOT_DIR/src/logic/managers/ImageManager.h \
	$$PWD_ROOT_DIR/src/logic/managers/FontManager.h \
	$$PWD_ROOT_DIR/src/logic/managers/ParagraphStyleManager.h \
	$$PWD_ROOT_DIR/src/logic/styles/FontStyle.h \
	$$PWD_ROOT_DIR/src/logic/styles/ParagraphStyle.h \
	$$PWD_ROOT_DIR/src/resources/ColorTable.h \
	$$PWD_ROOT_DIR/src/resources/Constants.h \
	$$PWD_ROOT_DIR/src/resources/ImageInfo.h \
	$$PWD_ROOT_DIR/src/resources/LinesTable.h \
	$$PWD_ROOT_DIR/src/resources/VectorGraphics.h \
	$$PWD_ROOT_DIR/src/resources/resources.h \
	$$PWD_ROOT_DIR/src/resources/utils.h \
	$$PWD_ROOT_DIR/src/logic/Page.h \
	$$PWD_ROOT_DIR/src/logic/Document.h \
	$$PWD_ROOT_DIR/DocxRenderer.h

SOURCES += \
	$$PWD_ROOT_DIR/src/logic/elements/BaseItem.cpp \
	$$PWD_ROOT_DIR/src/logic/elements/ContText.cpp \
	$$PWD_ROOT_DIR/src/logic/elements/Paragraph.cpp \
	$$PWD_ROOT_DIR/src/logic/elements/Shape.cpp \
	$$PWD_ROOT_DIR/src/logic/elements/TextLine.cpp \
	$$PWD_ROOT_DIR/src/logic/managers/FontManager.cpp \
	$$PWD_ROOT_DIR/src/logic/managers/FontStyleManager.cpp \
	$$PWD_ROOT_DIR/src/logic/managers/ImageManager.cpp \
	$$PWD_ROOT_DIR/src/logic/managers/ParagraphStyleManager.cpp \
	$$PWD_ROOT_DIR/src/logic/styles/FontStyle.cpp \
	$$PWD_ROOT_DIR/src/logic/Page.cpp \
	$$PWD_ROOT_DIR/src/logic/Document.cpp \
	$$PWD_ROOT_DIR/src/logic/styles/ParagraphStyle.cpp \
	$$PWD_ROOT_DIR/src/resources/VectorGraphics.cpp \
	$$PWD_ROOT_DIR/DocxRenderer.cpp

disable_full_document_creation {
	DEFINES += DISABLE_FULL_DOCUMENT_CREATION
} else {
	SOURCES += \
		$$PWD_ROOT_DIR/src/resources/resources.cpp
}

DISTFILES += \
	$$PWD_ROOT_DIR/readme.md
