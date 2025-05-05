DEFINES += MD4C_USE_UTF8

HEADERS += $$PWD/md4c/src/md4c.h \
           $$PWD/md4c/src/md4c-html.h \
           $$PWD/md4c/src/entity.h \
           $$PWD/md2html.h \

SOURCES += $$PWD/md4c/src/md4c.c \
           $$PWD/md4c/src/md4c-html.c \
           $$PWD/md4c/src/entity.c \
           $$PWD/md2html.cpp
