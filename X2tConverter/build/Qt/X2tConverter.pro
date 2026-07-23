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

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

# before include(base.pri) !!!
ios:CONFIG += build_x2t_as_library
android:CONFIG += build_x2t_as_library

build_x2t_as_library {
	TEMPLATE = lib
	CONFIG -= console

	DEFINES += BUILD_X2T_AS_LIBRARY_DYLIB

	CONFIG += shared
	CONFIG += plugin

	!core_debug:shared:QMAKE_LFLAGS += -exported_symbols_list $$PWD/../../src/dylib/export

	build_strip_debug {
		QMAKE_LFLAGS += -Wl,--strip-debug
	}
}

include(X2tConverter.pri)

!build_x2t_as_library {
	SOURCES += ../../src/main.cpp

	DESTDIR = $$CORE_BUILDS_BINARY_PATH

	core_windows {
		!build_xp {
			CONFIG -= embed_manifest_exe
			QMAKE_MANIFEST = $$PWD/x2t.exe.manifest
			OTHER_FILES += $$PWD/x2t.exe.manifest
		}
	}
} else {
	HEADERS += ../../src/dylib/x2t.h
	SOURCES += ../../src/dylib/x2t.cpp
}
