# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'skia',
      'type': '<(component)',
      'variables': {
        'conditions': [
          ['OS== "ios"', {
            'skia_support_gpu': 0,
          }, {
            'skia_support_gpu': 1,
          }],

          ['inside_chromium_build==0', {
            'webkit_src_dir': '<(DEPTH)/../../..',
          },{
            'webkit_src_dir': '<(DEPTH)/third_party/WebKit',
          }],
        ],

        'optimize': 'max',

        # These two set the paths so we can include skia/gyp/core.gypi
        'skia_src_path': '../third_party/skia/src',
        'skia_include_path': '../third_party/skia/include',
      },

      'includes': [
        '../third_party/skia/gyp/core.gypi',
        '../third_party/skia/gyp/effects.gypi',
      ],

      'sources': [
        # this should likely be moved into src/utils in skia
        '../third_party/skia/src/core/SkFlate.cpp',

        #'../third_party/skia/src/images/bmpdecoderhelper.cpp',
        #'../third_party/skia/src/images/bmpdecoderhelper.h',
        #'../third_party/skia/src/images/SkFDStream.cpp',
        #'../third_party/skia/src/images/SkImageDecoder.cpp',
        #'../third_party/skia/src/images/SkImageDecoder_FactoryDefault.cpp',
        #'../third_party/skia/src/images/SkImageDecoder_FactoryRegistrar.cpp',
        #'../third_party/skia/src/images/SkImageDecoder_fpdfemb.cpp',
        #'../third_party/skia/src/images/SkImageDecoder_libbmp.cpp',
        #'../third_party/skia/src/images/SkImageDecoder_libgif.cpp',
        #'../third_party/skia/src/images/SkImageDecoder_libico.cpp',
        #'../third_party/skia/src/images/SkImageDecoder_libjpeg.cpp',
        #'../third_party/skia/src/images/SkImageDecoder_libpng.cpp',
        #'../third_party/skia/src/images/SkImageDecoder_libpvjpeg.cpp',
        #'../third_party/skia/src/images/SkImageDecoder_wbmp.cpp',
        #'../third_party/skia/src/images/SkImageEncoder.cpp',
        #'../third_party/skia/src/images/SkImageEncoder_Factory.cpp',
        #'../third_party/skia/src/images/SkImageRef.cpp',
        #'../third_party/skia/src/images/SkImageRefPool.cpp',
        #'../third_party/skia/src/images/SkImageRefPool.h',
        #'../third_party/skia/src/images/SkImageRef_GlobalPool.cpp',
        #'../third_party/skia/src/images/SkMovie.cpp',
        #'../third_party/skia/src/images/SkMovie_gif.cpp',
        '../third_party/skia/src/images/SkScaledBitmapSampler.cpp',
        '../third_party/skia/src/images/SkScaledBitmapSampler.h',

        '../third_party/skia/src/opts/opts_check_SSE2.cpp',

        '../third_party/skia/src/pdf/SkPDFCatalog.cpp',
        '../third_party/skia/src/pdf/SkPDFCatalog.h',
        '../third_party/skia/src/pdf/SkPDFDevice.cpp',
        '../third_party/skia/src/pdf/SkPDFDocument.cpp',
        '../third_party/skia/src/pdf/SkPDFFont.cpp',
        '../third_party/skia/src/pdf/SkPDFFont.h',
        '../third_party/skia/src/pdf/SkPDFFormXObject.cpp',
        '../third_party/skia/src/pdf/SkPDFFormXObject.h',
        '../third_party/skia/src/pdf/SkPDFGraphicState.cpp',
        '../third_party/skia/src/pdf/SkPDFGraphicState.h',
        '../third_party/skia/src/pdf/SkPDFImage.cpp',
        '../third_party/skia/src/pdf/SkPDFImage.h',
        '../third_party/skia/src/pdf/SkPDFImageStream.cpp',
        '../third_party/skia/src/pdf/SkPDFImageStream.h',
        '../third_party/skia/src/pdf/SkPDFPage.cpp',
        '../third_party/skia/src/pdf/SkPDFPage.h',
        '../third_party/skia/src/pdf/SkPDFShader.cpp',
        '../third_party/skia/src/pdf/SkPDFShader.h',
        '../third_party/skia/src/pdf/SkPDFStream.cpp',
        '../third_party/skia/src/pdf/SkPDFStream.h',
        '../third_party/skia/src/pdf/SkPDFTypes.cpp',
        '../third_party/skia/src/pdf/SkPDFTypes.h',
        '../third_party/skia/src/pdf/SkPDFUtils.cpp',
        '../third_party/skia/src/pdf/SkPDFUtils.h',

        #'../third_party/skia/src/ports/SkPurgeableMemoryBlock_android.cpp',
        #'../third_party/skia/src/ports/SkPurgeableMemoryBlock_mac.cpp',
        '../third_party/skia/src/ports/SkPurgeableMemoryBlock_none.cpp',

        '../third_party/skia/src/ports/FontHostConfiguration_android.cpp',
        #'../third_party/skia/src/ports/SkFontHost_FONTPATH.cpp',
        '../third_party/skia/src/ports/SkFontHost_FreeType.cpp',
        '../third_party/skia/src/ports/SkFontHost_FreeType_common.cpp',
        '../third_party/skia/src/ports/SkFontHost_FreeType_common.h',
        '../third_party/skia/src/ports/SkFontHost_android.cpp',
        #'../third_party/skia/src/ports/SkFontHost_ascender.cpp',
        #'../third_party/skia/src/ports/SkFontHost_linux.cpp',
        '../third_party/skia/src/ports/SkFontHost_mac.cpp',
        #'../third_party/skia/src/ports/SkFontHost_none.cpp',
        '../third_party/skia/src/ports/SkFontHost_win.cpp',
        '../third_party/skia/src/ports/SkGlobalInitialization_chromium.cpp',
        #'../third_party/skia/src/ports/SkImageDecoder_CG.cpp',
        #'../third_party/skia/src/ports/SkImageDecoder_empty.cpp',
        #'../third_party/skia/src/ports/SkImageRef_ashmem.cpp',
        #'../third_party/skia/src/ports/SkImageRef_ashmem.h',
        #'../third_party/skia/src/ports/SkOSEvent_android.cpp',
        #'../third_party/skia/src/ports/SkOSEvent_dummy.cpp',
        '../third_party/skia/src/ports/SkOSFile_stdio.cpp',
        #'../third_party/skia/src/ports/SkThread_none.cpp',
        '../third_party/skia/src/ports/SkThread_pthread.cpp',
        '../third_party/skia/src/ports/SkThread_win.cpp',
        '../third_party/skia/src/ports/SkTime_Unix.cpp',
        #'../third_party/skia/src/ports/SkXMLParser_empty.cpp',
        #'../third_party/skia/src/ports/SkXMLParser_expat.cpp',
        #'../third_party/skia/src/ports/SkXMLParser_tinyxml.cpp',
        #'../third_party/skia/src/ports/SkXMLPullParser_expat.cpp',

        '../third_party/skia/src/sfnt/SkOTUtils.cpp',
        '../third_party/skia/src/sfnt/SkOTUtils.h',

        '../third_party/skia/include/utils/mac/SkCGUtils.h',
        '../third_party/skia/include/utils/SkDeferredCanvas.h',
        '../third_party/skia/include/utils/SkMatrix44.h',
        '../third_party/skia/src/utils/mac/SkCreateCGImageRef.cpp',
        '../third_party/skia/src/utils/SkBase64.cpp',
        '../third_party/skia/src/utils/SkBase64.h',
        '../third_party/skia/src/utils/SkBitSet.cpp',
        '../third_party/skia/src/utils/SkBitSet.h',
        '../third_party/skia/src/utils/SkDeferredCanvas.cpp',
        '../third_party/skia/src/utils/SkMatrix44.cpp',
        '../third_party/skia/src/utils/SkNullCanvas.cpp',
        '../third_party/skia/include/utils/SkNWayCanvas.h',
        '../third_party/skia/src/utils/SkNWayCanvas.cpp',
        '../third_party/skia/src/utils/SkPictureUtils.cpp',
        '../third_party/skia/src/utils/SkRTConf.cpp',
        '../third_party/skia/include/utils/SkRTConf.h',
        '../third_party/skia/include/pdf/SkPDFDevice.h',
        '../third_party/skia/include/pdf/SkPDFDocument.h',

        '../third_party/skia/include/ports/SkTypeface_win.h',

        #'../third_party/skia/include/images/SkImageDecoder.h',
        #'../third_party/skia/include/images/SkImageEncoder.h',
        '../third_party/skia/include/images/SkImageRef.h',
        '../third_party/skia/include/images/SkImageRef_GlobalPool.h',
        '../third_party/skia/include/images/SkMovie.h',
        '../third_party/skia/include/images/SkPageFlipper.h',

        '../third_party/skia/include/utils/SkNullCanvas.h',
        '../third_party/skia/include/utils/SkPictureUtils.h',
        'ext/analysis_canvas.cc',
        'ext/analysis_canvas.h',
        'ext/bitmap_platform_device.h',
        'ext/bitmap_platform_device_android.cc',
        'ext/bitmap_platform_device_android.h',
        'ext/bitmap_platform_device_data.h',
        'ext/bitmap_platform_device_linux.cc',
        'ext/bitmap_platform_device_linux.h',
        'ext/bitmap_platform_device_mac.cc',
        'ext/bitmap_platform_device_mac.h',
        'ext/bitmap_platform_device_win.cc',
        'ext/bitmap_platform_device_win.h',
        'ext/convolver.cc',
        'ext/convolver.h',
        'ext/google_logging.cc',
        'ext/image_operations.cc',
        'ext/image_operations.h',
        'ext/lazy_pixel_ref.cc',
        'ext/lazy_pixel_ref.h',
        'ext/SkThread_chrome.cc',
        'ext/paint_simplifier.cc',
        'ext/paint_simplifier.h',
        'ext/platform_canvas.cc',
        'ext/platform_canvas.h',
        'ext/platform_device.cc',
        'ext/platform_device.h',
        'ext/platform_device_linux.cc',
        'ext/platform_device_mac.cc',
        'ext/platform_device_win.cc',
        'ext/refptr.h',
        'ext/SkMemory_new_handler.cpp',
        'ext/skia_trace_shim.h',
        'ext/skia_utils_base.cc',
        'ext/skia_utils_base.h',
        'ext/skia_utils_ios.mm',
        'ext/skia_utils_ios.h',
        'ext/skia_utils_mac.mm',
        'ext/skia_utils_mac.h',
        'ext/skia_utils_win.cc',
        'ext/skia_utils_win.h',
        'ext/vector_canvas.cc',
        'ext/vector_canvas.h',
        'ext/vector_platform_device_emf_win.cc',
        'ext/vector_platform_device_emf_win.h',
        'ext/vector_platform_device_skia.cc',
        'ext/vector_platform_device_skia.h',
      ],
      'include_dirs': [
        '..',
        'config',
        '../third_party/skia/include/config',
        '../third_party/skia/include/core',
        '../third_party/skia/include/effects',
        '../third_party/skia/include/images',
        '../third_party/skia/include/lazy',
        '../third_party/skia/include/pathops',
        '../third_party/skia/include/pdf',
        '../third_party/skia/include/pipe',
        '../third_party/skia/include/ports',
        '../third_party/skia/include/utils',
        '../third_party/skia/src/core',
        '../third_party/skia/src/image',
        '../third_party/skia/src/sfnt',
        '../third_party/skia/src/utils',
        '../third_party/skia/src/lazy',
      ],
      'msvs_disabled_warnings': [4244, 4267, 4341, 4345, 4390, 4554, 4748, 4800],
      'defines': [
        #'SK_GAMMA_SRGB',
        #'SK_GAMMA_APPLY_TO_A8',
        'SK_BUILD_NO_IMAGE_ENCODE',
        'GR_GL_CUSTOM_SETUP_HEADER="GrGLConfig_chrome.h"',
        'GR_STATIC_RECT_VB=1',
        'GR_AGGRESSIVE_SHADER_OPTS=1',
        'SK_DEFERRED_CANVAS_USES_GPIPE=1',
        'SK_ENABLE_INST_COUNT=0',

        # this flag can be removed entirely once this has baked for a while
        'SK_ALLOW_OVER_32K_BITMAPS',

        # skia uses static initializers to initialize the serialization logic
        # of its "pictures" library. This is currently not used in chrome; if
        # it ever gets used the processes that use it need to call
        # SkGraphics::Init().
        'SK_ALLOW_STATIC_GLOBAL_INITIALIZERS=0',

        # Disable this check because it is too strict for some Chromium-specific
        # subclasses of SkPixelRef. See bug: crbug.com/171776.
        'SK_DISABLE_PIXELREF_LOCKCOUNT_BALANCE_CHECK',

        'IGNORE_ROT_AA_RECT_OPT',
      ],
      'sources!': [
        '../third_party/skia/include/core/SkTypes.h',
      ],
      'conditions': [
        ['skia_support_gpu != 0', {
          'includes': [
            '../third_party/skia/gyp/gpu.gypi',
          ],
          'sources': [
            '<@(gr_sources)',
            '<@(skgr_sources)',
          ],
          'include_dirs': [
            '../third_party/skia/include/gpu',
            '../third_party/skia/include/gpu/gl',
            '../third_party/skia/src/gpu',
          ],
        }, {  # skia_support_gpu == 0
          'defines': [
            'SK_SUPPORT_GPU=0',
          ],
        }],
        ['release_valgrind_build == 1', {
          'defines': [
            'SK_DEBUG_PATH_REF=1',
          ],
          'direct_dependent_settings': {
            'defines': [
              'SK_DEBUG_PATH_REF=1',
            ],
          },
        }],
        #Settings for text blitting, chosen to approximate the system browser.
        [ 'OS == "linux"', {
          'defines': [
            'SK_GAMMA_EXPONENT=1.2',
            'SK_GAMMA_CONTRAST=0.2',
          ],
        }],
        ['OS == "android"', {
          'defines': [
            'SK_GAMMA_APPLY_TO_A8',
            'SK_GAMMA_EXPONENT=1.4',
            'SK_GAMMA_CONTRAST=0.0',
          ],
        }],
        ['OS == "win"', {
          'defines': [
            'SK_GAMMA_SRGB',
            'SK_GAMMA_CONTRAST=0.5',
          ],
        }],
        ['OS == "mac"', {
          'defines': [
            'SK_GAMMA_SRGB',
            'SK_GAMMA_CONTRAST=0.0',
          ],
        }],

        # For POSIX platforms, prefer the Mutex implementation provided by Skia
        # since it does not generate static initializers.
        [ 'OS == "android" or OS == "linux" or OS == "mac" or OS == "ios"', {
          'defines+': [
            'SK_USE_POSIX_THREADS',
          ],
          'direct_dependent_settings': {
            'defines': [
              'SK_USE_POSIX_THREADS',
            ],
          },
          'sources!': [
            'ext/SkThread_chrome.cc',
          ],
        }],
        [ 'OS != "android"', {
          'sources/': [
            ['exclude', '_android\\.(cc|cpp)$'],
          ],
          'defines': [
            'SK_DEFAULT_FONT_CACHE_LIMIT=(20*1024*1024)',
          ],
        }],
        [ 'OS != "ios"', {
          'sources/': [
            ['exclude', '_ios\\.(cc|cpp|mm?)$'],
          ],
          'dependencies': [
            '<(webkit_src_dir)/Source/WebKit/chromium/skia_webkit.gyp:skia_webkit',
          ],
        }],
        [ 'OS != "mac"', {
          'sources/': [
            ['exclude', '_mac\\.(cc|cpp|mm?)$'],
            ['exclude', '/mac/']
          ],
        }],
        [ 'OS != "win"', {
          'sources/': [ ['exclude', '_win\\.(cc|cpp)$'] ],
        }],
        [ 'target_arch == "arm" and arm_version >= 7 and arm_neon == 1', {
          'defines': [
            '__ARM_HAVE_NEON',
          ],
        }],
        [ 'target_arch == "arm" or target_arch == "mipsel"', {
          'sources!': [
            '../third_party/skia/src/opts/opts_check_SSE2.cpp'
          ],
        }],
        [ 'use_glib == 1', {
          'dependencies': [
            '../build/linux/system.gyp:fontconfig',
            '../build/linux/system.gyp:freetype2',
            '../build/linux/system.gyp:pangocairo',
            '../third_party/icu/icu.gyp:icuuc',
          ],
          'cflags': [
            '-Wno-unused',
            '-Wno-unused-function',
          ],
          'sources': [
            '../third_party/skia/src/ports/SkFontHost_fontconfig.cpp',
            '../third_party/skia/src/ports/SkFontConfigInterface_direct.cpp',
          ],
          'defines': [
#            'SK_USE_COLOR_LUMINANCE',
          ],
        }],
        [ 'use_glib == 0 and OS != "android"', {
          'sources/': [ ['exclude', '_linux\\.(cc|cpp)$'] ],
          'sources!': [
            '../third_party/skia/src/ports/SkFontHost_FreeType.cpp',
            '../third_party/skia/src/ports/SkFontHost_FreeType_common.cpp',
          ],
        }],
        [ 'toolkit_uses_gtk == 1', {
          'dependencies': [
            '../build/linux/system.gyp:gdk',
          ],
        }, {  # toolkit_uses_gtk == 0
          'sources/': [ ['exclude', '_gtk\\.(cc|cpp)$'] ],
        }],
        [ 'OS == "android"', {
          'sources/': [
            ['exclude', '_linux\\.(cc|cpp)$'],
          ],
          'conditions': [
            [ '_toolset == "target"', {
              'defines': [
                'HAVE_PTHREADS',
                'OS_ANDROID',
                'SK_BUILD_FOR_ANDROID_NDK',
                # Android devices are typically more memory constrained, so
                # use a smaller glyph cache.
                'SK_DEFAULT_FONT_CACHE_LIMIT=(8*1024*1024)',
                'USE_CHROMIUM_SKIA',
              ],
              'dependencies': [
                '../third_party/expat/expat.gyp:expat',
                '../third_party/freetype/freetype.gyp:ft2',
                'skia_opts'
              ],
              'dependencies!': [
                # Android doesn't use Skia's PDF generation, which is what uses
                # sfntly.
                '../third_party/sfntly/sfntly.gyp:sfntly',
              ],
              # This exports a hard dependency because it needs to run its
              # symlink action in order to expose the skia header files.
              'hard_dependency': 1,
              'include_dirs': [
                '../third_party/expat/files/lib',
              ],
              'sources/': [
                ['include', 'ext/platform_device_linux\\.cc$'],
                ['exclude', '../third_party/skia/src/pdf/'],
              ],
              'sources!': [
                'ext/vector_platform_device_skia.cc',
              ],
            }],
            [ '_toolset == "target" and android_webview_build == 0', {
              'defines': [
                'HAVE_ENDIAN_H',
              ],
            }],
            [ '_toolset=="host" and host_os=="linux"', {
              'sources': [
                'ext/platform_device_linux.cc',
              ],
            }],
          ],
        }],
        [ 'OS == "ios"', {
          'defines': [
            'SK_BUILD_FOR_IOS',
            'SK_USE_MAC_CORE_TEXT',
          ],
          'include_dirs': [
            '../third_party/skia/include/utils/ios',
            '../third_party/skia/include/utils/mac',
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/ImageIO.framework',
            ],
          },
          'dependencies': [
            'skia_opts_ios',
          ],
          'dependencies!': [
            'skia_opts',
            '../third_party/sfntly/sfntly.gyp:sfntly',
          ],
          'sources': [
            # This file is used on both iOS and Mac, so it should be removed
            #  from the ios and mac conditions and moved into the main sources
            #  list.
            '../third_party/skia/src/utils/mac/SkStream_mac.cpp',
          ],
          'sources/': [
            ['exclude', '/pdf/'],
            ['exclude', '^ext/vector_platform_device_skia\\.'],
            ['exclude', 'opts_check_SSE2\\.cpp$'],
          ],
        }],
        [ 'OS == "mac"', {
          'defines': [
            'SK_BUILD_FOR_MAC',
            'SK_USE_MAC_CORE_TEXT',
#           'SK_USE_COLOR_LUMINANCE',
          ],
          'include_dirs': [
            '../third_party/skia/include/utils/mac',
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/AppKit.framework',
            ],
          },
          'sources': [
            '../third_party/skia/src/utils/mac/SkStream_mac.cpp',
          ],
        }],
        [ 'OS == "win"', {
          'sources!': [
            '../third_party/skia/src/core/SkMMapStream.cpp',
            '../third_party/skia/src/ports/SkThread_pthread.cpp',
            '../third_party/skia/src/ports/SkTime_Unix.cpp',
            'ext/SkThread_chrome.cc',
          ],
          'include_dirs': [
            'config/win',
          ],
          'direct_dependent_settings': {
            'include_dirs': [
              'config/win',
            ],
          },
        }],
        ['component=="shared_library"', {
          'defines': [
            'GR_DLL=1',
            'GR_IMPLEMENTATION=1',
            'SKIA_DLL',
            'SKIA_IMPLEMENTATION=1',
          ],
          'dependencies': [
            '../base/base.gyp:base',
          ],
          'direct_dependent_settings': {
            'defines': [
              'GR_DLL',
              'SKIA_DLL',
            ],
          },
        }],
        # TODO(scottmg): http://crbug.com/177306
        ['clang==1', {
          'xcode_settings': {
            'WARNING_CFLAGS!': [
              # Don't warn about string->bool used in asserts.
              '-Wstring-conversion',
            ],
          },
          'cflags!': [
            '-Wstring-conversion',
          ],
        }],
      ],
      'dependencies': [
        'skia_opts',
        '../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '../third_party/sfntly/sfntly.gyp:sfntly',
        '../third_party/zlib/zlib.gyp:zlib',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'config',

          #temporary until we can hide SkFontHost
          '../third_party/skia/src/core',

          '../third_party/skia/include/config',
          '../third_party/skia/include/core',
          '../third_party/skia/include/effects',
          '../third_party/skia/include/pdf',
          '../third_party/skia/include/gpu',
          '../third_party/skia/include/gpu/gl',
          '../third_party/skia/include/pathops',
          '../third_party/skia/include/pipe',
          '../third_party/skia/include/ports',
          '../third_party/skia/include/utils',
          'ext',
        ],
        'defines': [
          'SK_BUILD_NO_IMAGE_ENCODE',
          'SK_DEFERRED_CANVAS_USES_GPIPE=1',
          'GR_GL_CUSTOM_SETUP_HEADER="GrGLConfig_chrome.h"',
          'GR_AGGRESSIVE_SHADER_OPTS=1',
          'SK_ENABLE_INST_COUNT=0',
        ],
        'conditions': [
          ['OS=="android"', {
            'dependencies!': [
              'skia_opts',
              '../third_party/zlib/zlib.gyp:zlib',
            ],
            'defines': [
              # Don't use non-NDK available stuff.
              'SK_BUILD_FOR_ANDROID_NDK',
            ],
            'conditions': [
              [ '_toolset == "target" and android_webview_build == 0', {
                'defines': [
                  'HAVE_ENDIAN_H',
                ],
              }],
            ],
          }],
          ['OS=="mac"', {
            'include_dirs': [
              '../third_party/skia/include/utils/mac',
            ],
          }],
        ],
      },
      'target_conditions': [
        # Pull in specific Mac files for iOS (which have been filtered out
        # by file name rules).
        [ 'OS == "ios"', {
          'sources/': [
            ['include', 'SkFontHost_mac\\.cpp$',],
            ['include', 'SkStream_mac\\.cpp$',],
            ['include', 'SkCreateCGImageRef\\.cpp$',],
          ],
        }],
      ],
    },

    # Due to an unfortunate intersection of lameness between gcc and gyp,
    # we have to build the *_SSE2.cpp files in a separate target.  The
    # gcc lameness is that, in order to compile SSE2 intrinsics code, it
    # must be passed the -msse2 flag.  However, with this flag, it may
    # emit SSE2 instructions even for scalar code, such as the CPUID
    # test used to test for the presence of SSE2.  So that, and all other
    # code must be compiled *without* -msse2.  The gyp lameness is that it
    # does not allow file-specific CFLAGS, so we must create this extra
    # target for those files to be compiled with -msse2.
    #
    # This is actually only a problem on 32-bit Linux (all Intel Macs have
    # SSE2, Linux x86_64 has SSE2 by definition, and MSC will happily emit
    # SSE2 from instrinsics, which generating plain ol' 386 for everything
    # else).  However, to keep the .gyp file simple and avoid platform-specific
    # build breakage, we do this on all platforms.

    # For about the same reason, we need to compile the ARM opts files
    # separately as well.
    {
      'target_name': 'skia_opts',
      'type': 'static_library',
      'variables': {
        'optimize': 'max',
      },
      'include_dirs': [
        '..',
        'config',
        '../third_party/skia/include/config',
        '../third_party/skia/include/core',
        '../third_party/skia/include/effects',
        '../third_party/skia/include/images',
        '../third_party/skia/include/lazy',
        '../third_party/skia/include/pathops',
        '../third_party/skia/include/utils',
        '../third_party/skia/src/core',
      ],
      'conditions': [
        [ 'os_posix == 1 and OS != "mac" and OS != "android" and \
           target_arch != "arm" and target_arch != "mipsel"', {
          'cflags': [
            '-msse2',
          ],
        }],
        [ 'OS == "android"', {
          'defines': [
            'SK_BUILD_FOR_ANDROID_NDK',
          ],
        }],
        [ 'target_arch != "arm" and target_arch != "mipsel"', {
          'sources': [
            '../third_party/skia/src/opts/SkBitmapProcState_opts_SSE2.cpp',
            '../third_party/skia/src/opts/SkBlitRect_opts_SSE2.cpp',
            '../third_party/skia/src/opts/SkBlitRow_opts_SSE2.cpp',
            '../third_party/skia/src/opts/SkUtils_opts_SSE2.cpp',
            'ext/convolver_SSE2.cc',
          ],
          'conditions': [
            # x86 Android doesn't support SSSE3 instructions.
            [ 'OS != "android"', {
              'dependencies': [
                'skia_opts_ssse3',
              ],
            }],
          ],
        }],
        [ 'target_arch == "arm"', {
          'conditions': [
            [ 'arm_version >= 7 and arm_neon == 1', {
              'defines': [
                '__ARM_HAVE_NEON',
              ],
              'cflags': [
                # The neon assembly contains conditional instructions which
                # aren't enclosed in an IT block. The assembler complains
                # without this option.
                # See #86592.
                '-Wa,-mimplicit-it=always',
              ],
           }],
          ],
          # The assembly uses the frame pointer register (r7 in Thumb/r11 in
          # ARM), the compiler doesn't like that. Explicitly remove the
          # -fno-omit-frame-pointer flag for Android, as that gets added to all
          # targets via common.gypi.
          'cflags!': [
            '-fno-omit-frame-pointer',
            '-marm',
            '-mapcs-frame',
          ],
          'cflags': [
            '-fomit-frame-pointer',
          ],
          'sources': [
            '../third_party/skia/src/opts/SkBitmapProcState_opts_arm.cpp',
          ],
        }],
        [ 'target_arch == "arm" and (arm_version < 7 or arm_neon == 0)', {
          'sources': [
            '../third_party/skia/src/opts/memset.arm.S',
        ],
        }],
        [ 'target_arch == "arm" and arm_version >= 7 and arm_neon == 1', {
          'sources': [
            '../third_party/skia/src/opts/memset16_neon.S',
            '../third_party/skia/src/opts/memset32_neon.S',
            '../third_party/skia/src/opts/SkBitmapProcState_arm_neon.cpp',
            '../third_party/skia/src/opts/SkBitmapProcState_matrixProcs_neon.cpp',
            '../third_party/skia/src/opts/SkBitmapProcState_matrix_clamp_neon.h',
            '../third_party/skia/src/opts/SkBitmapProcState_matrix_repeat_neon.h',
            '../third_party/skia/src/opts/SkBlitRow_opts_arm_neon.cpp',
          ],
        }],
        [ 'target_arch == "arm" and arm_version < 6', {
          'sources': [
            '../third_party/skia/src/opts/SkBlitRow_opts_none.cpp',
            '../third_party/skia/src/opts/SkUtils_opts_none.cpp',
          ],
        }],
        [ 'target_arch == "arm" and arm_version >= 6', {
          'sources': [
            '../third_party/skia/src/opts/SkBlitRow_opts_arm.cpp',
            '../third_party/skia/src/opts/SkBlitRow_opts_arm.h',
            '../third_party/skia/src/opts/opts_check_arm.cpp',
          ],
        }],
        [ 'target_arch == "mipsel"',{
          'cflags': [
            '-fomit-frame-pointer',
          ],
          'sources': [
            '../third_party/skia/src/opts/SkBitmapProcState_opts_none.cpp',
            '../third_party/skia/src/opts/SkBlitRow_opts_none.cpp',
            '../third_party/skia/src/opts/SkUtils_opts_none.cpp',
          ],
        }],
      ],
    },
    # For the same lame reasons as what is done for skia_opts, we have to
    # create another target specifically for SSSE3 code as we would not want
    # to compile the SSE2 code with -mssse3 which would potentially allow
    # gcc to generate SSSE3 code.
    {
      'target_name': 'skia_opts_ssse3',
      'type': 'static_library',
      'variables': {
        'optimize': 'max',
      },
      'include_dirs': [
        '..',
        'config',
        '../third_party/skia/include/config',
        '../third_party/skia/include/core',
        '../third_party/skia/include/pathops',
        '../third_party/skia/src/core',
      ],
      'conditions': [
        [ 'OS in ["linux", "freebsd", "openbsd", "solaris"]', {
          'cflags': [
            '-mssse3',
          ],
        }],
        [ 'OS == "mac"', {
          'xcode_settings': {
            'GCC_ENABLE_SUPPLEMENTAL_SSE3_INSTRUCTIONS': 'YES',
          },
        }],
        [ 'OS == "win"', {
          'include_dirs': [
            'config/win',
          ],
          'direct_dependent_settings': {
            'include_dirs': [
              'config/win',
            ],
          },
        }],
        [ 'target_arch != "arm"', {
          'sources': [
            '../third_party/skia/src/opts/SkBitmapProcState_opts_SSSE3.cpp',
          ],
        }],
      ],
    },
    {
      'target_name': 'image_operations_bench',
      'type': 'executable',
      'dependencies': [
        '../base/base.gyp:base',
        'skia',
      ],
      'include_dirs': [
        '..',
      ],
      'sources': [
        'ext/image_operations_bench.cc',
      ],
    },
  ],
  'conditions': [
    ['OS=="ios"', {
      'targets': [
        # The main skia_opts target does not currently work on iOS because the
        # target architecture on iOS is determined at compile time rather than
        # gyp time (simulator builds are x86, device builds are arm).  As a
        # temporary measure, this is a separate opts target for iOS-only, using
        # the _none.cpp files to avoid architecture-dependent implementations.
        {
          'target_name': 'skia_opts_ios',
          'type': 'static_library',
          'include_dirs': [
            '..',
            'config',
            '../third_party/skia/include/config',
            '../third_party/skia/include/core',
            '../third_party/skia/include/effects',
            '../third_party/skia/include/images',
            '../third_party/skia/include/lazy',
            '../third_party/skia/include/pathops',
            '../third_party/skia/include/utils',
            '../third_party/skia/src/core',
          ],
          'sources': [
            '../third_party/skia/src/opts/SkBitmapProcState_opts_none.cpp',
            '../third_party/skia/src/opts/SkBlitRow_opts_none.cpp',
            '../third_party/skia/src/opts/SkUtils_opts_none.cpp',
          ],
        },
      ],
    }],
  ],
}
