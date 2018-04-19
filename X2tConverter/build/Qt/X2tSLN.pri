TEMPLATE = subdirs

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

include(../../../Common/base.pri)

SUBDIRS = \
    kernel \
    graphics \
    cryptopp \
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

kernel.file = ../../../Common/kernel.pro
graphics.file = ../../../DesktopEditor/graphics/pro/graphics.pro
cryptopp.file = ../../../Common/3dParty/cryptopp/project/cryptopp.pro

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

X2tConverter.depends = \
    kernel \
    graphics \
    cryptopp \
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

# for run in qt from this solution
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH
include(../../../Common/3dParty/icu/icu.pri)
