# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },

  'target_defaults': {
    'conditions': [
      ['use_x11 == 1', {
        'include_dirs': [
          '<(DEPTH)/third_party/angle/include',
        ],
      }],
      # TODO(ncarter): Does hlsl compilation belong in a shared location?
      ['OS == "win"', {
        'include_dirs': [
          '<(INTERMEDIATE_DIR)/hlsl',
        ],
        'rules': [
          {
            'variables': {
              'fxc': '<(windows_sdk_path)/bin/x86/fxc.exe',
              'h_file': '<(INTERMEDIATE_DIR)/hlsl/<(RULE_INPUT_ROOT)_hlsl_compiled.h',
              'cc_file': '<(INTERMEDIATE_DIR)/hlsl/<(RULE_INPUT_ROOT)_hlsl_compiled.cc',
            },
            'rule_name': 'compile_hlsl',
            'extension': 'hlsl',
            'inputs': [
              '<(fxc)',
              'compile_hlsl.py'
            ],
            'outputs': [
              '<(h_file)',
              '<(cc_file)',
            ],
            'action': [
              'python',
              'compile_hlsl.py',
              '--shader_compiler_tool', '<(fxc)',
              '--output_h_file', '<(h_file)',
              '--output_cc_file', '<(cc_file)',
              '--input_hlsl_file', '<(RULE_INPUT_PATH)',
            ],
            'msvs_cygwin_shell': 0,
            'message': 'Generating shaders from <(RULE_INPUT_PATH)',
            'process_outputs_as_sources': 1,
          },
        ],
      }],
    ],
  },
  'targets': [
    {
      'target_name': 'surface',
      'type': '<(component)',
      'dependencies': [
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '<(DEPTH)/media/media.gyp:media',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/ui/gl/gl.gyp:gl',
        '<(DEPTH)/ui/ui.gyp:ui',
      ],
      'sources': [
        'accelerated_surface_mac.cc',
        'accelerated_surface_mac.h',
        'accelerated_surface_transformer_win.cc',
        'accelerated_surface_transformer_win.h',
        'accelerated_surface_transformer_win.hlsl',
        'accelerated_surface_win.cc',
        'accelerated_surface_win.h',
        'd3d9_utils_win.cc',
        'd3d9_utils_win.h',
        'io_surface_support_mac.cc',
        'io_surface_support_mac.h',
        'surface_export.h',
        'surface_switches.h',
        'surface_switches.cc',
        'transport_dib.h',
        'transport_dib.cc',
        'transport_dib_posix.cc',
        'transport_dib_sysvipc.cc',
        'transport_dib_win.cc',
      ],
      'defines': [
        'SURFACE_IMPLEMENTATION',
      ],
    },
  ],
  'conditions': [
    ['OS == "win"', {
      'targets': [
        {
          'target_name': 'surface_gpu_tests',
          'type': '<(gtest_target_type)',
          'dependencies': [
            '<(DEPTH)/base/base.gyp:base',
            '<(DEPTH)/base/base.gyp:run_all_unittests',
            '<(DEPTH)/media/media.gyp:yuv_convert',
            '<(DEPTH)/skia/skia.gyp:skia',
            '<(DEPTH)/testing/gtest.gyp:gtest',
            '<(DEPTH)/ui/ui.gyp:ui',
            'surface',
          ],
          'sources': [
            'accelerated_surface_transformer_win_unittest.cc',
          ],
        },
      ],
    }],
  ],
}
