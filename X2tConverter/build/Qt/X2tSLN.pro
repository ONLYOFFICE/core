TEMPLATE = subdirs

CONFIG -= debug_and_release debug_and_release_target

# важно!!! build directory должна быть на уровне выше, чтобы Makefile и $$PWD были на одном уровне к ServerComponents

SUBDIRS = \
    OfficeUtils \
    graphics \
    DocxFormatLib \
    PPTXFormatLib \
    ASCOfficeDocxFile2Lib \
    TxtXmlFormatLib \
    RtfFormatLib \
    PptFormatLib \
    DocFormatLib \
    OdfFileReaderLib \
    OdfFileWriterLib \
    XlsFormatLib \
    X2tConverter

OfficeUtils.file = ../../../OfficeUtils/OfficeUtils.pro
graphics.file = ../../../DesktopEditor/Qt_build/graphics/project/graphics.pro

DocxFormatLib.file = ../../../Common/DocxFormat/DocxFormatLib/DocxFormatLib.pro
PPTXFormatLib.file = ../../../ASCOfficePPTXFile/PPTXLib/Linux/PPTXFormatLib/PPTXFormatLib.pro
ASCOfficeDocxFile2Lib.file = ../../../ASCOfficeDocxFile2/Linux/ASCOfficeDocxFile2Lib.pro
TxtXmlFormatLib.file = ../../../ASCOfficeTxtFile/TxtXmlFormatLib/Linux/TxtXmlFormatLib.pro
RtfFormatLib.file = ../../../ASCOfficeRtfFile/RtfFormatLib/Linux/RtfFormatLib.pro
PptFormatLib.file = ../../../ASCOfficePPTFile/PPTFormatLib/Linux/PPTFormatLib.pro
DocFormatLib.file = ../../../ASCOfficeDocFile/DocFormatLib/Linux/DocFormatLib.pro
OdfFileReaderLib.file = ../../../ASCOfficeOdfFile/linux/OdfFileReaderLib.pro
OdfFileWriterLib.file = ../../../ASCOfficeOdfFileW/linux/OdfFileWriterLib.pro
XlsFormatLib.file = ../../../ASCOfficeXlsFile2/source/linux/XlsFormatLib.pro
X2tConverter.file = ./X2tConverter.pro

X2tConverter.depends = \
    OfficeUtils \
    graphics \
    DocxFormatLib \
    PPTXFormatLib \
    ASCOfficeDocxFile2Lib \
    TxtXmlFormatLib \
    RtfFormatLib \
    PptFormatLib \
    DocFormatLib \
    OdfFileReaderLib \
    OdfFileWriterLib \
    XlsFormatLib

############### destination path ###############
DESTINATION_SDK_PATH = $$PWD/../../../SDK/lib
DESTINATION_ICU = $$PWD/../../../UnicodeConverter/icubuilds

win32:contains(QMAKE_TARGET.arch, x86_64):{
    DESTINATION_ICU = $$DESTINATION_ICU/win64/bin
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_64/DEBUG
} else {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_64
}
}
win32:!contains(QMAKE_TARGET.arch, x86_64):{
    DESTINATION_ICU = $$DESTINATION_ICU/win32/bin
CONFIG(debug, debug|release) {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_32/DEBUG
} else {
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/win_32
}
}

linux-g++ | linux-g++-64 | linux-g++-32:contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_ICU = $$DESTINATION_ICU/linux64/usr/local/lib
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_64
}
linux-g++ | linux-g++-64 | linux-g++-32:!contains(QMAKE_HOST.arch, x86_64):{
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/linux_32
}

mac {
    DESTINATION_ICU = $$DESTINATION_ICU/icubuilds/mac/build_release-55-1/lib
    DESTINATION_SDK_PATH = $$DESTINATION_SDK_PATH/mac_64
}

LIBS += -L$$DESTINATION_SDK_PATH
LIBS += -L$$DESTINATION_ICU
################################################
