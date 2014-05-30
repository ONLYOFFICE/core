# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'ui_test_support',
      'type': 'static_library',
      'dependencies': [
        '../base/base.gyp:base',
        '../skia/skia.gyp:skia',
        '../testing/gtest.gyp:gtest',
      ],
      'sources': [
        'base/test/cocoa_test_event_utils.h',
        'base/test/cocoa_test_event_utils.mm',
        'base/test/ui_cocoa_test_helper.h',
        'base/test/ui_cocoa_test_helper.mm',
        'base/test/dummy_input_method.cc',
        'base/test/dummy_input_method.h',
      ],
      'include_dirs': [
        '../',
      ],
      'conditions': [
        ['OS!="ios"', {
          'includes': [ 'base/ime/ime_test_support.gypi' ],
        }, {  # OS=="ios"
          # The cocoa files don't apply to iOS.
          'sources/': [['exclude', 'cocoa']],
        }],
        ['chromeos==1', {
          'dependencies': [
            '../chromeos/chromeos.gyp:chromeos_test_support_without_gmock',
            '../skia/skia.gyp:skia',
          ],
        }],
      ],
    },
    {
      'target_name': 'run_ui_unittests',
      'type': 'static_library',
      'dependencies': [
        '../base/base.gyp:base',
        '../base/base.gyp:test_support_base',
        'ui',
      ],
      'sources': [
        'test/test_suite.cc',
        'test/test_suite.h',
        'test/run_all_unittests.cc',
      ],
    },
    {
      'target_name': 'ui_unittests',
      'type': '<(gtest_target_type)',
      'dependencies': [
        '../base/base.gyp:base',
        '../base/base.gyp:test_support_base',
        '../build/temp_gyp/googleurl.gyp:googleurl',
        '../skia/skia.gyp:skia',
        '../testing/gmock.gyp:gmock',
        '../testing/gtest.gyp:gtest',
        '../third_party/icu/icu.gyp:icui18n',
        '../third_party/icu/icu.gyp:icuuc',
        '../third_party/libpng/libpng.gyp:libpng',
        'run_ui_unittests',
        'shell_dialogs',
        'ui',
        'ui_resources',
        'ui_test_support',
      ],
      # iOS uses a small subset of ui. common_sources are the only files that
      # are built on iOS.
      'common_sources' : [
        'base/animation/animation_container_unittest.cc',
        'base/animation/animation_unittest.cc',
        'base/animation/multi_animation_unittest.cc',
        'base/animation/slide_animation_unittest.cc',
        'base/layout_unittest.cc',
        'base/l10n/l10n_util_mac_unittest.mm',
        'base/l10n/l10n_util_unittest.cc',
        'base/l10n/l10n_util_win_unittest.cc',
        'base/models/tree_node_iterator_unittest.cc',
        'base/range/range_mac_unittest.mm',
        'base/range/range_unittest.cc',
        'base/range/range_win_unittest.cc',
        'base/resource/data_pack_literal.cc',
        'base/resource/data_pack_unittest.cc',
        'base/resource/resource_bundle_unittest.cc',
        'base/text/text_elider_unittest.cc',
        'gfx/codec/png_codec_unittest.cc',
        'gfx/color_utils_unittest.cc',
        'gfx/display_unittest.cc',
        'gfx/font_unittest.cc',
        'gfx/image/image_family_unittest.cc',
        'gfx/image/image_skia_unittest.cc',
        'gfx/image/image_unittest.cc',
        'gfx/image/image_unittest_util.cc',
        'gfx/image/image_unittest_util.h',
        'gfx/image/image_unittest_util_ios.mm',
        'gfx/image/image_unittest_util_mac.mm',
        'gfx/insets_unittest.cc',
        'gfx/matrix3_unittest.cc',
        'gfx/point_unittest.cc',
        'gfx/point3_unittest.cc',
        'gfx/quad_unittest.cc',
        'gfx/rect_unittest.cc',
        'gfx/safe_integer_conversions_unittest.cc',
        'gfx/screen_unittest.cc',
        'gfx/shadow_value_unittest.cc',
        'gfx/size_unittest.cc',
        'gfx/skbitmap_operations_unittest.cc',
        'gfx/text_utils_unittest.cc',
        'gfx/vector2d_unittest.cc',
        'gfx/vector3d_unittest.cc',
      ],
      'all_sources': [
        '<@(_common_sources)',
        'base/accelerators/accelerator_manager_unittest.cc',
        'base/clipboard/clipboard_unittest.cc',
        'base/clipboard/custom_data_helper_unittest.cc',
        'base/cocoa/base_view_unittest.mm',
        'base/cocoa/events_mac_unittest.mm',
        'base/cocoa/focus_tracker_unittest.mm',
        'base/cocoa/fullscreen_window_manager_unittest.mm',
        'base/cocoa/hover_image_button_unittest.mm',
        'base/cocoa/tracking_area_unittest.mm',
        'base/events/event_dispatcher_unittest.cc',
        'base/events/event_unittest.cc',
        'base/events/key_identifier_conversion_unittest.cc',
        'base/gtk/gtk_expanded_container_unittest.cc',
        'base/gtk/gtk_im_context_util_unittest.cc',
        'base/gtk/menu_label_accelerator_util_unittest.cc',
        'base/keycodes/usb_keycode_map_unittest.cc',
        'base/models/list_model_unittest.cc',
        'base/models/list_selection_model_unittest.cc',
        'base/models/tree_node_model_unittest.cc',
        'base/test/data/resource.h',
        'base/text/bytes_formatting_unittest.cc',
        'base/text/utf16_indexing_unittest.cc',
        'base/view_prop_unittest.cc',
        'gfx/blit_unittest.cc',
        'gfx/break_list_unittest.cc',
        'gfx/canvas_unittest.cc',
        'gfx/codec/jpeg_codec_unittest.cc',
        'gfx/color_analysis_unittest.cc',
        'gfx/font_list_unittest.cc',
        'gfx/image/image_mac_unittest.mm',
        'gfx/image/image_util_unittest.cc',
        'gfx/platform_font_mac_unittest.mm',
        'gfx/render_text_unittest.cc',
        'gfx/transform_util_unittest.cc',
        'shell_dialogs/select_file_dialog_win_unittest.cc',
        'webui/web_ui_util_unittest.cc',
      ],
      'include_dirs': [
        '../',
      ],
      'conditions': [
        ['OS!="ios"', {
          'sources' : ['<@(_all_sources)'],
          'includes': [
            'base/ime/ime_unittests.gypi',
          ],
        }, {  # OS=="ios"
          'sources' : [
            '<@(_common_sources)',
          ],
          # The ResourceBundle unittest expects a locale.pak file to exist in
          # the bundle for English-US. Copy it in from where it was generated
          # by ui_strings.gyp:ui_unittest_strings.
          'mac_bundle_resources': [
            '<(PRODUCT_DIR)/ui_unittests_strings/en.lproj/locale.pak',
          ],
        }],
        ['OS == "win"', {
          'sources': [
            'test/ui_unittests.rc',
            'base/dragdrop/os_exchange_data_win_unittest.cc',
            'base/win/hwnd_subclass_unittest.cc',
            'gfx/font_fallback_win_unittest.cc',
            'gfx/icon_util_unittest.cc',
            'gfx/platform_font_win_unittest.cc',
          ],
          'include_dirs': [
            '../..',
            '../third_party/wtl/include',
          ],
          'msvs_settings': {
            'VCLinkerTool': {
              'DelayLoadDLLs': [
                'd2d1.dll',
                'd3d10_1.dll',
              ],
              'AdditionalDependencies': [
                'd2d1.lib',
                'd3d10_1.lib',
              ],
            },
          },
          'link_settings': {
            'libraries': [
              '-limm32.lib',
              '-loleacc.lib',
            ],
          },
          # TODO(jschuh): crbug.com/167187 fix size_t to int truncations.
          'msvs_disabled_warnings': [ 4267, ],
        }],
        ['OS == "linux"', {
          'sources': [
            'base/x/x11_util_unittest.cc',
            'gfx/platform_font_pango_unittest.cc',
          ],
        }],
        ['OS == "linux" and toolkit_views==1', {
          'sources': [
            'base/x/events_x_unittest.cc',
          ],
        }],
        ['OS != "mac" and OS != "ios"', {
          'sources': [
            'gfx/transform_unittest.cc',
            'gfx/interpolated_transform_unittest.cc',
          ],
        }],
        ['OS == "android" and gtest_target_type == "shared_library"', {
          'dependencies': [
            '../testing/android/native_test.gyp:native_test_native_code',
          ],
        }],
        ['use_glib == 1 or OS == "ios"', {
          'dependencies': [
            'base/strings/ui_strings.gyp:ui_unittest_strings',
          ],
        }],
        ['use_glib == 1', {
          'dependencies': [
            '../build/linux/system.gyp:pangocairo',
          ],
          'conditions': [
            ['linux_use_tcmalloc==1', {
               'dependencies': [
                 '../base/allocator/allocator.gyp:allocator',
               ],
            }],
            ['toolkit_views==1', {
              'sources!': [
                'browser/ui/gtk/gtk_expanded_container_unittest.cc',
              ],
            }],
          ],
        }],
        ['use_x11==1', {
          'dependencies': [
            '../tools/xdisplaycheck/xdisplaycheck.gyp:xdisplaycheck',
          ],
        }],
        ['toolkit_uses_gtk == 1', {
          'sources': [
            'base/dragdrop/gtk_dnd_util_unittest.cc',
          ],
          'dependencies': [
            '../build/linux/system.gyp:gtk',
          ],
        }],
        ['OS=="android" or OS=="ios"', {
          'sources!': [
            'gfx/render_text_unittest.cc',
          ],
        }],
        ['OS!="win" or use_aura==0', {
          'sources!': [
            'base/view_prop_unittest.cc',
          ],
        }],
        ['use_aura==1 or toolkit_views==1',  {
          'sources': [
            'base/gestures/velocity_calculator_unittest.cc',
          ],
        }, {
          'sources!': [
            'base/events/event_dispatcher_unittest.cc',
            'base/events/event_unittest.cc',
            'base/events/key_identifier_conversion_unittest.cc',
          ],
        }],
        ['use_aura==1', {
          'sources!': [
            'base/dialogs/select_file_dialog_win_unittest.cc',
            'base/dragdrop/os_exchange_data_win_unittest.cc',
            'gfx/screen_unittest.cc',
          ],
        }],
      ],
      'target_conditions': [
        ['OS == "ios"', {
          'sources/': [
            # Pull in specific Mac files for iOS (which have been filtered out
            # by file name rules).
            ['include', '^base/l10n/l10n_util_mac_unittest\\.mm$'],
          ],
        }],
      ],
    },
  ],
  'conditions': [
    # Special target to wrap a gtest_target_type==shared_library
    # ui_unittests into an android apk for execution.
    # See base.gyp for TODO(jrg)s about this strategy.
    ['OS == "android" and gtest_target_type == "shared_library"', {
      'targets': [
        {
          'target_name': 'ui_unittests_apk',
          'type': 'none',
          'dependencies': [
            'ui_unittests',
          ],
          'variables': {
            'test_suite_name': 'ui_unittests',
            'input_shlib_path': '<(SHARED_LIB_DIR)/<(SHARED_LIB_PREFIX)ui_unittests<(SHARED_LIB_SUFFIX)',
          },
          'includes': [ '../build/apk_test.gypi' ],
        },
      ],
    }],
  ],
}
