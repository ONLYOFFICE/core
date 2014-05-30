{
  'variables': {
    'chromium_code': 1,
    'grit_base_out_dir': '<(SHARED_INTERMEDIATE_DIR)/ui',
  },
  'targets': [
    {
      'target_name': 'ui_strings',
      'type': 'none',
      'actions': [
        {
          'action_name': 'ui_strings',
          'variables': {
            'grit_grd_file': 'ui_strings.grd',
            'grit_out_dir': '<(grit_base_out_dir)/ui_strings',
          },
          'includes': [ '../../../build/grit_action.gypi' ],
        },
        {
          'action_name': 'app_locale_settings',
          'variables': {
            'grit_grd_file': 'app_locale_settings.grd',
            'grit_out_dir': '<(grit_base_out_dir)/app_locale_settings',
          },
          'includes': [ '../../../build/grit_action.gypi' ],
        },
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(grit_base_out_dir)/app_locale_settings',
          '<(grit_base_out_dir)/ui_strings',
        ],
      },
    },
  ],
  'conditions': [
    ['os_posix == 1 and OS != "mac"', {
      'targets': [{
        'target_name': 'ui_unittest_strings',
        'type': 'none',
        'dependencies': [
          'ui_strings',
        ],
        'variables': {
          'repack_path': '<(DEPTH)/tools/grit/grit/format/repack.py',
          'conditions': [
            ['OS == "ios"', {
              'pak_output': '<(PRODUCT_DIR)/ui_unittests_strings/en.lproj/locale.pak',
            }, {
              'pak_output': '<(PRODUCT_DIR)/ui_unittests_strings/en-US.pak',
            }],
          ],
        },
        'actions': [
          {
            'action_name': 'repack_ui_unittest_strings',
            'variables': {
              'pak_inputs': [
                '<(grit_base_out_dir)/ui_strings/ui_strings_en-US.pak',
                '<(grit_base_out_dir)/app_locale_settings/app_locale_settings_en-US.pak',
              ],
            },
            'inputs': [
              '<(repack_path)',
              '<@(pak_inputs)',
            ],
            'outputs': [
              '<(pak_output)',
            ],
            'action': ['python', '<(repack_path)', '<@(_outputs)',
                       '<@(pak_inputs)'],
          },
        ],
        'copies': [
          {
            'destination': '<(PRODUCT_DIR)/ui_unittests_strings',
            'files': [
              '<(grit_base_out_dir)/ui_resources/ui_resources_100_percent.pak',
            ],
          },
        ],
      }],
    }],
  ],
}
