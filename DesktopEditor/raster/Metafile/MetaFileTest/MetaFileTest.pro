QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY
DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY
DEFINES += KERNEL_USE_DYNAMIC_LIBRARY
DEFINES += GRAPHICS_USE_DYNAMIC_LIBRARY

INCLUDEPATH += \
    $$PWD/../../../agg-2.4/include \
    $$PWD/../../../freetype-2.5.2/include

SOURCES += main.cpp

LIBS += -L$$PWD/../../../../build/lib/win_64/DEBUG/ -lHtmlRenderer
LIBS += -L$$PWD/../../../../build/lib/win_64/DEBUG/ -lgraphics
LIBS += -L$$PWD/../../../../build/lib/win_64/DEBUG/ -lkernel
LIBS += -L$$PWD/../../../../build/lib/win_64/DEBUG/ -lUnicodeConverter

INCLUDEPATH += $$PWD/../../../../build/lib/win_64/DEBUG
DEPENDPATH += $$PWD/../../../../build/lib/win_64/DEBUG
