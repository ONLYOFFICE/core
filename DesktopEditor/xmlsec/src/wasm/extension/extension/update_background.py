import sys
sys.path.append("../../../../../../../build_tools/scripts")
import base
import argparse

parser = argparse.ArgumentParser(description='Generate background script for debug')

parser.add_argument("--wasm", action="store_true")
args = parser.parse_args()

content = base.readFile("./utils.js") + "\n\n"

if args.wasm:
    content = content + "\n\n" + base.readFile("./../deploy/engine.js")
    base.copy_file("./../deploy/engine.wasm", "./engine.wasm")
else:
    content = content + "\n\n" + base.readFile("./engine.js")

content = content + "\n\n" + base.readFile("./background_base.js")

base.delete_file("./background.js")
base.writeFile("./background.js", content)
