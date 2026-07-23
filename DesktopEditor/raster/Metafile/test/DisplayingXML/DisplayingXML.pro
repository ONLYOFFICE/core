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

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += METAFILE_SUPPORT_WMF_EMF

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CEditItemWidget.cpp \
    CItemWidget.cpp \
    CMapModel.cpp \
    CMetafileTreeView.cpp \
    CMetafileTreeWidget.cpp \
    CRecordCreator.cpp \
    CStatisticsWidget.cpp \
    CTextEditDelegate.cpp \
    CXMLHighlighter.cpp \
    Common/CCIEXYZTripleWidget.cpp \
    Common/CColorWidget.cpp \
    Common/CEmptyWidget.cpp \
    Common/CExtSelectClipRgnWidget.cpp \
    Common/CExtTextOutWidget.cpp \
    Common/CFillRgnWidget.cpp \
    Common/CFormWidget.cpp \
    Common/CFrameRgnWidget.cpp \
    Common/CGradientFillWidget.cpp \
    Common/CInvertRgnWidget.cpp \
    Common/CLogBrushExWidget.cpp \
    Common/CLogColorSpaceWidget.cpp \
    Common/CLogFontExDvWidget.cpp \
    Common/CLogPaletteWidget.cpp \
    Common/CLogPenWidget.cpp \
    Common/CPaintRgnWidget.cpp \
    Common/CPaletteEntriesWidget.cpp \
    Common/CPixelFormatDescriptorWidget.cpp \
    Common/CPointWidget.cpp \
    Common/CPointsWidget.cpp \
    Common/CPolyDrawWidget.cpp \
    Common/CPolyTextOutWidget.cpp \
    Common/CPolyWidget.cpp \
    Common/CRectangleWidget.cpp \
    Common/CSizeWidget.cpp \
    Common/CSmallTextOutWidget.cpp \
    Common/CXFormWidget.cpp \
    MainWindow.cpp \
    CCustomView.cpp \
    main.cpp

HEADERS += \
    CEditItemWidget.h \
    CItemWidget.h \
    CMapModel.h \
    CMetafileTreeView.h \
    CMetafileTreeWidget.h \
    CRecordCreator.h \
    CStatisticsWidget.h \
    CTextEditDelegate.h \
    CXMLHighlighter.h \
    Common/CCIEXYZTripleWidget.h \
    Common/CColorWidget.h \
    Common/CEmptyWidget.h \
    Common/CExtSelectClipRgnWidget.h \
    Common/CExtTextOutWidget.h \
    Common/CFillRgnWidget.h \
    Common/CFormWidget.h \
    Common/CFrameRgnWidget.h \
    Common/CGradientFillWidget.h \
    Common/CInvertRgnWidget.h \
    Common/CLogBrushExWidget.h \
    Common/CLogColorSpaceWidget.h \
    Common/CLogFontExDvWidget.h \
    Common/CLogPaletteWidget.h \
    Common/CLogPenWidget.h \
    Common/CPaintRgnWidget.h \
    Common/CPaletteEntriesWidget.h \
    Common/CPixelFormatDescriptorWidget.h \
    Common/CPointWidget.h \
    Common/CPointsWidget.h \
    Common/CPolyDrawWidget.h \
    Common/CPolyTextOutWidget.h \
    Common/CPolyWidget.h \
    Common/CRectangleWidget.h \
    Common/CSharedWidget.h \
    Common/CSizeWidget.h \
    Common/CSmallTextOutWidget.h \
    Common/CXFormWidget.h \
    Common/Common.h \
    MainWindow.h \
    CCustomView.h

FORMS += \
    Ui/EditItem.ui \
    Ui/MainWindow.ui \
    Ui/RecordCreator.ui \
    Ui/Statistics.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CORE_ROOT_DIR = $$PWD/../../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)

DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX
