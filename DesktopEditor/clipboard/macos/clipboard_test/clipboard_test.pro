#-------------------------------------------------
#
# Project created by QtCreator 2014-06-17T12:29:18
#
#-------------------------------------------------

#QT       += core

QT       -= gui

TARGET = clipboard_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../src \
    /usr/include/libxml2

DEPENDPATH += /usr/lib

LIBS += -lxml2 -framework Cocoa

SOURCES += main.cpp \
    ../../src/ui/base/clipboard/clipboard.cc \
    ../../src/ui/base/clipboard/custom_data_helper.cc \
    ../../src/ui/base/clipboard/scoped_clipboard_writer.cc \
    ../../src/base/pickle.cc \
    ../../src/base/string_util.cc \
    ../../src/base/string16.cc \
    ../../src/base/stringprintf.cc \
    ../../src/base/third_party/dmg_fp/dtoa.cc \
    ../../src/base/third_party/dmg_fp/g_fmt.cc \
    ../../src/base/third_party/icu/icu_utf.cc \
    ../../src/base/strings/string_number_conversions.cc \
    ../../src/base/strings/string_piece.cc \
    ../../src/base/strings/utf_offset_string_conversions.cc \
    ../../src/base/strings/utf_string_conversion_utils.cc \
    ../../src/base/strings/utf_string_conversions.cc \
    ../../src/ui/gfx/size.cc
#\
#    ../../src/base/mac/launch_services_util.cc \
#    ../../src/base/mac/launchd.cc \
#    ../../src/base/mac/libdispatch_task_runner.cc

HEADERS += \
    ../../src/ui/base/clipboard/clipboard.h \
    ../../src/ui/base/clipboard/custom_data_helper.h \
    ../../src/ui/base/clipboard/scoped_clipboard_writer.h \
    ../../src/base/pickle.h \
    ../../src/base/string_util.h \
    ../../src/base/string16.h \
    ../../src/base/stringprintf.h \
    ../../src/base/third_party/dmg_fp/dmg_fp.h \
    ../../src/base/third_party/icu/icu_utf.h \
    ../../src/base/strings/string_number_conversions.h \
    ../../src/base/strings/string_piece.h \
    ../../src/base/strings/utf_offset_string_conversions.h \
    ../../src/base/strings/utf_string_conversion_utils.h \
    ../../src/base/strings/utf_string_conversions.h \
    ../../src/ui/gfx/size.h \
    ../../src/third_party/mozilla/NSPasteboard+Utils.h
#    ../../src/base/mac/foundation_util.h
#\
#    ../../src/third_party/mozilla/ComplexTextInputPanel.h \
#     \
#    ../../src/third_party/mozilla/NSScreen+Utils.h \
#    ../../src/third_party/mozilla/NSString+Utils.h \
#    ../../src/third_party/mozilla/NSURL+Utils.h \
#    ../../src/third_party/mozilla/NSWorkspace+Utils.h \
#    ../../src/base/mac/mac_util.h \
#    ../../src/base/mac/scoped_cftyperef.h \
#    ../../src/base/mac/authorization_util.h \
#    ../../src/base/mac/bind_objc_block.h \
#    ../../src/base/mac/bundle_locations.h \
#    ../../src/base/mac/cocoa_protocols.h \
#    ../../src/base/mac/launch_services_util.h \
#    ../../src/base/mac/launchd.h \
#    ../../src/base/mac/libdispatch_task_runner.h \
#    ../../src/base/mac/objc_property_releaser.h \
#    ../../src/base/strings/sys_string_conversions.h

OBJECTIVE_SOURCES += \
    ../../src/ui/base/clipboard/custom_data_helper_mac.mm \
    ../../src/ui/base/clipboard/clipboard_mac.mm \
    ../../src/third_party/mozilla/NSPasteboard+Utils.mm \
    ../../src/base/strings/sys_string_conversions_mac.mm
#   ../../src/base/mac/foundation_util.mm
# \
#    ../../src/third_party/mozilla/ComplexTextInputPanel.mm \
#
#    ../../src/third_party/mozilla/NSString+Utils.mm \
#    ../../src/base/mac/mac_util.mm \
#    ../../src/base/mac/authorization_util.mm \
#    ../../src/base/mac/bundle_locations.mm \
#    ../../src/base/mac/objc_property_releaser.mm \
#    ../../src/base/strings/sys_string_conversions_mac.mm

OTHER_FILES += \
#    ../../src/third_party/mozilla/NSScreen+Utils.m \
#    ../../src/third_party/mozilla/NSURL+Utils.m \
#    ../../src/third_party/mozilla/NSWorkspace+Utils.m
