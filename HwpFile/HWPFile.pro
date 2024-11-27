QT       -= core gui

VERSION = 0.0.0.1
TARGET = HWPFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD

include($$CORE_ROOT_DIR/Common/base.pri)

ADD_DEPENDENCY(kernel)

SOURCES += \
    HwpDoc/HWPDocInfo.cpp \
    HwpDoc/HWPElements/HWPRecord.cpp \
    HwpDoc/HWPElements/HWPRecordBinData.cpp \
    HwpDoc/HWPElements/HWPRecordBorderFill.cpp \
    HwpDoc/HWPFile.cpp \
    HwpDoc/HWPStream.cpp \
    HwpDoc/HwpFileHeader.cpp

HEADERS += \
    HwpDoc/Errors.h \
    HwpDoc/HWPDocInfo.h \
    HwpDoc/HWPElements/HWPRecord.h \
    HwpDoc/HWPElements/HWPRecordBinData.h \
    HwpDoc/HWPElements/HWPRecordBorderFill.h \
    HwpDoc/HWPElements/HWPTag.h \
    HwpDoc/HWPElements/HwpRecordTypes.h \
    HwpDoc/HWPFile.h \
    HwpDoc/HWPStream.h \
    HwpDoc/HanType.h \
    HwpDoc/HwpFileHeader.h

