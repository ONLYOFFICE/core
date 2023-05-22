import sys
import re
import os

class MethodInfo:
	def __init__(self, name, args):
		self.name = name
		self.args = args

def parseHeader(header_file):
	class_name = ""
	methods = []
	src_methods = []
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

		# Extract name and argument list of the methods that return JSSmart<CJSValue>
		src_methods = re.findall(r'JSSmart\<CJSValue\>\s+(\w+)\s*\((.*)\)', content)

	for src_method in src_methods:
		args = re.findall(r'JSSmart\<CJSValue\>\s+(\w+)', src_method[1])
		
		method = MethodInfo(src_method[0], args)
		methods.append(method)

	return class_name, methods

def generateCommonCode(class_name, methods, header_file):
	code =  "// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!\n"
	code += "// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT.\n\n"
	code += "#include \"../" + header_file + "\"\n\n"
	code += "std::string " + class_name + "::getName() { return \"" + class_name + "\"; }\n"
	code += "\n"
	code += "CJSEmbedObject* " + class_name + "::getCreator()\n"
	code += "{\n"
	code += "	return new " + class_name + "();\n"
	code += "}\n"
	code += "\n"
	code += "#ifndef JS_ENGINE_JAVASCRIPTCORE\n"
	code += "std::vector<std::string> " + class_name + "::getMethodNames()\n"
	code += "{\n"
	code += "	return std::vector<std::string> {\n"
	for method in methods:
		code += "		\"" + method.name + "\""
		if (method != methods[-1]):
			code += ","
		code += "\n"
	code += "	};\n"
	code += "}\n"
	code += "\n"
	code += "void " + class_name + "::initFunctions()\n"
	code += "{\n"
	code += "	m_functions = std::vector<EmbedFunctionType> {\n"
	for method in methods:
		code += "		[this](CJSFunctionArguments* args) { return this->" + method.name + "("
		for i in range(len(method.args)):
			code += "args->Get(" + str(i) + ")"
			if i != len(method.args) - 1:
				code += ", "
		code += "); }"
		if (method != methods[-1]):
			code += ","
		code += "\n"
	code += "	};\n"
	code += "}\n"
	code += "#endif\n"

	return code

def generateJSCCode(class_name, methods, header_file):
	code =  "// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!\n"
	code += "// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT.\n\n"
	code += "#include \"../" + header_file + "\"\n"
	code += "#import \"js_embed.h\"\n\n"
	objc_protocol_name = "IJS" + class_name
	code += "@protocol " + objc_protocol_name + " <JSExport>\n"
	for method in methods:
		code += "-(JSValue*) " + method.name
		for arg in method.args:
			code += " : (JSValue*)" + arg
		code += ";\n"
	code += "@end\n"
	code += "\n"
	objc_class_name = "CJS" + class_name
	code += "@interface " + objc_class_name + " : NSObject<" + objc_protocol_name + ", JSEmbedObjectProtocol>\n"
	code += "{\n"
	code += "@public\n"
	code += "	" + class_name + "* m_internal;\n"
	code += "}\n"
	code += "@end\n"
	code += "\n"
	code += "@implementation " + objc_class_name + "\n"
	code += "EMBED_OBJECT_WRAPPER_METHODS(" + class_name + ");\n"
	code += "\n"
	for method in methods:
		code += "-(JSValue*) " + method.name
		for arg in method.args:
			code += " : (JSValue*)" + arg
		code += "\n{\n"
		code += "	JSSmart<CJSValue> ret = m_internal->" + method.name + "("
		for arg in method.args:
			code += "NSJSBase::Native2Value((__bridge void*)" + arg + ")"
			if arg != method.args[-1]:
				code += ", "
		code += ");\n"
		code += "	return (__bridge JSValue*)NSJSBase::Value2Native(ret);\n"
		code += "}\n\n"

	code += "@end\n"

	return code

def writeToFile(file_name, content):
	with open(file_name, 'w') as file:
		file.write(content)
		print("\t" + file_name)

# MAIN

if len(sys.argv) < 2:
	print("Usage: python embed.py YourEmbedClass.h")
	sys.exit(1);

header_file = sys.argv[1];
if header_file[-2:] != ".h":
	print("Argument must be a header file with \".h\" extension!")
	sys.exit(1);

header_base_name = header_file[:-2]
class_name, methods = parseHeader(header_file)

if not class_name:
	print("Proper class was not found in specified header file.")
	sys.exit(1)

if not os.path.exists("embed"):
	os.mkdir("embed")

print("Generated code was written to:")
code_common = generateCommonCode(class_name, methods, header_file)
writeToFile("embed/common_" + header_base_name + ".cpp", code_common)
code_jsc = generateJSCCode(class_name, methods, header_file)
writeToFile("embed/jsc_" + header_base_name + ".mm", code_jsc)
# TODO: generateV8Code