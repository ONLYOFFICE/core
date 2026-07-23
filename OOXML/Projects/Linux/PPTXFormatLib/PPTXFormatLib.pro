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

TARGET = PPTXFormatLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t
include(../../../../Common/base.pri)

#BOOST
include($$PWD/../../../../Common/3dParty/boost/boost.pri)

#LOGIC
include($$PWD/../../../PPTXFormat/Logic/pri/pptx_logic.pri)

#LIMITS
include($$PWD/../../../PPTXFormat/Limit/pri/pptx_limits.pri)

#PPTXSHAPES
include($$PWD/../../../../MsBinaryFile/Common/Vml/PPTXShape/OOXMLShapes/pri/pptx_shapes.pri)

DEFINES += UNICODE \
    _UNICODE \
    AVS_USE_CONVERT_PPTX_TOCUSTOM_VML \
    #DISABLE_FILE_DOWNLOADER \
    DONT_WRITE_EMBEDDED_FONTS

INCLUDEPATH += \
	../../../../Common/ASCDocxFormat/Source/Utility \
	../../../../Common/ASCDocxFormat/Source/XML \
	../../../../Common/ASCDocxFormat/Source \
	../../../../../MsBinaryFile/XlsFile/Format \
	../../../../../MsBinaryFile/Common/common_xls \
	../../../XlsbFormat

#precompile_header {
#    PRECOMPILED_HEADER = precompiled.h
#    HEADERS += precompiled.h
#}

SOURCES += \
	pptx_format.cpp \
	../../../../OOXML/Binary/Document/BinWriter/BinEquationWriter.cpp \
	\
	../../../../MsBinaryFile/Common/Vml/BaseShape.cpp \
	../../../../MsBinaryFile/Common/Vml/GraphicsPath.cpp \
        ../../../../MsBinaryFile/Common/Vml/VmlPath.cpp \
	../../../../MsBinaryFile/Common/Vml/toVmlConvert.cpp \
	\
	../../../../MsBinaryFile/Common/Vml/PPTShape/PptShape.cpp \
	../../../../MsBinaryFile/Common/Vml/PPTShape/PptFormula.cpp \
	../../../../MsBinaryFile/Common/Vml/PPTShape/ElementSettings.cpp \
	../../../../MsBinaryFile/Common/Vml/PPTShape/BinaryReader.cpp \
	../../../../MsBinaryFile/Common/Vml/PPTShape/CustomGeomShape.cpp \
	../../../../MsBinaryFile/Common/Vml/PPTShape/Ppt2PptxShapeConverter.cpp \
	\
	../../../../MsBinaryFile/Common/Vml/PPTXShape/PptxFormula.cpp \
	../../../../MsBinaryFile/Common/Vml/PPTXShape/PptxShape.cpp \
	../../../../MsBinaryFile/Common/Vml/PPTXShape/Pptx2PptShapeConverter.cpp \
	\
	../../../../MsBinaryFile/PptFile/Drawing/TextAttributesEx.cpp \
	../../../../MsBinaryFile/PptFile/Drawing/Elements.cpp \
	\
	../../../Binary/Presentation/BinaryFileReaderWriter.cpp \
	../../../Binary/Presentation/XmlWriter.cpp \
	../../../Binary/Presentation/FontPicker.cpp \
	../../../Binary/Presentation/imagemanager.cpp \	
	../../../Binary/Presentation/PPTXWriter.cpp \
	../../../Binary/Presentation/DefaultNotesTheme.cpp \
	../../../Binary/Presentation/DefaultNotesMaster.cpp \
	../../../Binary/Presentation/Converter.cpp \
	../../../Binary/Presentation/CalculatorCRC32.cpp \
	../../../Binary/Presentation/FontCutter.cpp	\
	\
	../../../PPTXFormat/DrawingConverter/ASCOfficeDrawingConverter.cpp \
	../../../PPTXFormat/DrawingConverter/ASCOfficePPTXFileRealization.cpp \
	\
	../../../PPTXFormat/ViewProps/CSldViewPr.cpp \
	../../../PPTXFormat/ViewProps/CViewPr.cpp \
	../../../PPTXFormat/ViewProps/GridSpacing.cpp \
	../../../PPTXFormat/ViewProps/Guide.cpp \
	../../../PPTXFormat/ViewProps/NormalViewPr.cpp \
	../../../PPTXFormat/ViewProps/NotesTextViewPr.cpp \
	../../../PPTXFormat/ViewProps/NotesViewPr.cpp \
	../../../PPTXFormat/ViewProps/Origin.cpp \
	../../../PPTXFormat/ViewProps/OutlineViewPr.cpp \
	../../../PPTXFormat/ViewProps/Ratio.cpp \
	../../../PPTXFormat/ViewProps/Restored.cpp \
	../../../PPTXFormat/ViewProps/Scale.cpp \
	../../../PPTXFormat/ViewProps/SlideViewPr.cpp \
	../../../PPTXFormat/ViewProps/SorterViewPr.cpp \
	\
	../../../PPTXFormat/Theme/ClrScheme.cpp \
	../../../PPTXFormat/Theme/ExtraClrScheme.cpp \
	../../../PPTXFormat/Theme/FmtScheme.cpp \
	../../../PPTXFormat/Theme/FontScheme.cpp \
	../../../PPTXFormat/Theme/ThemeElements.cpp \
        ../../../PPTXFormat/Theme/VisioThemeElements.cpp \
        \
        ../../../PPTXFormat/ShowPr/PresentationPr.cpp \
	../../../PPTXFormat/ShowPr/Present.cpp \
	../../../PPTXFormat/ShowPr/Kiosk.cpp \
        ../../../PPTXFormat/ShowPr/CustomShowId.cpp \
	../../../PPTXFormat/ShowPr/Browse.cpp \
	../../../PPTXFormat/ShowPr/SldRg.cpp \
	../../../PPTXFormat/ShowPr/SldAll.cpp \
	\
        ../../../PPTXFormat/Presentation/PresentationChildElements.cpp

