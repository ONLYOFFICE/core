TEMPLATE = subdirs

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

include(../../../Common/base.pri)

SUBDIRS = \
    XlsbFormatLib \
    VbaFormatLib \
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
    kernel \
    HtmlFile2 \
    EpubFile \
    XpsFile \
    doctrenderer \
    X2tConverter

XlsbFormatLib.file = ../../../Common/DocxFormat/DocxFormatLib/XlsbFormatLib.pro
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
VbaFormatLib.file = ../../../ASCOfficeXlsFile2/source/linux/VbaFormatLib.pro
HtmlFile2.file = ../../../HtmlFile2/HtmlFile2.pro
EpubFile.file = ../../../EpubFile/CEpubFile.pro
XpsFile.file = ../../../XpsFile/XpsFile.pro
kernel.file = ../../../Common/kernel.pro
doctrenderer.file = ../../../DesktopEditor/doctrenderer/doctrenderer.pro

X2tConverter.depends = \
    XlsbFormatLib \
    DocxFormatLib \
    PPTXFormatLib \
    ASCOfficeDocxFile2Lib \
    TxtXmlFormatLib \
    RtfFormatLib \
    PptFormatLib \
    DocFormatLib \
    OdfFileReaderLib \
    OdfFileWriterLib \
    VbaFormatLib \
    XlsFormatLib \
    kernel.pro \
    HtmlFile2 \
    EpubFile \
    XpsFile \
    doctrenderer

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

# for run in qt from this solution
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH
include(../../../Common/3dParty/icu/icu.pri)
