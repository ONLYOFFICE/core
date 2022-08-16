QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
