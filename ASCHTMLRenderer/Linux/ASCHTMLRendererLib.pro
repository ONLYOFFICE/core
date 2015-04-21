#-------------------------------------------------
#
# Project created by QtCreator 2014-10-10T14:24:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = ASCHTMLRendererLib
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wno-ignored-qualifiers

DEFINES +=  UNICODE \
            _UNICODE \
            _LINUX_QT \
            NODOCX \
            DESKTOP_EDITOR_GRAPHICS

INCLUDEPATH += \
    ../../DesktopEditor/freetype-2.5.2/include \
    ../../DesktopEditor/agg-2.4/include
	
SOURCES += ../ASCSVGWriter.cpp \
    ../CASCSVGRenderer.cpp \
    ../CMetafile.cpp \
    ../CASCImage.cpp \
    ../../DesktopEditor/raster/Metafile/Common.cpp \
    ../../DesktopEditor/raster/Metafile/MetaFile.cpp \
    ../../DesktopEditor/raster/Metafile/Emf/EmfObjects.cpp \
    ../../DesktopEditor/raster/Metafile/Emf/EmfPath.cpp \
    ../../DesktopEditor/raster/Metafile/Emf/EmfPlayer.cpp \
    ../../DesktopEditor/raster/Metafile/Emf/EmfClip.cpp

HEADERS += ../ASCSVGWriter.h \
    ../CASCImage.h \
    ../CASCSVGRenderer.h \
    ../CMetafile.h \
    ../Writer/Common.h \
    ../Writer/Const.h \
    ../IBaseMatrixUpdater.h \
    ../Writer/TextItem.h \
    ../Writer/StringWriter.h \
    ../Writer/SVGWriter.h \
    ../Writer/VectorGraphicsWriter2.h \
    ../Writer/VMLWriter.h \
    ../../DesktopEditor/raster/Metafile/Common.h \
    ../../DesktopEditor/raster/Metafile/MetaFile.h \
    ../../DesktopEditor/raster/Metafile/Emf/EmfFile.h \
    ../../DesktopEditor/raster/Metafile/Emf/EmfObjects.h \
    ../../DesktopEditor/raster/Metafile/Emf/EmfOutputDevice.h \
    ../../DesktopEditor/raster/Metafile/Emf/EmfPath.h \
    ../../DesktopEditor/raster/Metafile/Emf/EmfPlayer.h \
    ../../DesktopEditor/raster/Metafile/Emf/EmfTypes.h \
    ../../DesktopEditor/raster/Metafile/Emf/RendererOutput.h \
    ../../DesktopEditor/raster/Metafile/Wmf/RendererOutput.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfCharsets.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfCodePage932.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfCodePage936.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfCodePage949.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfCodePage950.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfCodePage1361.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfColor.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfFile.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfMemory.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfOutputDevice.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfRegion.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfTypes.h \
    ../../DesktopEditor/raster/Metafile/Wmf/WmfUtils.h \
    ../../DesktopEditor/raster/Metafile/Emf/EmfClip.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

