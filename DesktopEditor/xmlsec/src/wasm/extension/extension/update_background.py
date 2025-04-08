import sys
sys.path.append("../../../../../../../build_tools/scripts")
import base
import argparse

parser = argparse.ArgumentParser(description='Generate background script for debug')

parser.add_argument("-w", "--wasm", dest="is_wasm", action="store_true")
parser.set_defaults(is_wasm=False)

args = parser.parse_args()

content = base.readFile("./utils.js") + "\n\n" + base.readFile("./background.js")

if (parser.is_wasm):
    content = content + "\n\n" + base.readFile("./engine.wasm.js")
else:
    content = content + "\n\n" + base.readFile("./engine.js")

content = content + "\n\n" + base.readFile("./background.js")

base.delete_file("./background.js")
base.writeFile("./background.js", content)
