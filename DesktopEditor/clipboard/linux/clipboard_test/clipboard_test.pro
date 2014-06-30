#-------------------------------------------------
#
# Project created by QtCreator 2014-05-30T18:51:02
#
#-------------------------------------------------

message($$PWD)
DEFINES += USE_X11 HAVE_SYS_TIME_H

#QT       += core

QT       -= gui

TARGET = clipboard_test

CONFIG += link_pkgconfig
CONFIG   += console
CONFIG   -= app_bundle

PKGCONFIG = gtk+-2.0

LIBS += -lX11 -lXfixes -lXrender -lXrandr -lXext -lxml2

TEMPLATE = app

INCLUDEPATH += \
    ../../src \
    /usr/include/gtk-2.0 \
    /usr/include/glib-2.0 \
    /usr/include/libxml2

SOURCES += main.cpp \
    ../../src/ui/base/clipboard/clipboard.cc \
    ../../src/ui/base/clipboard/clipboard_gtk.cc \
    ../../src/ui/base/clipboard/custom_data_helper.cc \
    ../../src/ui/base/clipboard/custom_data_helper_linux.cc \
    ../../src/ui/base/clipboard/scoped_clipboard_writer.cc \
    ../../src/base/third_party/dmg_fp/dtoa.cc \
    ../../src/base/third_party/dmg_fp/g_fmt.cc \
    ../../src/base/third_party/icu/icu_utf.cc \
    ../../src/base/pickle.cc \
    ../../src/base/string16.cc \
    ../../src/base/string_util.cc \
    ../../src/base/strings/string_number_conversions.cc \
    ../../src/base/strings/string_piece.cc \
    ../../src/base/stringprintf.cc \
    ../../src/base/strings/utf_offset_string_conversions.cc \
    ../../src/base/strings/utf_string_conversions.cc \
    ../../src/base/strings/utf_string_conversion_utils.cc \
    ../../src/ui/gfx/size.cc \
    ../../src/ui/gfx/gtk_util.cc

HEADERS += \
    ../../src/ui/base/clipboard/clipboard.h \
    ../../src/ui/base/clipboard/custom_data_helper.h \
    ../../src/ui/base/clipboard/scoped_clipboard_writer.h \
    ../../src/build/build_config.h \
    ../../src/base/third_party/dmg_fp/dmg_fp.h \
    ../../src/base/third_party/icu/icu_utf.h \
    ../../src/base/pickle.h \
    ../../src/base/string16.h \
    ../../src/base/string_piece.h \
    ../../src/base/string_util.h \
    ../../src/base/string_util_posix.h \
    ../../src/base/strings/string_number_conversions.h \
    ../../src/base/strings/string_piece.h \
    ../../src/base/stringprintf.h \
    ../../src/base/strings/utf_offset_string_conversions.h \
    ../../src/base/strings/utf_string_conversions.h \
    ../../src/base/strings/utf_string_conversion_utils.h \
    ../../src/ui/gfx/size.h \
    ../../src/ui/gfx/gtk_util.h

