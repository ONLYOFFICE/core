# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'compositor',
      'type': '<(component)',
      'dependencies': [
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '<(DEPTH)/cc/cc.gyp:cc',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/third_party/WebKit/Source/WebKit/chromium/WebKit.gyp:webkit',
        '<(DEPTH)/ui/gl/gl.gyp:gl',
        '<(DEPTH)/ui/ui.gyp:ui',
        '<(DEPTH)/webkit/gpu/webkit_gpu.gyp:webkit_gpu',
      ],
      'defines': [
        'COMPOSITOR_IMPLEMENTATION',
      ],
      'sources': [
        'compositor.cc',
        'compositor.h',
        'compositor_export.h',
        'compositor_observer.h',
        'compositor_setup.h',
        'compositor_switches.cc',
        'compositor_switches.h',
        'debug_utils.cc',
        'debug_utils.h',
        'dip_util.cc',
        'dip_util.h',
        'float_animation_curve_adapter.cc',
        'float_animation_curve_adapter.h',
        'layer.cc',
        'layer.h',
        'layer_animation_delegate.h',
        'layer_animation_element.cc',
        'layer_animation_element.h',
        'layer_animation_observer.cc',
        'layer_animation_observer.h',
        'layer_animation_sequence.cc',
        'layer_animation_sequence.h',
        'layer_animator.cc',
        'layer_animator.h',
        'layer_delegate.h',
        'layer_owner.cc',
        'layer_owner.h',
        'layer_type.h',
        'scoped_animation_duration_scale_mode.cc',
        'scoped_animation_duration_scale_mode.h',
        'scoped_layer_animation_settings.cc',
        'scoped_layer_animation_settings.h',
        # UI tests need TestWebGraphicsContext3D, so we always build it.
        'test_web_graphics_context_3d.cc',
        'test_web_graphics_context_3d.h',
        'transform_animation_curve_adapter.cc',
        'transform_animation_curve_adapter.h',
      ],
      'conditions': [
        ['OS == "win" and use_aura == 1', {
          # TODO(sky): before we make this real need to remove
          # IDR_BITMAP_BRUSH_IMAGE.
          'dependencies': [
            '<(DEPTH)/ui/ui.gyp:ui_resources',
            '<(DEPTH)/third_party/angle/src/build_angle.gyp:libEGL',
            '<(DEPTH)/third_party/angle/src/build_angle.gyp:libGLESv2',
          ],
        }],
      ],
    },
    {
      'target_name': 'compositor_test_support',
      'type': 'static_library',
      'dependencies': [
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/webkit/support/webkit_support.gyp:webkit_support',
        '<(DEPTH)/third_party/WebKit/Source/WebKit/chromium/WebKit.gyp:webkit',
      ],
      'sources': [
        'test/compositor_test_support.cc',
        'test/compositor_test_support.h',
      ],
      'conditions': [
        ['os_posix == 1 and OS != "mac"', {
          'conditions': [
            ['linux_use_tcmalloc==1', {
              'dependencies': [
                '<(DEPTH)/base/allocator/allocator.gyp:allocator',
              ],
            }],
          ],
        }],
      ],
    },
    {
      'target_name': 'compositor_unittests',
      'type': 'executable',
      'dependencies': [
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/base/base.gyp:test_support_base',
        '<(DEPTH)/cc/cc.gyp:cc',
        '<(DEPTH)/cc/cc_tests.gyp:cc_test_utils',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/testing/gtest.gyp:gtest',
        '<(DEPTH)/ui/gl/gl.gyp:gl',
        '<(DEPTH)/ui/ui.gyp:ui',
        '<(DEPTH)/ui/ui.gyp:ui_resources',
        'compositor',
        'compositor_test_support',
      ],
      'sources': [
        'layer_animation_element_unittest.cc',
        'layer_animation_sequence_unittest.cc',
        'layer_animator_unittest.cc',
        'layer_unittest.cc',
        'run_all_unittests.cc',
        'test/layer_animator_test_controller.cc',
        'test/layer_animator_test_controller.h',
        'test/test_compositor_host.h',
        'test/test_compositor_host_linux.cc',
        'test/test_compositor_host_mac.mm',
        'test/test_compositor_host_win.cc',
        'test/test_layer_animation_delegate.cc',
        'test/test_layer_animation_delegate.h',
        'test/test_layer_animation_observer.cc',
        'test/test_layer_animation_observer.h',
        'test/test_suite.cc',
        'test/test_suite.h',
        'test/test_utils.cc',
        'test/test_utils.h',
      ],
      'conditions': [
        # osmesa GL implementation is used on linux.
        ['OS=="linux"', {
          'dependencies': [
            '<(DEPTH)/third_party/mesa/mesa.gyp:osmesa',
          ],
        }],
      ],
    },
  ],
}
