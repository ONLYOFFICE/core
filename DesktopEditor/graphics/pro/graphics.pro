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

QT -= core gui

TARGET = graphics
TEMPLATE = lib

CONFIG += graphics_dynamic_library

DEFINES += _QT
graphics_dynamic_library {
	CONFIG += shared
	CONFIG += plugin

	DEFINES += GRAPHICS_USE_DYNAMIC_LIBRARY_BUILDING
} else {
	DEFINES += GRAPHICS_NO_USE_DYNAMIC_LIBRARY
	CONFIG += static
}

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD
include(../../../Common/base.pri)

ADD_DEPENDENCY(UnicodeConverter, kernel)

core_windows {
	LIBS += -lAdvapi32
	LIBS += -lShell32
}

HEADERS += ./../config.h

GRAPHICS_AGG_PATH = $$PWD/../../agg-2.4

INCLUDEPATH += \
	$$GRAPHICS_AGG_PATH/include

# matrix
HEADERS += \
	$$GRAPHICS_AGG_PATH/include/test_grads/custom_gradients.h \
	./../Matrix_private.h \
	./../Matrix.h

SOURCES += \
	./../Matrix.cpp

SOURCES += \
	$$GRAPHICS_AGG_PATH/src/agg_trans_affine.cpp

# paths
HEADERS += \
	./../GraphicsPath_private.h \
	./../GraphicsPath.h \
	./../BooleanOperations.h \
	./../boolean_operations_math.h

SOURCES += \
	./../GraphicsPath.cpp \
	./../BooleanOperations.cpp

# alpha mask
HEADERS += \
	./../AlphaMask.h \
	./../AlphaMask_p.h

SOURCES += \
	./../AlphaMask.cpp

# grapgics layer
HEADERS += \
	./../GraphicsLayer.h \
	./../GraphicsLayerBlend.h

SOURCES += \
	./../GraphicsLayer.cpp

SOURCES += \
	$$GRAPHICS_AGG_PATH/src/agg_arc.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_bezier_arc.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_curves.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_bspline.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_bspline.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_stroke.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_contour.cpp

include(raster.pri)

#CONFIG += graphics_disable_metafile
graphics_disable_metafile {
	DEFINES += GRAPHICS_DISABLE_METAFILE
} else {
	include(metafile.pri)
}

CONFIG += support_font_converter
include(fontengine.pri)

# drawing file
HEADERS += ./officedrawingfile.h
SOURCES += ./officedrawingfile.cpp

# graphics
SOURCES += \
	$$GRAPHICS_AGG_PATH/src/agg_arrowhead.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_image_filters.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_line_aa_basics.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_line_profile_aa.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_dash.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_markers_term.cpp \
	$$GRAPHICS_AGG_PATH/src/agg_vcgen_smooth_poly1.cpp

HEADERS += \
	./../ArrowHead.h \
	./../Brush.h \
	./../Clip.h \
	./../Color.h \
	./../Defines.h \
	./../Graphics.h \
	./../ImageFilesCache.h \
	./../structures.h \
	./../shading_info.h \
	./../Graphics.h \
	./../GraphicsRenderer.h \
	\
	./Graphics.h \
	./MemoryStorage.h \
	./Image.h

SOURCES += \
	./../ArrowHead.cpp \
	./../Brush.cpp \
	./../Clip.cpp \
	./../Graphics.cpp \
	./../GraphicsRenderer.cpp \
	\
	./pro_Image.cpp \
	./pro_Graphics.cpp

HEADERS += \
	./../MetafileToRenderer.h \
	./../MetafileToRendererCheck.h \
	./../MetafileToRendererReader.h \
	./../MetafileToGraphicsRenderer.h \
	\
	./../commands/FormField.h \
	./../commands/AnnotField.h \
	./../commands/DocInfo.h

SOURCES += \
	./../MetafileToRenderer.cpp \
	./../MetafileToRendererReader.cpp \
	./../MetafileToGraphicsRenderer.cpp \
	\
	./../commands/FormField.cpp \
	./../commands/AnnotField.cpp \
	./../commands/DocInfo.cpp
