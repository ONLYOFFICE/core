INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

CORE_ROOT_DIR = $$PWD/../../../..

HEADERS += $$files($$PWD/src/*.h, true)
SOURCES += $$files($$PWD/src/*.cpp, true)

include($$CORE_ROOT_DIR/Common/3dParty/html/katana.pri)