HEADERS += \
	pptx_format.h \
	\
	../../../Base/Nullable.h \
	../../../../Common/FileDownloader/FileDownloader.h \
	\
	../../../../MsBinaryFile/Common/Vml/Common.h \
	../../../../MsBinaryFile/Common/Vml/BaseShape.h \
	../../../../MsBinaryFile/Common/Vml/toVmlConvert.h \
	../../../../MsBinaryFile/Common/Vml/VmlPath.h \
	../../../../MsBinaryFile/Common/Vml/GraphicsPath.h \	
	\
	../../../../MsBinaryFile/Common/Vml/PPTShape/PptShape.h \
	../../../../MsBinaryFile/Common/Vml/PPTShape/PptFormula.h \
	../../../../MsBinaryFile/Common/Vml/PPTShape/ElementSettings.h \
	../../../../MsBinaryFile/Common/Vml/PPTShape/BinaryReader.h \
	../../../../MsBinaryFile/Common/Vml/PPTShape/CustomGeomShape.h \
	../../../../MsBinaryFile/Common/Vml/PPTShape/Ppt2PptxShapeConverter.h \
	../../../../MsBinaryFile/Common/Vml/PPTShape/Enums.h \
	../../../../MsBinaryFile/Common/Vml/PPTShape/PptShapeEnum.h \
	../../../../MsBinaryFile/DocFile/OfficeDrawing/PresetShapeTypes.h \
	\
	../../../../MsBinaryFile/Common/Vml/PPTXShape/PptxFormula.h \
	../../../../MsBinaryFile/Common/Vml/PPTXShape/PptxShape.h \
	../../../../MsBinaryFile/Common/Vml/PPTXShape/PresetShapesHeader.h \
	../../../../MsBinaryFile/Common/Vml/PPTXShape/Pptx2PptShapeConverter.h \
	\
	../../../PPTXFormat/FileTypes.h \
	../../../PPTXFormat/Namespaces.h \
	../../../PPTXFormat/DrawingConverter/ASCOfficeDrawingConverter.h \
	../../../PPTXFormat/DrawingConverter/ASCOfficePPTXFile.h \	
	\
        ../../../PPTXFormat/Presentation/PresentationChildElements.h \
	\
	../../../PPTXFormat/Theme/ClrScheme.h \
	../../../PPTXFormat/Theme/ExtraClrScheme.h \
	../../../PPTXFormat/Theme/FmtScheme.h \
	../../../PPTXFormat/Theme/FontScheme.h \
	../../../PPTXFormat/Theme/ThemeElements.h \
        ../../../PPTXFormat/Theme/VisioThemeElements.h \
	\
	../../../PPTXFormat/ViewProps/CSldViewPr.h \
	../../../PPTXFormat/ViewProps/CViewPr.h \
	../../../PPTXFormat/ViewProps/GridSpacing.h \
	../../../PPTXFormat/ViewProps/Guide.h \
	../../../PPTXFormat/ViewProps/NormalViewPr.h \
	../../../PPTXFormat/ViewProps/NotesTextViewPr.h \
	../../../PPTXFormat/ViewProps/NotesViewPr.h \
	../../../PPTXFormat/ViewProps/Origin.h \
	../../../PPTXFormat/ViewProps/OutlineViewPr.h \
	../../../PPTXFormat/ViewProps/Ratio.h \
	../../../PPTXFormat/ViewProps/Restored.h \
	../../../PPTXFormat/ViewProps/Scale.h \
	../../../PPTXFormat/ViewProps/SlideViewPr.h \
	../../../PPTXFormat/ViewProps/SorterViewPr.h \
	\
	../../../Binary/Presentation/BinaryFileReaderWriter.h \
	../../../Binary/Presentation/BinReaderWriterDefines.h \
	../../../Binary/Presentation/BinWriters.h \
	../../../Binary/Presentation/CalculatorCRC32.h \
	../../../Binary/Presentation/Converter.h \
	../../../Binary/Presentation/FontCutter.h \
	../../../Binary/Presentation/FontPicker.h \
	../../../Binary/Presentation/imagemanager.h \
	../../../Binary/Presentation/PPTXWriter.h \
	../../../Binary/Presentation/WMFToImageConverter.h \
	../../../Binary/Presentation/DefaultNotesMaster.h \
	../../../Binary/Presentation/DefaultNotesTheme.h \
	\
	../../../PPTXFormat/ShowPr/Browse.h \
        ../../../PPTXFormat/ShowPr/CustomShowId.h \
	../../../PPTXFormat/ShowPr/Kiosk.h \
	../../../PPTXFormat/ShowPr/Present.h \
        ../../../PPTXFormat/ShowPr/PresentationPr.h \
	../../../PPTXFormat/ShowPr/SldAll.h \
	../../../PPTXFormat/ShowPr/SldRg.h
