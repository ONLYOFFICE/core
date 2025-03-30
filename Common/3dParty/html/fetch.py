#!/usr/bin/env python

import sys
sys.path.append('../../../../build_tools-local-qt-default-jdk/scripts')
import config
import base
import os

base_directory = os.getcwd()

if not base.is_dir("gumbo-parser"):
  base.cmd("git", ["clone", "https://github.com/google/gumbo-parser.git"])
  base.cmd_in_dir("gumbo-parser", "git", ["checkout", "aa91b27b02c0c80c482e24348a457ed7c3c088e0"])

  # fix gumbo
  base.replaceInFile(base_directory + "/gumbo-parser/src/tag.c", "isspace(*c)", "isspace((unsigned char)*c)")

if not base.is_dir("katana-parser"):
  base.cmd("git", ["clone", "https://github.com/jasenhuang/katana-parser.git"])
  base.cmd_in_dir("katana-parser", "git", ["checkout", "be6df458d4540eee375c513958dcb862a391cdd1"])

  # fix katana
  base.replaceInFileUtf8(base_directory + "/katana-parser/src/tokenizer.c", "static inline bool katana_is_html_space(char c);", "static inline bool2 katana_is_html_space(char c);")
  base.replaceInFileUtf8(base_directory + "/katana-parser/src/tokenizer.c", "inline bool katana_is_html_space(char c)", "static inline bool katana_is_html_space(char c)")
  base.replaceInFileUtf8(base_directory + "/katana-parser/src/tokenizer.c", "static inline bool2 katana_is_html_space(char c);", "static inline bool katana_is_html_space(char c);")
  base.replaceInFileUtf8(base_directory + "/katana-parser/src/parser.c", "katanaget_text(parser->scanner)", "/*katanaget_text(parser->scanner)*/\"error\"")
  base.replaceInFileUtf8(base_directory + "/katana-parser/src/parser.c", "#define KATANA_PARSER_STRING(literal) (KatanaParserString){", "#define KATANA_PARSER_STRING(literal) {")
  # katana may not be able to handle an empty string correctly in some cases (bug#73485)
  base.replaceInFileUtf8(base_directory + "/katana-parser/src/foundation.c", "size_t len = strlen(str);", "if (NULL == str)\n        return;\n    size_t len = strlen(str);")
