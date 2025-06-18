import os
import glob
import json
import subprocess

curDirectory = os.path.dirname(os.path.realpath(__file__))
dictionatiesDirectory = curDirectory + "/../../../../../dictionaries"

all_dictionaties = {}
for dir in glob.glob(dictionatiesDirectory + "/*"):
    if not os.path.isdir(dir):
        continue
    dictionaryName = os.path.basename(dir)
    configFile = dictionatiesDirectory + "/" + dictionaryName + "/" + dictionaryName + ".json"
    if not os.path.isfile(configFile):
        continue
    isHyphen = False
    hyphenFile = dictionatiesDirectory + "/" + dictionaryName + "/hyph_" + dictionaryName + ".dic"
    if os.path.isfile(hyphenFile):
        isHyphen = True
    with open(configFile, 'r', encoding='utf-8') as file:
        data = json.loads(file.read())
        for lang in data["codes"]:
            all_dictionaties[str(lang)] = {
                "name": dictionaryName,
                "hyphen": isHyphen
            }

content = ""
content += "#define DictionaryRec_count " + str(len(all_dictionaties)) + "\n"
content += "typedef struct {\n"
content += "    const char* m_name;\n"
content += "    int         m_lang;\n"
content += "} DictionaryRec;\n\n"
content += "static const DictionaryRec Dictionaries[DictionaryRec_count] = {\n"

for lang in all_dictionaties:
    info = all_dictionaties[lang]
    content += "    { \"" + info["name"] + "\", " + str(lang) + " },\n"
content += "};\n"

with open("./records.h", 'w', encoding='utf-8') as f:
    f.write(content)
