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

QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = HtmlFile2
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

DEFINES += HTMLFILE2_USE_DYNAMIC_LIBRARY
DEFINES += CSSCALCULATOR_LIBRARY_STATIC
DEFINES += CSS_CALCULATOR_WITH_XHTML
DEFINES += DISABLE_RUBY_SUPPORT
DEFINES += MDCONVERTER_DECL_EXPORT
DEFINES += HTML_NORMALIZER_DECL_EXPORT

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

# equal .c names
CONFIG += object_parallel_to_source
core_windows:CONFIG += no_batch

include($$CORE_ROOT_DIR/Common/3dParty/html/gumbo.pri)
include($$CORE_ROOT_DIR/Common/3dParty/html/css/CssCalculator.pri)
include($$CORE_ROOT_DIR/Common/3dParty/md/md2html.pri)

CONFIG += core_boost_regex
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter, graphics, kernel_network)

SOURCES += htmlfile2.cpp \
    ./Writers/OOXMLWriter.cpp \
    HTMLReader.cpp \
    Tags/MDTags.cpp \
    Writers/MDWriter.cpp \
    Table.cpp \
    Tags/OOXMLTags.cpp

HEADERS += htmlfile2.h \
	./src/StringFinder.h \
	./src/Languages.h \
	Common.h \
	./Writers/OOXMLWriter.h \
	./Writers/IWriter.h \
	HTMLParameters.h \
	HTMLReader.h \
	./Tags/HTMLTags.h \
	MarkdownParameters.h \
	Tags/MDTags.h \
	Writers/MDWriter.h \
	Table.h \
	Tags/OOXMLTags.h
