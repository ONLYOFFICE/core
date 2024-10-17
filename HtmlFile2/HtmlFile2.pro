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

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

# equal .c names
CONFIG += object_parallel_to_source
core_windows:CONFIG += no_batch

include($$CORE_ROOT_DIR/Common/3dParty/html/gumbo.pri)
include($$CORE_ROOT_DIR/Common/3dParty/html/css/CssCalculator.pri)

CONFIG += core_boost_regex
include($$CORE_ROOT_DIR/Common/3dParty/boost/boost.pri)

ADD_DEPENDENCY(kernel, UnicodeConverter, graphics, kernel_network)

SOURCES += htmlfile2.cpp

HEADERS += htmlfile2.h \
	./src/StringFinder.h \
	./src/Languages.h
