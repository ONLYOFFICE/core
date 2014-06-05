#-------------------------------------------------
#
# Project created by QtCreator 2014-05-30T18:51:02
#
#-------------------------------------------------

message($$PWD)
DEFINES += USE_X11

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
    ../../src/third_party/skia/src/image

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
    ../../src/third_party/skia/src/core/SkBitmap.cpp \
    ../../src/base/message_pump.cc \
    ../../src/base/message_pump_gtk.cc \
    ../../src/base/message_pump_glib.cc \
    ../../src/base/message_pump_default.cc \
    ../../src/base/message_pump_libevent.cc \
    ../../src/third_party/skia/src/core/SkDebug.cpp \
    ../../src/ui/gfx/image/image.cc \
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
    ../../src/third_party/skia/src/ports/SkDebug_stdio.cpp \
    ../../src/third_party/skia/src/core/Sk64.cpp \
    ../../src/third_party/skia/src/core/SkPixelRef.cpp \
    ../../src/third_party/skia/src/core/SkMallocPixelRef.cpp \
    ../../src/third_party/skia/src/ports/SkMemory_malloc.cpp \
    ../../src/third_party/skia/src/core/SkCanvas.cpp \
    ../../src/third_party/skia/src/core/SkColor.cpp \
    ../../src/third_party/skia/src/core/SkColorTable.cpp \
    ../../src/third_party/skia/src/core/SkMask.cpp \
    ../../src/third_party/skia/src/core/SkMatrix.cpp \
    ../../src/third_party/skia/src/core/SkUnPreMultiply.cpp \
    ../../src/third_party/skia/src/core/SkPackBits.cpp \
    ../../src/third_party/skia/src/core/SkPoint.cpp \
    ../../src/third_party/skia/src/core/SkRect.cpp \
    ../../src/third_party/skia/src/core/SkUtils.cpp \
    ../../src/third_party/skia/src/core/SkPaint.cpp

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
    ../../src/third_party/skia/include/core/Sk64.h \
    ../../src/third_party/skia/include/core/SkAdvancedTypefaceMetrics.h \
    ../../src/third_party/skia/include/core/SkAnnotation.h \
    ../../src/third_party/skia/include/core/SkBitmap.h \
    ../../src/third_party/skia/include/core/SkBlitRow.h \
    ../../src/third_party/skia/include/core/SkBounder.h \
    ../../src/third_party/skia/include/core/SkCanvas.h \
    ../../src/third_party/skia/include/core/SkChecksum.h \
    ../../src/third_party/skia/include/core/SkChunkAlloc.h \
    ../../src/third_party/skia/include/core/SkClipStack.h \
    ../../src/third_party/skia/include/core/SkColor.h \
    ../../src/third_party/skia/include/core/SkColorFilter.h \
    ../../src/third_party/skia/include/core/SkColorPriv.h \
    ../../src/third_party/skia/include/core/SkColorShader.h \
    ../../src/third_party/skia/include/core/SkColorTable.h \
    ../../src/third_party/skia/include/core/SkComposeShader.h \
    ../../src/third_party/skia/include/core/SkData.h \
    ../../src/third_party/skia/include/core/SkDataSet.h \
    ../../src/third_party/skia/include/core/SkDataTable.h \
    ../../src/third_party/skia/include/core/SkDeque.h \
    ../../src/third_party/skia/include/core/SkDevice.h \
    ../../src/third_party/skia/include/core/SkDeviceProperties.h \
    ../../src/third_party/skia/include/core/SkDither.h \
    ../../src/third_party/skia/include/core/SkDraw.h \
    ../../src/third_party/skia/include/core/SkDrawFilter.h \
    ../../src/third_party/skia/include/core/SkDrawLooper.h \
    ../../src/third_party/skia/include/core/SkEmptyShader.h \
    ../../src/third_party/skia/include/core/SkEndian.h \
    ../../src/third_party/skia/include/core/SkError.h \
    ../../src/third_party/skia/include/core/SkFixed.h \
    ../../src/third_party/skia/include/core/SkFlate.h \
    ../../src/third_party/skia/include/core/SkFlattenable.h \
    ../../src/third_party/skia/include/core/SkFlattenableBuffers.h \
    ../../src/third_party/skia/include/core/SkFloatBits.h \
    ../../src/third_party/skia/include/core/SkFloatingPoint.h \
    ../../src/third_party/skia/include/core/SkFontHost.h \
    ../../src/third_party/skia/include/core/SkFontLCDConfig.h \
    ../../src/third_party/skia/include/core/SkGeometry.h \
    ../../src/third_party/skia/include/core/SkGraphics.h \
    ../../src/third_party/skia/include/core/SkImage.h \
    ../../src/third_party/skia/include/core/SkImageFilter.h \
    ../../src/third_party/skia/include/core/SkImageFilterUtils.h \
    ../../src/third_party/skia/include/core/SkImageTypes.h \
    ../../src/third_party/skia/include/core/SkInstCnt.h \
    ../../src/third_party/skia/include/core/SkLineClipper.h \
    ../../src/third_party/skia/include/core/SkMallocPixelRef.h \
    ../../src/third_party/skia/include/core/SkMask.h \
    ../../src/third_party/skia/include/core/SkMaskFilter.h \
    ../../src/third_party/skia/include/core/SkMath.h \
    ../../src/third_party/skia/include/core/SkMatrix.h \
    ../../src/third_party/skia/include/core/SkMetaData.h \
    ../../src/third_party/skia/include/core/SkOSFile.h \
    ../../src/third_party/skia/include/core/SkPackBits.h \
    ../../src/third_party/skia/include/core/SkPaint.h \
    ../../src/third_party/skia/include/core/SkPath.h \
    ../../src/third_party/skia/include/core/SkPathEffect.h \
    ../../src/third_party/skia/include/core/SkPathMeasure.h \
    ../../src/third_party/skia/include/core/SkPicture.h \
    ../../src/third_party/skia/include/core/SkPixelRef.h \
    ../../src/third_party/skia/include/core/SkPoint.h \
    ../../src/third_party/skia/include/core/SkPostConfig.h \
    ../../src/third_party/skia/include/core/skpreconfig.h \
    ../../src/third_party/skia/include/core/SkRasterizer.h \
    ../../src/third_party/skia/include/core/SkReader32.h \
    ../../src/third_party/skia/include/core/SkRect.h \
    ../../src/third_party/skia/include/core/SkRefCnt.h \
    ../../src/third_party/skia/include/core/SkRegion.h \
    ../../src/third_party/skia/include/core/SkRRect.h \
    ../../src/third_party/skia/include/core/SkScalar.h \
    ../../src/third_party/skia/include/core/SkScalarCompare.h \
    ../../src/third_party/skia/include/core/SkShader.h \
    ../../src/third_party/skia/include/core/SkSize.h \
    ../../src/third_party/skia/include/core/SkStream.h \
    ../../src/third_party/skia/include/core/SkString.h \
    ../../src/third_party/skia/include/core/SkStringUtils.h \
    ../../src/third_party/skia/include/core/SkStrokeRec.h \
    ../../src/third_party/skia/include/core/SkSurface.h \
    ../../src/third_party/skia/include/core/SkTArray.h \
    ../../src/third_party/skia/include/core/SkTDArray.h \
    ../../src/third_party/skia/include/core/SkTDict.h \
    ../../src/third_party/skia/include/core/SkTDStack.h \
    ../../src/third_party/skia/include/core/SkTemplates.h \
    ../../src/third_party/skia/include/core/SkThread.h \
    ../../src/third_party/skia/include/core/SkThread_platform.h \
    ../../src/third_party/skia/include/core/SkTileGridPicture.h \
    ../../src/third_party/skia/include/core/SkTime.h \
    ../../src/third_party/skia/include/core/SkTInternalLList.h \
    ../../src/third_party/skia/include/core/SkTLazy.h \
    ../../src/third_party/skia/include/core/SkTrace.h \
    ../../src/third_party/skia/include/core/SkTRegistry.h \
    ../../src/third_party/skia/include/core/SkTScopedPtr.h \
    ../../src/third_party/skia/include/core/SkTSearch.h \
    ../../src/third_party/skia/include/core/SkTypeface.h \
    ../../src/third_party/skia/include/core/SkTypes.h \
    ../../src/third_party/skia/include/core/SkUnitMapper.h \
    ../../src/third_party/skia/include/core/SkUnPreMultiply.h \
    ../../src/third_party/skia/include/core/SkUtils.h \
    ../../src/third_party/skia/include/core/SkWeakRefCnt.h \
    ../../src/third_party/skia/include/core/SkWriter32.h \
    ../../src/third_party/skia/include/core/SkXfermode.h \
    ../../src/ui/base/x/x11_util.h \
    ../../src/base/message_pump.h \
    ../../src/base/message_pump_gtk.h \
    ../../src/base/message_pump_glib.h \
    ../../src/base/message_pump_default.h \
    ../../src/base/message_pump_dispatcher.h \
    ../../src/base/message_pump_libevent.h \
    ../../src/ui/gfx/image/image.h \
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
    ../../src/third_party/skia/include/pathops/SkPathOps.h \
    ../../src/third_party/skia/src/image/SkSurface_Base.h
