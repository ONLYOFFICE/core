# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'metro_viewer',
      'type': 'static_library',
      'dependencies': [
        '../../base/base.gyp:base',
        '../../skia/skia.gyp:skia',
      ],
      'sources': [
        'metro_viewer_message_generator.cc',
        'metro_viewer_message_generator.h',
        'metro_viewer_messages.h',
      ],
      'include_dirs': [
        '..',
      ],
    },
  ],
}

