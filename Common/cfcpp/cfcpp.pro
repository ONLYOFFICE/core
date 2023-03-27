QT       -= core gui

TARGET = CompoundFileLib
TEMPLATE = lib
CONFIG += staticlib

CORE_ROOT_DIR = $$PWD/../..
PWD_ROOT_DIR = $$PWD

CONFIG += core_x2t

include(../base.pri)

SOURCES += \
    RBTree/rbtree.cpp \
    Stream/fstream_utils.cpp \
    Stream/stream_utils.cpp \
    cfitem.cpp \
    cfstorage.cpp \
    cfstream.cpp \
    compoundfile.cpp \
    directoryentry.cpp \
    header.cpp \
    sector.cpp \
    sectorcollection.cpp \
    streamrw.cpp \
    streamview.cpp

HEADERS += \
    RBTree/action.h \
    RBTree/irbnode.h \
    RBTree/rbtree.h \
    RBTree/rbtreeexception.h \
    Stream/fstream_utils.h \
    Stream/fstream_wrapper.h \
    Stream/stream_utils.h \
    cfexception.h \
    cfitem.h \
    cfstorage.h \
    cfstream.h \
    compoundfile.h \
    compoundfile_impl.h \
    directoryentry.h \
    event.h \
    guid.h \
    header.h \
    idirectoryentry.h \
    sector.h \
    sectorcollection.h \
    slist.h \
    Stream/stream.h \
    streamrw.h \
    streamview.h \
    svector.h
