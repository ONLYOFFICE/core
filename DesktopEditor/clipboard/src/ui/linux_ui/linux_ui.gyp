# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'linux_ui',
      'type': '<(component)',
      'dependencies': [
        '../../base/base.gyp:base',
        '../../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '../../skia/skia.gyp:skia',
        '../ui.gyp:ui',
        '../ui.gyp:ui_resources',
        '../native_theme/native_theme.gyp:native_theme',
      ],
      'defines': [
        'LINUX_UI_IMPLEMENTATION',
      ],
      'sources': [
        'linux_ui.cc',
        'linux_ui.h',
        'linux_ui_export.h',
      ],
    },
  ],
}
