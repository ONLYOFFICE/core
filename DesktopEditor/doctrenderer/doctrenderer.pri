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

VERSION = 1.0.0.3
TARGET = doctrenderer
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD

PWD_CUR = $$PWD
include($$PWD_CUR/../../Common/base.pri)

DEFINES += DOCTRENDERER_USE_DYNAMIC_LIBRARY_BUILDING
DEFINES += JSBASE_USE_DYNAMIC_LIBRARY_BUILDING
#DEFINES += DOCBUILDER_V8_ALLOW_WASM
ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, kernel_network)

#CONFIG += build_xp
#CONFIG += v8_version_60
core_android:DEFINES += DISABLE_MEMORY_LIMITATION

HEADERS += \
	$$PWD_CUR/config.h \
	$$PWD_CUR/editors.h \
	$$PWD_CUR/doctrenderer.h \
	$$PWD_CUR/docbuilder.h

SOURCES += \
	$$PWD_CUR/editors.cpp \
	$$PWD_CUR/nativecontrol.cpp \
	$$PWD_CUR/doctrenderer.cpp \
	$$PWD_CUR/docbuilder.cpp \
	$$PWD_CUR/docbuilder_p.cpp \
	$$PWD_CUR/graphics.cpp \
	$$PWD_CUR/hash.cpp

SOURCES += \
	$$PWD_CUR/../../Common/OfficeFileFormatChecker2.cpp \
	$$PWD_CUR/../../Common/3dParty/pole/pole.cpp \
	$$PWD_CUR/../../OOXML/Base/unicode_util.cpp

HEADERS += \
	$$PWD_CUR/docbuilder_p.h \
	$$PWD_CUR/nativecontrol.h \
	$$PWD_CUR/graphics.h \
	$$PWD_CUR/hash.h \
	$$PWD_CUR/server.h

HEADERS += \
	$$PWD_CUR/embed/PointerEmbed.h \
	$$PWD_CUR/embed/ZipEmbed.h \
	$$PWD_CUR/embed/GraphicsEmbed.h \
	$$PWD_CUR/embed/MemoryStreamEmbed.h \
	$$PWD_CUR/embed/NativeControlEmbed.h \
	$$PWD_CUR/embed/NativeBuilderEmbed.h \
	$$PWD_CUR/embed/NativeBuilderDocumentEmbed.h \
	$$PWD_CUR/embed/TextMeasurerEmbed.h \
	$$PWD_CUR/embed/HashEmbed.h \
	$$PWD_CUR/embed/Default.h \
	$$PWD_CUR/js_internal/js_base.h

SOURCES += \
	$$PWD_CUR/embed/PointerEmbed.cpp \
	$$PWD_CUR/embed/ZipEmbed.cpp \
	$$PWD_CUR/embed/GraphicsEmbed.cpp \
	$$PWD_CUR/embed/MemoryStreamEmbed.cpp \
	$$PWD_CUR/embed/NativeControlEmbed.cpp \
	$$PWD_CUR/embed/NativeBuilderEmbed.cpp \
	$$PWD_CUR/embed/NativeBuilderDocumentEmbed.cpp \
	$$PWD_CUR/embed/TextMeasurerEmbed.cpp \
	$$PWD_CUR/embed/HashEmbed.cpp \
	$$PWD_CUR/embed/Default.cpp

# Serialize objects to JS
HEADERS += \
	$$PWD_CUR/json/json.h \
	$$PWD_CUR/json/json_p.h \
	$$PWD_CUR/json/json_values.h \
	$$PWD_CUR/json/serialization.h

SOURCES += \
	$$PWD_CUR/json/json.cpp \
	$$PWD_CUR/json/json_values.cpp

include($$PWD_CUR/js_internal/js_base.pri)

!use_javascript_core {
	build_xp:DESTDIR=$$DESTDIR/xp
}

use_javascript_core {
	OBJECTIVE_SOURCES += $$PWD_CUR/../common/Mac/NSString+StringUtils.mm
}

# files for embedded classes
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER($$PWD_CUR/embed/GraphicsEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER($$PWD_CUR/embed/HashEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER($$PWD_CUR/embed/MemoryStreamEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER($$PWD_CUR/embed/NativeBuilderEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER($$PWD_CUR/embed/NativeBuilderDocumentEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER($$PWD_CUR/embed/NativeControlEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER($$PWD_CUR/embed/PointerEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER($$PWD_CUR/embed/TextMeasurerEmbed.h)
ADD_FILES_FOR_EMBEDDED_CLASS_HEADER($$PWD_CUR/embed/ZipEmbed.h)

include($$PWD_CUR/../graphics/pro/textshaper.pri)
include($$PWD_CUR/../../Common/3dParty/openssl/openssl.pri)

# downloader
DEFINES += BUIDLER_OPEN_DOWNLOAD_ENABLED
DEFINES += BUIDLER_OPEN_BASE64_ENABLED

CONFIG += drawingfile_support
drawingfile_support {
	DEFINES += WASM_SERIALIZER_USE_ALLOCATOR
	DEFINES += DOCBUILDER_PDF_EDITOR
	ADD_DEPENDENCY(PdfFile, XpsFile, DjVuFile, OFDFile, DocxRenderer)

	HEADERS += \
		$$PWD_CUR/drawingfile.h \
		$$PWD_CUR/embed/DrawingFileEmbed.h

	SOURCES += \
		$$PWD_CUR/drawingfile.cpp \
		$$PWD_CUR/../graphics/pro/js/wasm/src/HTMLRendererText.cpp \
		$$PWD_CUR/embed/DrawingFileEmbed.cpp

	ADD_FILES_FOR_EMBEDDED_CLASS_HEADER($$PWD_CUR/embed/DrawingFileEmbed.h)
}
