TEMPLATE = subdirs

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

include(../../../Common/base.pri)

SUBDIRS = \
    OfficeUtils \
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
    XmlLib \
    X2tConverter

OfficeUtils.file = ../../../OfficeUtils/OfficeUtils.pro
graphics.file = ../../../DesktopEditor/Qt_build/graphics/project/graphics.pro
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
XmlLib.file = ../../../DesktopEditor/xml/build/qt/libxml2.pro
X2tConverter.file = ./X2tConverter.pro

X2tConverter.depends = \
    OfficeUtils \
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
    XmlLib

# for run in qt from this solution
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH
include(../../../Common/3dParty/icu/icu.pri)
