QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CEditItemWidget.cpp \
    CItemWidget.cpp \
    CMapModel.cpp \
    CMetafileTreeView.cpp \
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
    Common/CFrameRgn.cpp \
    Common/CGradientFillWidget.cpp \
    Common/CInvertRgn.cpp \
    Common/CLogBrushExWidget.cpp \
    Common/CLogColorSpaceWidget.cpp \
    Common/CLogFontExDv.cpp \
    Common/CLogPaletteWidget.cpp \
    Common/CLogPenWidget.cpp \
    Common/CPaletteEntriesWidget.cpp \
    Common/CPixelFormatDescriptorWidget.cpp \
    Common/CPointWidget.cpp \
    Common/CPointsWidget.cpp \
    Common/CPolyDrawWidget.cpp \
    Common/CPolyTextOut.cpp \
    Common/CPolyWidget.cpp \
    Common/CRectangleWidget.cpp \
    Common/CSizeWidget.cpp \
    Common/CSmallTextOutWidget.cpp \
    MainWindow.cpp \
    CCustomView.cpp \
    main.cpp

HEADERS += \
    CEditItemWidget.h \
    CItemWidget.h \
    CMapModel.h \
    CMetafileTreeView.h \
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
    Common/CFrameRgn.h \
    Common/CGradientFillWidget.h \
    Common/CInvertRgn.h \
    Common/CLogBrushExWidget.h \
    Common/CLogColorSpaceWidget.h \
    Common/CLogFontExDv.h \
    Common/CLogPaletteWidget.h \
    Common/CLogPenWidget.h \
    Common/CPaletteEntriesWidget.h \
    Common/CPixelFormatDescriptorWidget.h \
    Common/CPointWidget.h \
    Common/CPointsWidget.h \
    Common/CPolyDrawWidget.h \
    Common/CPolyTextOut.h \
    Common/CPolyWidget.h \
    Common/CRectangleWidget.h \
    Common/CSharedWidget.h \
    Common/CSizeWidget.h \
    Common/CSmallTextOutWidget.h \
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

SOURCES += ../../../../fontengine/ApplicationFontsWorker.cpp

DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX
