# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'snapshot',
      'type': '<(component)',
      'dependencies': [
        '../../skia/skia.gyp:skia',
        '../../base/base.gyp:base',
        '../ui.gyp:ui',
      ],
      'defines': [
        'SNAPSHOT_IMPLEMENTATION',
      ],
      'sources': [
        'snapshot.h',
        'snapshot_android.cc',
        'snapshot_aura.cc',
        'snapshot_export.h',
        'snapshot_gtk.cc',
        'snapshot_ios.mm',
        'snapshot_mac.mm',
        'snapshot_win.cc',
        'snapshot_win.h',
      ],
      'include_dirs': [
        '..',
      ],
      'conditions': [
        ['use_aura==1', {
          'dependencies': [
            '../aura/aura.gyp:aura',
            '../compositor/compositor.gyp:compositor',
          ],
        }],
      ],
    },
    {
      'target_name': 'snapshot_unittests',
      'type': '<(gtest_target_type)',
      'dependencies': [
        '../../skia/skia.gyp:skia',
        '../../base/base.gyp:base',
        '../../testing/gtest.gyp:gtest',
        '../../testing/gmock.gyp:gmock',
        '../../testing/gtest.gyp:gtest',
        '../ui.gyp:ui',
        'snapshot'
      ],
      'sources': [
        'snapshot_mac_unittest.mm',
      ]
    },
  ],
  'conditions': [
    ['OS=="win"', {
      'targets': [
        {
          'target_name': 'snapshot_test_support',
          'type': 'static_library',
          'sources': [
            'test/snapshot_desktop.h',
            'test/snapshot_desktop_win.cc',
          ],
          'dependencies': [
            'snapshot',
          ],
          'include_dirs': [
            '../..',
          ],
        },
      ],
    }],
  ],
}
