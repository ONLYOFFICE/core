import sys
sys.path.append("../../../../../../build_tools/scripts")
import base

base.cmd_in_dir("./../3rdParty", sys.executable, ["openssl.py"])
