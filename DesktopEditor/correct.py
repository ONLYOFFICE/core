#!/usr/bin/env python

import sys
sys.path.append('../../build_tools/scripts')
import base
import os
import glob

# patch for qmake... 
# remove after full undestanding...

base_directory = os.getcwd()
gzip_directory = base_directory + "/freetype-2.10.4/src/gzip"
base.create_dir(gzip_directory + "/src")

for file in glob.glob(gzip_directory + "/*"):
  file_name = os.path.basename(file)
  if not base.is_file(file):
    continue
  if (file_name.find("ft") != 0):
    base.copy_file(file, gzip_directory + "/src/" + file_name)
    base.delete_file(file)

base.replaceInFile(gzip_directory + "/ftgzip.c", "<zlib.h>", "\"zlib.h\"");
base.replaceInFile(gzip_directory + "/ftgzip.c", "#include \"zutil.c\"", "//#include \"zutil.c\"");
base.replaceInFile(gzip_directory + "/ftgzip.c", "#include \"inftrees.c\"", "//#include \"inftrees.c\"");
base.replaceInFile(gzip_directory + "/ftgzip.c", "#include \"infutil.c\"", "//#include \"infutil.c\"");
base.replaceInFile(gzip_directory + "/ftgzip.c", "#include \"infcodes.c\"", "//#include \"infcodes.c\"");
base.replaceInFile(gzip_directory + "/ftgzip.c", "#include \"infblock.c\"", "//#include \"infblock.c\"");
base.replaceInFile(gzip_directory + "/ftgzip.c", "#include \"inflate.c\"", "//#include \"inflate.c\"");
base.replaceInFile(gzip_directory + "/ftgzip.c", "#include \"adler32.c\"", "//#include \"adler32.c\"");
