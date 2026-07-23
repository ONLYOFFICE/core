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

DEFINES += HAVE_VA_COPY

core_static_link_xml_full {
DEFINES += \
    LIBXML_READER_ENABLED \
    LIBXML_PUSH_ENABLED \
    LIBXML_HTML_ENABLED \
    LIBXML_XPATH_ENABLED \
    LIBXML_OUTPUT_ENABLED \
    LIBXML_C14N_ENABLED \
    LIBXML_SAX1_ENABLED \
    LIBXML_TREE_ENABLED \
    LIBXML_XPTR_ENABLED \
    IN_LIBXML \
    LIBXML_STATIC
}

INCLUDEPATH += \
    $$PWD/../../libxml2/include \
    $$PWD/../../libxml2/include/libxml \
    $$PWD

core_release {
    SOURCES += $$PWD/libxml2_all.c
    SOURCES += $$PWD/libxml2_all2.c

    DEFINES += XML_ERROR_DISABLE_MODE
}

core_debug {
SOURCES += \
    $$PWD/../../libxml2/buf.c \
    $$PWD/../../libxml2/c14n.c \
    $$PWD/../../libxml2/catalog.c \
    $$PWD/../../libxml2/chvalid.c \
    $$PWD/../../libxml2/debugXML.c \
    $$PWD/../../libxml2/dict.c \
    $$PWD/../../libxml2/DOCBparser.c \
    $$PWD/../../libxml2/encoding.c \
    $$PWD/../../libxml2/entities.c \
    $$PWD/../../libxml2/error.c \
    $$PWD/../../libxml2/globals.c \
    $$PWD/../../libxml2/hash.c \
    $$PWD/../../libxml2/HTMLparser.c \
    $$PWD/../../libxml2/HTMLtree.c \
    $$PWD/../../libxml2/legacy.c \
    $$PWD/../../libxml2/list.c \
    $$PWD/../../libxml2/nanoftp.c \
    $$PWD/../../libxml2/nanohttp.c \
    $$PWD/../../libxml2/parserInternals.c \
    $$PWD/../../libxml2/pattern.c \
    $$PWD/../../libxml2/relaxng.c \
    $$PWD/../../libxml2/SAX.c \
    $$PWD/../../libxml2/SAX2.c \
    $$PWD/../../libxml2/schematron.c \
    $$PWD/../../libxml2/threads.c \
    $$PWD/../../libxml2/tree.c \
    $$PWD/../../libxml2/uri.c \
    $$PWD/../../libxml2/xinclude.c \
    $$PWD/../../libxml2/xlink.c \
    $$PWD/../../libxml2/xmlIO.c \
    $$PWD/../../libxml2/xmlmemory.c \
    $$PWD/../../libxml2/xmlmodule.c \
    $$PWD/../../libxml2/xmlreader.c \
    $$PWD/../../libxml2/xmlregexp.c \
    $$PWD/../../libxml2/xmlsave.c \
    $$PWD/../../libxml2/xmlschemas.c \
    $$PWD/../../libxml2/xmlschemastypes.c \
    $$PWD/../../libxml2/xmlstring.c \
    $$PWD/../../libxml2/xmlunicode.c \
    $$PWD/../../libxml2/xmlwriter.c \
    $$PWD/../../libxml2/xpath.c \
    $$PWD/../../libxml2/xpointer.c \
    $$PWD/../../libxml2/valid.c \
     $$PWD/../../libxml2/parser.c
}

!core_only_libxml {
SOURCES +=  \
    $$PWD/../../src/xmlwriter.cpp \
    $$PWD/../../src/xmllight.cpp \
    $$PWD/../../src/xmldom.cpp

HEADERS += \
    $$PWD/../../src/xmllight_private.h

HEADERS += \
    $$PWD/../../include/xmlutils.h \
    $$PWD/../../include/xmlwriter.h
}
