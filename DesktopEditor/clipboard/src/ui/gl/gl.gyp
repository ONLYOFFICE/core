# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },

  'targets': [
    {
      'target_name': 'gl',
      'type': '<(component)',
      'product_name': 'gl_wrapper',  # Avoid colliding with OS X's libGL.dylib
      'dependencies': [
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '<(DEPTH)/gpu/command_buffer/command_buffer.gyp:gles2_utils',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/third_party/mesa/mesa.gyp:mesa_headers',
        '<(DEPTH)/ui/ui.gyp:ui',
      ],
      'variables': {
        'gl_binding_output_dir': '<(SHARED_INTERMEDIATE_DIR)/ui/gl',
      },
      'defines': [
        'GL_IMPLEMENTATION',
      ],
      'include_dirs': [
        '<(DEPTH)/third_party/swiftshader/include',
        '<(DEPTH)/third_party/mesa/MesaLib/include',
        '<(gl_binding_output_dir)',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(gl_binding_output_dir)',
        ],
      },
      'export_dependent_settings': [
        '<(DEPTH)/third_party/mesa/mesa.gyp:mesa_headers',
      ],
     'sources': [
        'android/gl_jni_registrar.cc',
        'android/gl_jni_registrar.h',
        'android/scoped_java_surface.cc',
        'android/scoped_java_surface.h',
        'android/surface_texture_bridge.cc',
        'android/surface_texture_bridge.h',
        'android/surface_texture_listener.cc',
        'android/surface_texture_listener.h',
        'async_pixel_transfer_delegate.cc',
        'async_pixel_transfer_delegate.h',
        'async_pixel_transfer_delegate_android.cc',
        'async_pixel_transfer_delegate_idle.cc',
        'async_pixel_transfer_delegate_idle.h',
        'async_pixel_transfer_delegate_linux.cc',
        'async_pixel_transfer_delegate_mac.cc',
        'async_pixel_transfer_delegate_stub.cc',
        'async_pixel_transfer_delegate_stub.h',
        'async_pixel_transfer_delegate_sync.cc',
        'async_pixel_transfer_delegate_sync.h',
        'async_pixel_transfer_delegate_win.cc',
        'gl_bindings.h',
        'gl_bindings_skia_in_process.cc',
        'gl_bindings_skia_in_process.h',
        'gl_context.cc',
        'gl_context.h',
        'gl_context_android.cc',
        'gl_context_mac.mm',
        'gl_context_osmesa.cc',
        'gl_context_osmesa.h',
        'gl_context_stub.cc',
        'gl_context_stub.h',
        'gl_context_win.cc',
        'gl_context_x11.cc',
        'gl_export.h',
        'gl_fence.cc',
        'gl_fence.h',
        'gl_gl_api_implementation.cc',
        'gl_gl_api_implementation.h',
        'gl_image.cc',
        'gl_image.h',
        'gl_image_android.cc',
        'gl_image_mac.cc',
        'gl_image_stub.cc',
        'gl_image_stub.h',
        'gl_image_win.cc',
        'gl_image_x11.cc',
        'gl_implementation.cc',
        'gl_implementation.h',
        'gl_implementation_android.cc',
        'gl_implementation_mac.cc',
        'gl_implementation_win.cc',
        'gl_implementation_x11.cc',
        'gl_interface.cc',
        'gl_interface.h',
        'gl_osmesa_api_implementation.cc',
        'gl_osmesa_api_implementation.h',
        'gl_share_group.cc',
        'gl_share_group.h',
        'gl_state_restorer.cc',
        'gl_state_restorer.h',
        'gl_surface.cc',
        'gl_surface.h',
        'gl_surface_android.cc',
        'gl_surface_mac.cc',
        'gl_surface_stub.cc',
        'gl_surface_stub.h',
        'gl_surface_win.cc',
        'gl_surface_x11.cc',
        'gl_surface_osmesa.cc',
        'gl_surface_osmesa.h',
        'gl_switches.cc',
        'gl_switches.h',
        'gpu_switching_manager.cc',
        'gpu_switching_manager.h',
        'safe_shared_memory_pool.h',
        'safe_shared_memory_pool.cc',
        'scoped_make_current.cc',
        'scoped_make_current.h',
        'gl_state_restorer.cc',
        'gl_state_restorer.h',
        'vsync_provider.cc',
        'vsync_provider.h',
        '<(gl_binding_output_dir)/gl_bindings_autogen_gl.cc',
        '<(gl_binding_output_dir)/gl_bindings_autogen_gl.h',
        '<(gl_binding_output_dir)/gl_bindings_autogen_mock.cc',
        '<(gl_binding_output_dir)/gl_bindings_autogen_osmesa.cc',
        '<(gl_binding_output_dir)/gl_bindings_autogen_osmesa.h',
        '<(gl_binding_output_dir)/gl_interface_autogen_gl.h',
      ],
      # hard_dependency is necessary for this target because it has actions
      # that generate header files included by dependent targets. The header
      # files must be generated before the dependents are compiled. The usual
      # semantics are to allow the two targets to build concurrently.
      'hard_dependency': 1,
      'actions': [
        {
          'action_name': 'generate_gl_bindings',
          'variables': {
            'generator_path': 'generate_bindings.py',
            'conditions': [
              ['use_system_mesa==0', {
                'header_paths': '../../third_party/mesa/MesaLib/include:../../third_party/khronos',
              }, { # use_system_mesa==1
                'header_paths': '/usr/include',
              }],
            ],
          },
          'inputs': [
            '<(generator_path)',
            '<!@(python <(generator_path) --header-paths=<(header_paths) --inputs)',
          ],
          'outputs': [
            '<(gl_binding_output_dir)/gl_bindings_autogen_egl.cc',
            '<(gl_binding_output_dir)/gl_bindings_autogen_egl.h',
            '<(gl_binding_output_dir)/gl_bindings_api_autogen_egl.h',
            '<(gl_binding_output_dir)/gl_bindings_autogen_gl.cc',
            '<(gl_binding_output_dir)/gl_bindings_autogen_gl.h',
            '<(gl_binding_output_dir)/gl_bindings_api_autogen_gl.h',
            '<(gl_binding_output_dir)/gl_bindings_autogen_glx.cc',
            '<(gl_binding_output_dir)/gl_bindings_autogen_glx.h',
            '<(gl_binding_output_dir)/gl_bindings_api_autogen_glx.h',
            '<(gl_binding_output_dir)/gl_bindings_autogen_mock.cc',
            '<(gl_binding_output_dir)/gl_bindings_autogen_osmesa.cc',
            '<(gl_binding_output_dir)/gl_bindings_autogen_osmesa.h',
            '<(gl_binding_output_dir)/gl_bindings_api_autogen_osmesa.h',
            '<(gl_binding_output_dir)/gl_bindings_autogen_wgl.cc',
            '<(gl_binding_output_dir)/gl_bindings_autogen_wgl.h',
            '<(gl_binding_output_dir)/gl_bindings_api_autogen_wgl.h',
            '<(gl_binding_output_dir)/gl_interface_autogen_egl.h',
            '<(gl_binding_output_dir)/gl_interface_autogen_gl.h',
            '<(gl_binding_output_dir)/gl_interface_autogen_glx.h',
            '<(gl_binding_output_dir)/gl_interface_autogen_osmesa.h',
            '<(gl_binding_output_dir)/gl_interface_autogen_wgl.h',
            '<(gl_binding_output_dir)/gl_mock_autogen_egl.h',
            '<(gl_binding_output_dir)/gl_mock_autogen_gl.h',
            '<(gl_binding_output_dir)/gl_mock_autogen_glx.h',
            '<(gl_binding_output_dir)/gl_mock_autogen_osmesa.h',
            '<(gl_binding_output_dir)/gl_mock_autogen_wgl.h',
          ],
          'action': [
            'python',
            '<(generator_path)',
            '--header-paths=<(header_paths)',
            '<(gl_binding_output_dir)',
          ],
        },
      ],
      'conditions': [
        ['OS in ("win", "android") or (OS == "linux" and use_x11 == 1)', {
          'sources': [
            'async_pixel_transfer_delegate_egl.cc',
            'async_pixel_transfer_delegate_egl.h',
            'egl_util.cc',
            'egl_util.h',
            'gl_context_egl.cc',
            'gl_context_egl.h',
            'gl_surface_egl.cc',
            'gl_surface_egl.h',
            'gl_egl_api_implementation.cc',
            'gl_egl_api_implementation.h',
            '<(gl_binding_output_dir)/gl_bindings_autogen_egl.cc',
            '<(gl_binding_output_dir)/gl_bindings_autogen_egl.h',
          ],
          'include_dirs': [
            '<(DEPTH)/third_party/angle/include',
          ],
        }],
        ['use_x11 == 1', {
          'sources': [
            'gl_context_glx.cc',
            'gl_context_glx.h',
            'gl_glx_api_implementation.cc',
            'gl_glx_api_implementation.h',
            'gl_image_glx.cc',
            'gl_image_glx.h',
            'gl_surface_glx.cc',
            'gl_surface_glx.h',
            'gl_egl_api_implementation.cc',
            'gl_egl_api_implementation.h',
            '<(gl_binding_output_dir)/gl_bindings_autogen_glx.cc',
            '<(gl_binding_output_dir)/gl_bindings_autogen_glx.h',
          ],
          'all_dependent_settings': {
            'defines': [
              'GL_GLEXT_PROTOTYPES',
            ],
          },
          'link_settings': {
            'libraries': [
              '-lX11',
              '-lXcomposite',
            ],
          },
        }],
        ['OS=="win"', {
          'sources': [
            'gl_context_wgl.cc',
            'gl_context_wgl.h',
            'gl_egl_api_implementation.cc',
            'gl_egl_api_implementation.h',
            'gl_surface_wgl.cc',
            'gl_surface_wgl.h',
            'gl_wgl_api_implementation.cc',
            'gl_wgl_api_implementation.h',
            '<(gl_binding_output_dir)/gl_bindings_autogen_wgl.cc',
            '<(gl_binding_output_dir)/gl_bindings_autogen_wgl.h',
          ],
        }],
        ['OS=="mac"', {
          'sources': [
            'gl_context_cgl.cc',
            'gl_context_cgl.h',
            'gl_surface_cgl.cc',
            'gl_surface_cgl.h',
          ],
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
            ],
          },
        }],
        ['OS=="mac" and use_aura == 1', {
          'sources': [
            'gl_context_nsview.mm',
            'gl_context_nsview.h',
            'gl_surface_nsview.mm',
            'gl_surface_nsview.h',
          ],
        }],
        ['OS=="android"', {
          'dependencies': [
            'gl_jni_headers',
          ],
          'sources': [
            'gl_image_egl.cc',
            'gl_image_egl.h',
          ],
          'link_settings': {
            'libraries': [
              '-landroid',
            ],
          },
          'sources!': [
            'gl_context_osmesa.cc',
            'system_monitor_posix.cc',
          ],
          'defines': [
            'GL_GLEXT_PROTOTYPES',
            'EGL_EGLEXT_PROTOTYPES',
          ],
        }],
        ['OS!="android"', {
          'sources/': [ ['exclude', '^android/'] ],
        }],
      ],
    },
    {
      'target_name': 'gl_unittest_utils',
      'type': 'static_library',
      'variables': {
        'gl_binding_output_dir': '<(SHARED_INTERMEDIATE_DIR)/ui/gl',
      },
      'dependencies': [
        '../../testing/gmock.gyp:gmock',
        '../../third_party/khronos/khronos.gyp:khronos_headers',
        'gl',
      ],
      'include_dirs': [
        '<(gl_binding_output_dir)',
        '../..',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(gl_binding_output_dir)',
        ],
      },
      'sources': [
        'gl_mock.h',
        'gl_mock.cc',
        '<(gl_binding_output_dir)/gl_mock_autogen_gl.h',
      ],
    },
  ],
  'conditions': [
    ['OS=="android"' , {
      'targets': [
        {
          'target_name': 'surface_texture_jni_headers',
          'type': 'none',
          'variables': {
            'jni_gen_package': 'ui/gl',
            'input_java_class': 'android/graphics/SurfaceTexture.class',
          },
          'includes': [ '../../build/jar_file_jni_generator.gypi' ],
        },
        {
          'target_name': 'surface_jni_headers',
          'type': 'none',
          'variables': {
            'jni_gen_package': 'ui/gl',
            'input_java_class': 'android/view/Surface.class',
          },
          'includes': [ '../../build/jar_file_jni_generator.gypi' ],
        },
        {
          'target_name': 'gl_jni_headers',
          'type': 'none',
          'dependencies': [
            'surface_texture_jni_headers',
            'surface_jni_headers',
          ],
          'sources': [
            '../android/java/src/org/chromium/ui/gfx/SurfaceTextureListener.java',
          ],
          'variables': {
            'jni_gen_package': 'ui/gl',
          },
          'includes': [ '../../build/jni_generator.gypi' ],
        },
      ],
    }],
  ],
}
