#!/usr/bin/env python

import sys
sys.path.append('./../../../build_tools/scripts')
import base

def make():
  # 1. Configure for 7z using (if need)
  base.configure_common_apps("../../../build_tools/scripts/base.py")

  # 2. Reading template.docx

  # 3. Reading template.h

  # 4. Replace the content inside autogen section to BYTE template_memory[len] = { 1,0,... };

  # 5. update template.docx.
  return

make()
