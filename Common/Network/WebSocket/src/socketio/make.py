#!/usr/bin/env python

import sys
sys.path.append('../../../../../../build_tools/scripts')
import base

file_path = "./socketio_internal_private_no_tls.h"
if not base.is_file(file_path):
  file.delete_file(file_path)

base.copy_file("./socketio_internal_private.h", file_path)

header = "///////////////////////////////////////////////////////////\n"
header += "/// auto generated file. please use generator (make.py) ///\n"
header += "///////////////////////////////////////////////////////////\n"
header += "\n#pragma once"

base.replaceInFile(file_path, "#pragma once", header)
base.replaceInFile(file_path, "/socket.io-client-cpp/src/", "/socket.io-client-cpp/src_no_tls/")
base.replaceInFile(file_path, "sio::", "sio_no_tls::")
base.replaceInFile(file_path, "CIOWebSocket_private_tls", "CIOWebSocket_private_no_tls")
