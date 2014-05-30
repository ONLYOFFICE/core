# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'sources': [
    'text_input_test_support.cc',
    'text_input_test_support.h',
    'win/mock_tsf_bridge.cc',
    'win/mock_tsf_bridge.h',
  ],
  'conditions': [
    ['OS!="win"', {
      'sources!': [
        'win/mock_tsf_bridge.cc',
        'win/mock_tsf_bridge.h',
      ],
    }],
  ],
}
