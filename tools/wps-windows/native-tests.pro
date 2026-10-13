TEMPLATE = app
CONFIG += console c++14
QT -= core gui
CORE = $$PWD/../..
include($$CORE/X2tConverter/build/Qt/X2tConverter.pri)
TARGET = wps-cell-images-tests
SOURCES -= $$CORE/X2tConverter/src/ASCConverters.cpp
SOURCES -= $$CORE/X2tConverter/src/cextracttools.cpp
SOURCES -= ../../../Common/OfficeFileFormatChecker2.cpp
SOURCES -= ../../src/ASCConverters.cpp
SOURCES -= ../../src/cextracttools.cpp
SOURCES += $$CORE/Common/OfficeFileFormatChecker2.cpp
SOURCES += $$CORE/OOXML/test/wps-cell-images/formula.cpp
SOURCES += $$CORE/OOXML/test/wps-cell-images/conversion.cpp
GTEST = $$(WPS_GTEST_ROOT)
INCLUDEPATH += $$GTEST $$GTEST/include
SOURCES += $$GTEST/src/gtest-all.cc $$GTEST/src/gtest_main.cc
DESTDIR = $$CORE/build/bin/win_64
