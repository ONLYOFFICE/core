TEMPLATE = subdirs

CORE_ROOT_DIR = $$PWD/../../..
PWD_ROOT_DIR = $$PWD

include(../../../Common/base.pri)

SUBDIRS = \
    XlsbFormatLib \
    VbaFormatLib \
    DocxFormatLib \
    PPTXFormatLib \
    BinDocument \
    TxtXmlFormatLib \
    RtfFormatLib \
    PptFormatLib \
    DocFormatLib \
    OdfFormatLib \
    XlsFormatLib \
    CompoundFileLib \
    X2tConverter

XlsbFormatLib.file =	../../../OOXML/Projects/Linux/XlsbFormatLib/XlsbFormatLib.pro
DocxFormatLib.file =	../../../OOXML/Projects/Linux/DocxFormatLib/DocxFormatLib.pro
PPTXFormatLib.file =	../../../OOXML/Projects/Linux/PPTXFormatLib/PPTXFormatLib.pro
BinDocument.file =	../../../OOXML/Projects/Linux/BinDocument/BinDocument.pro
TxtXmlFormatLib.file =	../../../TxtFile/Projects/Linux/TxtXmlFormatLib.pro
RtfFormatLib.file =	../../../RtfFile/Projects/Linux/RtfFormatLib.pro
PptFormatLib.file =	../../../MsBinaryFile/Projects/PPTFormatLib/Linux/PPTFormatLib.pro
DocFormatLib.file =	../../../MsBinaryFile/Projects/DocFormatLib/Linux/DocFormatLib.pro
OdfFormatLib.file =	../../../OdfFile/Projects/Linux/OdfFormatLib.pro
XlsFormatLib.file =	../../../MsBinaryFile/Projects/XlsFormatLib/Linux/XlsFormatLib.pro
VbaFormatLib.file =	../../../MsBinaryFile/Projects/VbaFormatLib/Linux/VbaFormatLib.pro
CompoundFileLib.file =	../../../Common/cfcpp/cfcpp.pro

X2tConverter.depends = \
    XlsbFormatLib \
    DocxFormatLib \
    PPTXFormatLib \
    BinDocument \
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
