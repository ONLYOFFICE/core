DEFINES += HAVE_VA_COPY

INCLUDEPATH += \
    $$CORE_ROOT_DIR/DesktopEditor/xml/libxml2/include \
    $$CORE_ROOT_DIR/DesktopEditor/xml/libxml2/include/libxml \
    $$CORE_ROOT_DIR/DesktopEditor/xml/build/qt

SOURCES += $$CORE_ROOT_DIR/DesktopEditor/xml/build/qt/libxml2_all.c
SOURCES += $$CORE_ROOT_DIR/DesktopEditor/xml/build/qt/libxml2_all2.c

SOURCES +=  \
    $$CORE_ROOT_DIR/DesktopEditor/xml/src/xmllight.cpp \
    $$CORE_ROOT_DIR/DesktopEditor/xml/src/xmldom.cpp
