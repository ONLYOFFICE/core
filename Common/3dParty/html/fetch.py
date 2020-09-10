#!/usr/bin/env python

import sys
sys.path.append('../../../../build_tools/scripts')
import config
import base
import os
import build

base_directory = os.getcwd()

if not base.is_dir("gumbo-parser"):
  base.cmd("git", ["clone", "https://github.com/google/gumbo-parser.git"])

if not base.is_dir("katana-parser"):
  base.cmd("git", ["clone", "https://github.com/hackers-painters/katana-parser.git"])

  # fix katana
  base.replaceInFile(base_directory + "/katana-parser/src/tokenizer.c", "static inline bool katana_is_html_space(char c);", "static inline bool2 katana_is_html_space(char c);")
  base.replaceInFile(base_directory + "/katana-parser/src/tokenizer.c", "inline bool katana_is_html_space(char c)", "static inline bool katana_is_html_space(char c)")
  base.replaceInFile(base_directory + "/katana-parser/src/tokenizer.c", "static inline bool2 katana_is_html_space(char c);", "static inline bool katana_is_html_space(char c);")
  base.replaceInFile(base_directory + "/katana-parser/src/parser.c", "katanaget_text(parser->scanner)", "/*katanaget_text(parser->scanner)*/\"error\"")

