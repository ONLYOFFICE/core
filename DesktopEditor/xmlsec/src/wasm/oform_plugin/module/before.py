import sys
sys.path.append("../../../../../../../build_tools/scripts")
import base

base.cmd_in_dir("./../../3rdParty", "python", ["openssl.py"])
