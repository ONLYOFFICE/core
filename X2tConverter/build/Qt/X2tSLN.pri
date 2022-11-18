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
    OdfFormatLib \
    XlsFormatLib \
    CompoundFileLib \
    X2tConverter

XlsbFormatLib.file = 			../../../OOXML/Projects/Linux/XlsbFormatLib/XlsbFormatLib.pro
DocxFormatLib.file = 			../../../OOXML/Projects/Linux/DocxFormatLib/DocxFormatLib.pro
PPTXFormatLib.file = 			../../../OOXML/Projects/Linux/PPTXFormatLib/PPTXFormatLib.pro
ASCOfficeDocxFile2Lib.file = 	../../../OOXML/Projects/Linux/ASCOfficeDocxFile2Lib/ASCOfficeDocxFile2Lib.pro
TxtXmlFormatLib.file = 			../../../ASCOfficeTxtFile/Projects/Linux/TxtXmlFormatLib.pro
RtfFormatLib.file = 			../../../ASCOfficeRtfFile/Projects/Linux/RtfFormatLib.pro
PptFormatLib.file = 			../../../MsBinaryFile/Projects/PPTFormatLib/Linux/PPTFormatLib.pro
DocFormatLib.file = 			../../../MsBinaryFile/Projects/DocFormatLib/Linux/DocFormatLib.pro
OdfFormatLib.file = 			../../../ASCOfficeOdfFile/Projects/Linux/OdfFormatLib.pro
XlsFormatLib.file = 			../../../MsBinaryFile/Projects/XlsFormatLib/Linux/XlsFormatLib.pro
VbaFormatLib.file = 			../../../MsBinaryFile/Projects/VbaFormatLib/Linux/VbaFormatLib.pro
CompoundFileLib.file = 			../../../Common/cfcpp/cfcpp.pro

X2tConverter.depends = \
    XlsbFormatLib \
    DocxFormatLib \
    PPTXFormatLib \
    ASCOfficeDocxFile2Lib \
    TxtXmlFormatLib \
    RtfFormatLib \
    PptFormatLib \
    DocFormatLib \
    OdfFormatLib \
    VbaFormatLib \
    XlsFormatLib \
    CompoundFileLib

# build the project sequentially as listed in SUBDIRS !
CONFIG += ordered

# for run in qt from this solution
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH
include(../../../Common/3dParty/icu/icu.pri)
