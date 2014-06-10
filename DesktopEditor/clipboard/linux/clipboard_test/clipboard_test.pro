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
    ../../src/ui/base/x/x11_util.cc \
    ../../src/base/message_pump.cc \
    ../../src/base/message_pump_gtk.cc \
    ../../src/base/message_pump_glib.cc \
    ../../src/base/message_pump_default.cc \
    ../../src/base/message_pump_libevent.cc \
    ../../src/ui/gfx/gtk_util.cc \
    ../../src/base/threading/thread_restrictions.cc \
    ../../src/base/at_exit.cc \
    ../../src/base/memory/singleton.cc \
    ../../src/base/atomicops_internals_x86_gcc.cc \
    ../../src/base/message_loop.cc \
    ../../src/ui/base/keycodes/keyboard_code_conversion.cc \
    ../../src/ui/base/keycodes/keyboard_code_conversion_gtk.cc \
    ../../src/ui/base/keycodes/keyboard_code_conversion_x.cc \
    ../../src/base/location.cc \
    ../../src/ui/gfx/rect.cc \
    ../../src/ui/gfx/rect_conversions.cc \
    ../../src/ui/gfx/rect_f.cc \
    ../../src/ui/gfx/point.cc \
    ../../src/ui/gfx/point3_f.cc \
    ../../src/ui/gfx/point_conversions.cc \
    ../../src/ui/gfx/point_f.cc \
    ../../src/base/memory/ref_counted.cc \
    ../../src/base/memory/ref_counted_memory.cc \
    ../../src/base/callback_internal.cc \
    ../../src/base/threading/platform_thread_posix.cc \
    ../../src/base/time.cc \
    ../../src/base/time_posix.cc \
    ../../src/base/memory/weak_ptr.cc \
    ../../src/base/synchronization/waitable_event_posix.cc \
    ../../src/third_party/libevent/event.c \
    ../../src/base/synchronization/lock.cc \
    ../../src/base/synchronization/lock_impl_posix.cc \
    ../../src/base/threading/thread_checker_impl.cc \
    ../../src/base/threading/thread_local_posix.cc \
    ../../src/base/threading/thread_local_storage_posix.cc \
    ../../src/base/pending_task.cc \
    ../../src/base/message_loop/message_loop_proxy.cc \
    ../../src/base/message_loop/message_loop_proxy_impl.cc \
    ../../src/base/run_loop.cc \
    ../../src/base/task_runner.cc \
    ../../src/base/synchronization/condition_variable_posix.cc \
    ../../src/base/threading/thread_id_name_manager.cc \
    ../../src/base/tracked_objects.cc \
    ../../src/base/tracking_info.cc \
    ../../src/base/profiler/tracked_time.cc \
    ../../src/base/debug/trace_event_impl.cc \
    ../../src/base/lazy_instance.cc \
    ../../src/base/thread_task_runner_handle.cc \
    ../../src/base/debug/alias.cc \
    ../../src/base/metrics/statistics_recorder.cc \
    ../../src/base/metrics/histogram.cc \
    ../../src/base/metrics/histogram_base.cc \
    ../../src/base/json/json_string_value_serializer.cc \
    ../../src/base/json/json_reader.cc \
    ../../src/base/json/json_writer.cc \
    ../../src/base/json/string_escape.cc \
    ../../src/base/json/json_parser.cc \
    ../../src/ui/gfx/vector2d.cc \
    ../../src/ui/gfx/vector2d_conversions.cc \
    ../../src/ui/gfx/vector2d_f.cc \
    ../../src/ui/gfx/vector3d_f.cc


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
    ../../src/ui/base/x/x11_util.h \
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
    ../../src/ui/base/keycodes/usb_keycode_map.h \
    ../../src/base/location.h \
    ../../src/ui/gfx/rect.h \
    ../../src/ui/gfx/rect_base.h \
    ../../src/ui/gfx/rect_conversions.h \
    ../../src/ui/gfx/rect_f.h \
    ../../src/ui/gfx/point.h \
    ../../src/ui/gfx/point3_f.h \
    ../../src/ui/gfx/point_base.h \
    ../../src/ui/gfx/point_conversions.h \
    ../../src/ui/gfx/point_f.h \
    ../../src/base/memory/ref_counted.h \
    ../../src/base/memory/ref_counted_memory.h \
    ../../src/base/callback_internal.h \
    ../../src/base/synchronization/waitable_event.h \
    ../../src/third_party/libevent/event.h \
    ../../src/base/synchronization/lock.h \
    ../../src/base/synchronization/lock_impl.h \
    ../../src/base/threading/thread_checker.h \
    ../../src/base/threading/thread_checker_impl.h \
    ../../src/base/threading/thread_local.h \
    ../../src/base/threading/thread_local_storage.h \
    ../../src/base/pending_task.h \
    ../../src/base/message_loop/message_loop_proxy.h \
    ../../src/base/message_loop/message_loop_proxy_impl.h \
    ../../src/base/run_loop.h \
    ../../src/base/task_runner.h \
    ../../src/base/task_runner_util.h \
    ../../src/base/synchronization/condition_variable.h \
    ../../src/base/threading/thread_id_name_manager.h \
    ../../src/base/tracked_objects.h \
    ../../src/base/tracking_info.h \
    ../../src/base/profiler/tracked_time.h \
    ../../src/base/debug/trace_event.h \
    ../../src/base/debug/trace_event_impl.h \
    ../../src/base/lazy_instance.h \
    ../../src/base/thread_task_runner_handle.h \
    ../../src/base/debug/alias.h \
    ../../src/base/metrics/statistics_recorder.h \
    ../../src/base/metrics/histogram.h \
    ../../src/base/metrics/histogram_base.h \
    ../../src/base/json/json_string_value_serializer.h \
    ../../src/base/json/json_reader.h \
    ../../src/base/json/json_writer.h \
    ../../src/base/json/string_escape.h \
    ../../src/base/json/json_parser.h \
    ../../src/ui/gfx/vector2d.h \
    ../../src/ui/gfx/vector2d_conversions.h \
    ../../src/ui/gfx/vector2d_f.h \
    ../../src/ui/gfx/vector3d_f.h
