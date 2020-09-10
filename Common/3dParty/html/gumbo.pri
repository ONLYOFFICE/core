INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

core_linux:QMAKE_CFLAGS += -std=c99
core_windows:INCLUDEPATH += $$PWD/gumbo-parser/visualc/include

HEADERS += $$files($$PWD/gumbo-parser/src/*.h, true) \
           $$PWD/htmltoxhtml.h

SOURCES += $$files($$PWD/gumbo-parser/src/*.c, true)
