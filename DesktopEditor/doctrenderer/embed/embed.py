import sys
import re

def parseHeader(header_file):
	class_name = ""
	methods = []
	content = ""
	with open(header_file, 'r') as file:
		content = file.read()

		# Extract the class name
		match = re.search(r'class\s+(\w+)\s*:\s*public\s+CJSEmbedObject', content)
		if match:
			class_name = match.group(1)
		else:
			print("No class derived from CJSEmbedObject was found")
			sys.exit(1)

		# Extract names of the methods that return JSSmart<CJSValue>
		method_pattern = r'JSSmart\<CJSValue\>\s+(\w+)\s*\((.*)\)'
		methods = re.findall(method_pattern, content)

	return class_name, methods, content

def generateCode(class_name, methods):
	code =  "// THIS CODE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!\n"
	code += "// IF YOU NEED TO UPDATE THIS CODE, SIMPLY REMOVE ALL OF IT, SPECIFY \"// @embed_external\" COMMENT AND THEN RUN SCRIPT AS BEFORE.\n"
	code += "public:\n"
	code += "	static std::string getName() { return \"" + class_name + "\"; }\n"
	code += "\n"
	code += "	static CJSEmbedObject* getCreator()\n"
	code += "	{\n"
	code += "		return new " + class_name + "();\n"
	code += "	}\n"
	code += "\n"
	code += "	virtual std::vector<std::string> getNames() override\n"
	code += "	{\n"
	code += "		return std::vector<std::string> {\n"
	for method in methods:
		code += "			\"" + method[0] + "\""
		if (method != methods[-1]):
			code += ","
		code += "\n"
	code += "		};\n"
	code += "	}\n"
	code += "\n"
	code += "	void initFunctions() override\n"
	code += "	{\n"
	code += "		m_functions.resize(" + str(len(methods)) + ");\n"
	for i in range(len(methods)):
		code += "		m_functions[" + str(i) + "] = JS_FUNCTION_EMBED_" + str(methods[i][1].count("JSSmart<CJSValue>")) + "(" + methods[i][0] + ");\n"
	code += "	}\n"
	code += "// END OF GENERATED CODE"

	return code

def writeCode(header_file, code, content):
	label_embed = re.search(r'.*// \@embed_external', content)
	if (not label_embed):
		print("No comment \"// @embed_external\" was found in current file.")
		print("Write this line comment inside your class declaration where you need code to be generated.")
		sys.exit(1)

	content = content[:label_embed.start()] + generated_code + content[label_embed.end():]

	with open(header_file, 'w') as file:
		file.write(content)
		print("Generated code was written to " + header_file)


if len(sys.argv) < 2:
	print("Usage: python embed.py YourEmbedClass.h")
	sys.exit(1);

header_file = sys.argv[1];

class_name, methods, content = parseHeader(header_file)
if class_name:
	generated_code = generateCode(class_name, methods)
	writeCode(header_file, generated_code, content)
else:
	print("Proper class was not found in specified header file.")
