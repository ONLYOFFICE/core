# Copyright (c) 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'keyboard_resources',
      'type': 'none',
      'variables': {
        'grit_out_dir': '<(SHARED_INTERMEDIATE_DIR)/ui/keyboard',
      },
      'actions': [
        {
          'action_name': 'keyboard_resources',
          'variables': {
            'grit_grd_file': 'keyboard_resources.grd',
          },
          'includes': [ '../../build/grit_action.gypi' ],
        },
      ],
      'includes': [ '../../build/grit_target.gypi' ],
      'copies': [
        {
          'destination': '<(PRODUCT_DIR)',
          'files': [
            '<(SHARED_INTERMEDIATE_DIR)/ui/keyboard/keyboard_resources.pak',
          ],
        },
      ],
    },
    {
      'target_name': 'keyboard',
      'type': '<(component)',
      'dependencies': [
        '../../base/base.gyp:base',
        '../../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '../../build/temp_gyp/googleurl.gyp:googleurl',
        '../../content/content.gyp:content_browser',
        '../../ipc/ipc.gyp:ipc',
        '../../skia/skia.gyp:skia',
        '../aura/aura.gyp:aura',
        '../compositor/compositor.gyp:compositor',
        '../ui.gyp:ui',
        'keyboard_resources',
      ],
      'defines': [
        'KEYBOARD_IMPLEMENTATION',
      ],
      'sources': [
        'keyboard.cc',
        'keyboard.h',
        'keyboard_constants.cc',
        'keyboard_constants.h',
        'keyboard_controller.cc',
        'keyboard_controller.h',
        'keyboard_controller_observer.h',
        'keyboard_controller_proxy.cc',
        'keyboard_controller_proxy.h',
        'keyboard_export.h',
        'keyboard_switches.cc',
        'keyboard_switches.h',
        'keyboard_ui_controller.cc',
        'keyboard_ui_controller.h',
        'keyboard_ui_handler.cc',
        'keyboard_ui_handler.h',
        'keyboard_util.cc',
        'keyboard_util.h',
      ]
    },
    {
      'target_name': 'keyboard_unittests',
      'type': '<(gtest_target_type)',
      'dependencies': [
        '../../base/base.gyp:base',
        '../../base/base.gyp:test_support_base',
        '../../content/content.gyp:content_browser',
        '../../skia/skia.gyp:skia',
        '../../testing/gtest.gyp:gtest',
        '../aura/aura.gyp:aura',
        '../aura/aura.gyp:aura_test_support',
        '../compositor/compositor.gyp:compositor',
        '../compositor/compositor.gyp:compositor_test_support',
        '../ui.gyp:run_ui_unittests',
        '../ui.gyp:ui',
        'keyboard',
      ],
      'sources': [
        'keyboard_controller_unittest.cc',
        'keyboard_test_suite.cc',
      ],
      'conditions': [
        [ 'os_posix == 1 and OS != "mac" and OS != "android" and OS != "ios"', {
          'conditions': [
            ['linux_use_tcmalloc==1', {
              'dependencies': [
                '../../base/allocator/allocator.gyp:allocator',
              ],
            }],
          ],
        }],
      ],
    },
  ],
}
