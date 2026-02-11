QT -= core
QT -= gui

VERSION = 0.0.0.1
TARGET = HtmlFile2
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

DEFINES += HTMLFILE2_USE_DYNAMIC_LIBRARY
DEFINES += CSSCALCULATOR_LIBRARY_STATIC
DEFINES += CSS_CALCULATOR_WITH_XHTML
DEFINES += DISABLE_RUBY_SUPPORT
DEFINES += MDCONVERTER_DECL_EXPORT
DEFINES += HTML_NORMALIZER_DECL_EXPORT

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

# equal .c names
CONFIG += object_parallel_to_source
core_windows:CONFIG += no_batch

include($$CORE_ROOT_DIR/Common/3dParty/html/gumbo.pri)
include($$CORE_ROOT_DIR/Common/3dParty/html/css/CssCalculator.pri)
include($$CORE_ROOT_DIR/Common/3dParty/md/md2html.pri)

CONFIG += core_boost_regex
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter, graphics, kernel_network)

SOURCES += htmlfile2.cpp \
    ./Writers/OOXMLWriter.cpp \
    HTMLReader.cpp \
    Tags/MDTags.cpp \
    Writers/MDWriter.cpp \
    Table.cpp \
    Tags/OOXMLTags.cpp

HEADERS += htmlfile2.h \
	./src/StringFinder.h \
	./src/Languages.h \
	Common.h \
	./Writers/OOXMLWriter.h \
	./Writers/IWriter.h \
	HTMLParameters.h \
	HTMLReader.h \
	./Tags/HTMLTags.h \
	MarkdownParameters.h \
	Tags/MDTags.h \
	Writers/MDWriter.h \
	Table.h \
	Tags/OOXMLTags.h
