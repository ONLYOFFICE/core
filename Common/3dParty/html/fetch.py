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
  base.cmd_in_dir("gumbo-parser", "git", ["checkout", "aa91b27b02c0c80c482e24348a457ed7c3c088e0"])

  # fix gumbo
  base.replaceInFile(base_directory + "/gumbo-parser/src/tag.c", "isspace(*c)", "isspace((unsigned char)*c)")

if not base.is_dir("katana-parser"):
  base.cmd("git", ["clone", "https://github.com/hackers-painters/katana-parser.git"])
  base.cmd_in_dir("katana-parser", "git", ["checkout", "499118d32c387a893fdc9dda2cb95eee524bdb9b"])

  # fix katana
  base.replaceInFile(base_directory + "/katana-parser/src/tokenizer.c", "static inline bool katana_is_html_space(char c);", "static inline bool2 katana_is_html_space(char c);")
  base.replaceInFile(base_directory + "/katana-parser/src/tokenizer.c", "inline bool katana_is_html_space(char c)", "static inline bool katana_is_html_space(char c)")
  base.replaceInFile(base_directory + "/katana-parser/src/tokenizer.c", "static inline bool2 katana_is_html_space(char c);", "static inline bool katana_is_html_space(char c);")
  base.replaceInFile(base_directory + "/katana-parser/src/parser.c", "katanaget_text(parser->scanner)", "/*katanaget_text(parser->scanner)*/\"error\"")

