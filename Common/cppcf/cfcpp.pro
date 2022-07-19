CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#BOOST
include(../3dParty/boost/boost.pri)

SOURCES += \
    RBTree/rbtree.cpp \
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
    RBTree/irbnode.h \
    RBTree/rbtree.h \
    RBTree/rbtreeexception.h \
    Stream.h \
    cfexception.h \
    cfitem.h \
    cfstorage.h \
    cfstream.h \
    compoundfile.h \
    directoryentry.h \
    event.h \
    guid.h \
    header.h \
    idirectoryentry.h \
    sector.h \
    sectorcollection.h \
    slist.h \
    streamrw.h \
    streamview.h \
    svector.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
