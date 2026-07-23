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

TARGET = XlsFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../../Common/base.pri)

#BOOST
include($$PWD/../../../../Common/3dParty/boost/boost.pri)

#LOGIC
include($$PWD/../../../XlsFile/Format/Logic/pri/xls_logic.pri)

DEFINES +=  UNICODE \
			_UNICODE \
			DONT_WRITE_EMBEDDED_FONTS

INCLUDEPATH += ../../../XlsFile/Format
INCLUDEPATH += ../../../Common
INCLUDEPATH += ../../../../OOXML/XlsbFormat

precompile_header {
    PRECOMPILED_HEADER = precompiled.h
    HEADERS += precompiled.h
}

SOURCES +=  \
	../../../XlsFile/Format/Auxiliary/HelpFunc.cpp \
	../../../XlsFile/Format/Binary/CFRecord.cpp \
	../../../XlsFile/Format/Binary/CFRecordType.cpp \
	../../../XlsFile/Format/Binary/CFStream.cpp \
	../../../XlsFile/Format/Binary/CFStreamCacheReader.cpp \
	../../../XlsFile/Format/Binary/CFStreamCacheWriter.cpp \
	../../../XlsFile/Format/Binary/CompoundFile.cpp \
	\
	../../../XlsFile/Format/Crypt/rtl/cipher.cpp \
	../../../XlsFile/Format/Crypt/rtl/digest.cpp \
	../../../XlsFile/Format/Crypt/BiffDecoder_RCF.cpp \
	../../../XlsFile/Format/Crypt/BinaryCodec_RCF.cpp \
	../../../XlsFile/Format/Crypt/Decryptor.cpp \
	../../../XlsFile/Format/Crypt/RC4Crypt.cpp \
	../../../XlsFile/Format/Crypt/XORCrypt.cpp \
	\
	../../../XlsFile/Format/Logging/Log.cpp \
	../../../XlsFile/Format/Logging/Logger.cpp \
	\
	../../../XlsFile/Converter/ConvertXls2Xlsx.cpp \
	../../../XlsFile/Converter/external_items.cpp \
	../../../XlsFile/Converter/mediaitems_utils.cpp \
	../../../XlsFile/Converter/namespaces.cpp \
	../../../XlsFile/Converter/oox_content_type.cpp \
	../../../XlsFile/Converter/oox_package.cpp \
	../../../XlsFile/Converter/oox_rels.cpp \
	../../../XlsFile/Converter/XlsConverter.cpp \
	../../../XlsFile/Converter/xlsx_conversion_context.cpp \
	../../../XlsFile/Converter/xlsx_drawing_context.cpp \
	../../../XlsFile/Converter/xlsx_drawings.cpp \
	../../../XlsFile/Converter/xlsx_hyperlinks.cpp \
	../../../XlsFile/Converter/xlsx_output_xml.cpp \
	../../../XlsFile/Converter/xlsx_package.cpp \
	../../../XlsFile/Converter/xlsx_protection.cpp \
	../../../XlsFile/Converter/xlsx_tablecontext.cpp \
	../../../XlsFile/Converter/xlsx_textcontext.cpp \
	../../../XlsFile/Converter/xlsx_chart_context.cpp \
	../../../XlsFile/Converter/xlsx_pivots_context.cpp \
	../../../XlsFile/Converter/xlsx_sheet_context.cpp \
	../../../XlsFile/Converter/xlsx_external_context.cpp \
	../../../XlsFile/Converter/xlsx_activeX_context.cpp \
	../../../XlsFile/Converter/xlsx_comments.cpp \
	../../../XlsFile/Converter/xlsx_comments_context.cpp \
	../../../XlsFile/Converter/xls_writer.cpp 

HEADERS +=  \
	../../../XlsFile/Format/Binary/BinSmartPointers.h \
	../../../XlsFile/Format/Binary/CFRecord.h \
	../../../XlsFile/Format/Binary/CFRecordType.h \
	../../../XlsFile/Format/Binary/CFStream.h \
	../../../XlsFile/Format/Binary/CFStreamCacheReader.h \
	../../../XlsFile/Format/Binary/CFStreamCacheWriter.h \
	../../../XlsFile/Format/Binary/CompoundFile.h \
	\
	../../../XlsFile/Format/Crypt/rtl/cipher.h \
	../../../XlsFile/Format/Crypt/rtl/digest.h \
	../../../XlsFile/Format/Crypt/BiffDecoder_RCF.h \
	../../../XlsFile/Format/Crypt/BinaryCodec_RCF.h \
	../../../XlsFile/Format/Crypt/Crypt.h \
	../../../XlsFile/Format/Crypt/Decryptor.h \
	../../../XlsFile/Format/Crypt/RC4Crypt.h \
	../../../XlsFile/Format/Crypt/XORCrypt.h \
	\
	../../../XlsFile/Format/Logging/Log.h \
	../../../XlsFile/Format/Logging/Logger.h \	
	\
	../../../Common/Utils/OptPtr.h \
	../../../Common/Utils/simple_xml_writer.h \
	\
	../../../XlsFile/Converter/ConvertXls2Xlsx.h \
	../../../XlsFile/Converter/external_items.h \
	../../../XlsFile/Converter/mediaitems_utils.h \
	../../../XlsFile/Converter/namespaces.h \
	../../../XlsFile/Converter/oox_content_type.h \
	../../../XlsFile/Converter/oox_package.h \
	../../../XlsFile/Converter/oox_rels.h \
	../../../XlsFile/Converter/progressCallback.h \
	../../../XlsFile/Converter/XlsConverter.h \
	../../../XlsFile/Converter/xlsx_conversion_context.h \
	../../../XlsFile/Converter/xlsx_drawing_context.h \
	../../../XlsFile/Converter/xlsx_drawings.h \
	../../../XlsFile/Converter/xlsx_sheet_context.cpp \
	../../../XlsFile/Converter/xlsx_pivots_context.cpp \
	../../../XlsFile/Converter/xlsx_hyperlinks.h \
	../../../XlsFile/Converter/xlsx_output_xml.h \
	../../../XlsFile/Converter/xlsx_package.h \
	../../../XlsFile/Converter/xlsx_protection.h \
	../../../XlsFile/Converter/xlsx_tablecontext.h \
	../../../XlsFile/Converter/xlsx_textcontext.h \
	../../../XlsFile/Converter/xlsx_external_context.h \
	../../../XlsFile/Converter/xlsx_activeX_context.h \
	\
	../../../XlsFile/Converter/ShapeType.h \
	../../../XlsFile/Converter/xlsx_chart_context.h \
	../../../XlsFile/Format/Auxiliary/HelpFunc.h \
	../../../XlsFile/Converter/xls_writer.h 

