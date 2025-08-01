SRC_DIR = $$PWD/brotli/c
DEFINES += FT_CONFIG_OPTION_USE_BROTLI

INCLUDEPATH += \
  $$SRC_DIR/include

SOURCES += $$files($$SRC_DIR/common/*.c)
SOURCES += $$files($$SRC_DIR/dec/*.c)
#SOURCES += $$files($$SRC_DIR/enc/*.c)
