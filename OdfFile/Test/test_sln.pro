TEMPLATE = subdirs

CORE_ROOT_DIR = $$PWD/../../

include($$CORE_ROOT_DIR/Common/base.pri)

SUBDIRS = \
    TestOdf\
    OdfFormatLib\
    X2tConverter

TestOdf.file = $$PWD/test_odf/test_odf.pro
OdfFormatLib.file = $$CORE_ROOT_DIR/OdfFile/Projects/Linux/OdfFormatLib.pro
X2tConverter.file = $$CORE_ROOT_DIR/X2tConverter/build/Qt/X2tConverterDll.pro

X2tConverter.depends = OdfFormatLib

TestOdf.depends = X2tConverter
