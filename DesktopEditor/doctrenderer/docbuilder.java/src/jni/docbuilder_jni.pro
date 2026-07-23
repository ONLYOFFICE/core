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

QT		-= core
QT		-= gui

TARGET	 = docbuilder.jni

TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(graphics, kernel, kernel_network, UnicodeConverter, doctrenderer)

INCLUDEPATH += ../../..

# get path to JDK from environment variable JAVA_HOME (preferable way) or take the default one
JDK_PATH = $$(JAVA_HOME)
isEmpty(JDK_PATH) {
	core_windows:JDK_PATH = "C:/Program Files/Java/jdk1.8.0_202"
	core_linux:JDK_PATH = "/usr/lib/jvm/java-8-openjdk-amd64"
	core_mac:JDK_PATH = "/Library/Java/JavaVirtualMachines/jdk-1.8.jdk/Contents/Home"
}

INCLUDEPATH += $$JDK_PATH/include

core_windows:JAVA_ARCH = win32
core_linux:JAVA_ARCH = linux
core_mac:JAVA_ARCH = darwin

INCLUDEPATH += $$JDK_PATH/include/$$JAVA_ARCH

SOURCES += \
	docbuilder_CDocBuilderValue.cpp \
	docbuilder_CDocBuilder.cpp \
	docbuilder_CDocBuilderContextScope.cpp \
	docbuilder_CDocBuilderContext.cpp

HEADERS += \
	docbuilder_CDocBuilderValue.h \
	docbuilder_CDocBuilder.h \
	docbuilder_CDocBuilderContextScope.h \
	docbuilder_CDocBuilderContext.h
