#-------------------------------------------------
#
# Project created by QtCreator 2014-05-30T18:51:02
#
#-------------------------------------------------

message($$PWD)
DEFINES += USE_X11 HAVE_SYS_TIME_H

QT       += core

QT       -= gui

TARGET = clipboard_test

CONFIG += link_pkgconfig
CONFIG   += console
CONFIG   -= app_bundle

PKGCONFIG = gtk+-2.0

LIBS += -lX11 -lXfixes -lXrender -lXrandr -lXext

TEMPLATE = app

INCLUDEPATH += \
    ../../src \
    /usr/include/gtk-2.0 \
    /usr/include/glib-2.0 \
    ../../src/third_party/skia/include/core \
    ../../src/third_party/skia/include/config \
    ../../src/third_party/skia/include/pathops \
    ../../src/third_party/skia/src/image \
    ../../src/third_party/libevent/linux \

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
    ../../src/base/message_pump.h \
    ../../src/base/message_pump_gtk.h \
    ../../src/base/message_pump_glib.h \
    ../../src/base/message_pump_default.h \
    ../../src/base/message_pump_dispatcher.h \
    ../../src/base/message_pump_libevent.h \
    ../../src/ui/gfx/gtk_util.h \
    ../../src/base/threading/thread_restrictions.h \
    ../../src/base/at_exit.h \
    ../../src/base/memory/singleton.h \
    ../../src/base/atomicops.h \
    ../../src/base/atomicops_internals_atomicword_compat.h \
    ../../src/base/atomicops_internals_gcc.h \
    ../../src/base/atomicops_internals_x86_gcc.h \
    ../../src/base/message_loop.h \
    ../../src/base/message_loop_proxy.h \
    ../../src/ui/base/keycodes/keyboard_code_conversion.h \
    ../../src/ui/base/keycodes/keyboard_code_conversion_gtk.h \
    ../../src/ui/base/keycodes/keyboard_code_conversion_x.h \
    ../../src/ui/base/keycodes/keyboard_codes.h \
    ../../src/ui/base/keycodes/keyboard_codes_posix.h \
    ../../src/ui/base/keycodes/usb_keycode_map.h

