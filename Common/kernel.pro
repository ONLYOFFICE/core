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
TARGET = kernel
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY_BUILDING

# CONFIG
HEADERS += ./kernel_config.h

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

CONFIG += build_all_zlib build_zlib_as_sources
include(../OfficeUtils/OfficeUtils.pri)

CONFIG += core_static_link_xml_full
include(../DesktopEditor/xml/build/qt/libxml2.pri)

ADD_DEPENDENCY(UnicodeConverter)

# CONFIG
HEADERS += ./kernel_config.h

# BLOCKER
HEADERS += \
	./../DesktopEditor/graphics/TemporaryCS.h

SOURCES += \
	./../DesktopEditor/graphics/TemporaryCS.cpp

# THREAD
core_android:DEFINES += NOT_USE_PTHREAD_CANCEL USE_FILE32API
HEADERS += \
	./../DesktopEditor/graphics/BaseThread.h \
	./../DesktopEditor/graphics/BaseThreadMonitor.h

SOURCES += \
	./../DesktopEditor/graphics/BaseThread.cpp \
	./../DesktopEditor/graphics/BaseThreadMonitor.cpp

# TIMER
HEADERS += \
	./../DesktopEditor/graphics/Timer.h

SOURCES += \
	./../DesktopEditor/graphics/Timer.cpp

# PATH
HEADERS += ./../DesktopEditor/common/Path.h
SOURCES += ./../DesktopEditor/common/Path.cpp

# STRINGS
HEADERS += ./../DesktopEditor/common/StringBuilder.h
SOURCES += ./../DesktopEditor/common/StringBuilder.cpp

HEADERS += ./../DesktopEditor/common/StringExt.h
SOURCES += ./../DesktopEditor/common/StringExt.cpp

HEADERS += ./../DesktopEditor/common/StringUTF32.h
SOURCES += ./../DesktopEditor/common/StringUTF32.cpp

# BYTE BUILDER
HEADERS += ./../DesktopEditor/common/ByteBuilder.h
SOURCES += ./../DesktopEditor/common/ByteBuilder.cpp

# BASE64
HEADERS += ./../DesktopEditor/common/Base64.h
SOURCES += ./../DesktopEditor/common/Base64.cpp

# FILE
HEADERS += ./../DesktopEditor/common/File.h
SOURCES += ./../DesktopEditor/common/File.cpp

HEADERS += ./../DesktopEditor/common/StreamWriter.h
SOURCES += ./../DesktopEditor/common/StreamWriter.cpp

# DIRECTORY
HEADERS += ./../DesktopEditor/common/Directory.h
SOURCES += ./../DesktopEditor/common/Directory.cpp

# SYSTEM
HEADERS += ./../DesktopEditor/common/SystemUtils.h
SOURCES += ./../DesktopEditor/common/SystemUtils.cpp

# PROCESSENV
HEADERS += ./../DesktopEditor/common/ProcessEnv.h
SOURCES += ./../DesktopEditor/common/ProcessEnv.cpp

core_windows {
	LIBS += -lRpcrt4
	LIBS += -lShell32
	!build_xp:LIBS += -lole32
}

core_ios {
	OBJECTIVE_SOURCES += ./../DesktopEditor/common/File_ios.mm
	LIBS += -framework Foundation
}
