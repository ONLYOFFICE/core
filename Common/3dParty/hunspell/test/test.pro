#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T18:28:42
#
#-------------------------------------------------

QT -= core gui

TARGET = dictionariestester
CONFIG += console
CONFIG -= app_bundle

DEFINES += KERNEL_USE_DYNAMIC_LIBRARY

TEMPLATE = app

CONFIG += hunspell_build_static

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/Common/3dParty/icu/icu.pri)
include($$CORE_ROOT_DIR/Common/3dParty/hunspell/qt/hunspell.pri)

# custom time limits of hunspell in clocks (if before.py was executed)
# when increasing the limit for each case, it is important to consider that the total time will
# also increase, so it is good to increase the global limit. this works the same for the candidate limit with suggest limit
DEFINES += CUSTOM_TIMELIMITS

escape_bracket=
!core_windows:escape_bracket=\\

# total time limit per word for all cases. (default is CLOCKS_PER_SEC/4)
DEFINES += "CUSTOM_TIMELIMIT_GLOBAL=$${escape_bracket}(20*CLOCKS_PER_SEC$${escape_bracket})"

# total time limit per "1 case" - forgotten char, double char, moved char and so on for all candidates. (default is CLOCKS_PER_SEC/10)
DEFINES += "CUSTOM_TIMELIMIT_SUGGESTION=$${escape_bracket}(5*CLOCKS_PER_SEC$${escape_bracket})"

# time limit per candidate (default is CLOCKS_PER_SEC/20)
DEFINES += "CUSTOM_TIMELIMIT=$${escape_bracket}(CLOCKS_PER_SEC$${escape_bracket}\)"

ADD_DEPENDENCY(UnicodeConverter kernel)

core_windows:LIBS += -lgdi32 -ladvapi32 -luser32 -lshell32

SOURCES += main.cpp

DESTDIR = $$CORE_BUILDS_BINARY_PATH
