INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

CORE_ROOT_DIR = $$PWD/../../../..

HEADERS += $$files($$PWD/src/*.h, true) \
    $$PWD/src/CDocumentStyle.h \
    $$PWD/src/CXmlElement.h
SOURCES += $$files($$PWD/src/*.cpp, true) \
    $$PWD/src/CDocumentStyle.cpp \
    $$PWD/src/CXmlElement.cpp

include($$CORE_ROOT_DIR/Common/3dParty/html/katana.pri)
