# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'webview',
      'type': '<(component)',
      'dependencies': [
        '../../../../base/base.gyp:base',
        '../../../../base/base.gyp:base_i18n',
        '../../../../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '../../../../build/temp_gyp/googleurl.gyp:googleurl',
        '../../../../content/content.gyp:content',
        '../../../../skia/skia.gyp:skia',
        '../../../ui.gyp:ui',
        '../../../web_dialogs/web_dialogs.gyp:web_dialogs',
        '../../views.gyp:views',
      ],
      'defines': [
        'WEBVIEW_IMPLEMENTATION',
      ],
      'sources': [
        'unhandled_keyboard_event_handler.cc',
        'unhandled_keyboard_event_handler.h',
        'unhandled_keyboard_event_handler_aurax11.cc',
        'unhandled_keyboard_event_handler_win.cc',
        'web_dialog_view.cc',
        'web_dialog_view.h',
        'webview.cc',
        'webview.h',
        'webview_export.h',
      ],
    },
  ],
}
