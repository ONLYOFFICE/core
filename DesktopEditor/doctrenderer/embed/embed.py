import sys
import re
import os
import argparse

class MethodInfo:
	def __init__(self, name, args):
		self.name = name
		self.args = args

def makeDir(dirname):
	if not os.path.exists(dirname):
		os.mkdir(dirname)

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

def generateCommonCode(class_name):
	code = "std::string " + class_name + "::getName() { return \"" + class_name + "\"; }\n"
	code += "\n"
	code += "CJSEmbedObject* " + class_name + "::getCreator()\n"
	code += "{\n"
	code += "	return new " + class_name + "();\n"
	code += "}\n"
	return code

def generateV8InternalCode(class_name, methods, header_file):
	code =  "// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!\n"
	code += "// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH \"--internal\" OPTION.\n\n"
	code += "#include \"../" + header_file + "\"\n"
	code += "#include \"../../js_internal/v8/v8_base.h\"\n\n"
	namespace_name = "NS" + header_file[:-2]
	code += "namespace " + namespace_name + "\n"
	code += "{\n"
	code += "#define CURRENTWRAPPER " + class_name + "\n\n"
	for method in methods:
		code += "	FUNCTION_WRAPPER_V8_" + str(len(method.args)) + "(_" + method.name + ", " + method.name + ")\n"
	code += "	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)\n"
	code += "	{\n"
	code += "		v8::EscapableHandleScope handle_scope(isolate);\n"
	code += "		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);\n"
	code += "		result->SetInternalFieldCount(1);\n"
	code += "\n"
	for method in methods:
		code += "		NSV8Objects::Template_Set(result, \"" + method.name + "\",	_" + method.name + ");\n"
	code += "\n"
	code += "		return handle_scope.Escape(result);\n"
	code += "	}\n"
	code += "}\n"
	code += "\n"
	code += "void* " + class_name + "::GetDataForEmbedObject(void* data)\n"
	code += "{\n"
	code += "	v8::Isolate* isolate = reinterpret_cast<v8::Isolate*>(data);\n"
	code += "	v8::Local<v8::ObjectTemplate>* internalTemplate = new v8::Local<v8::ObjectTemplate>(" + namespace_name + "::CreateTemplate(isolate));\n"
	code += "	return reinterpret_cast<void*>(internalTemplate);\n"
	code += "}\n"
	code += "\n"
	code += "std::vector<std::string> " + class_name + "::getMethodNames()\n"
	code += "{\n"
	code += "	return std::vector<std::string>();\n"
	code += "}\n"
	code += "\n"
	code += "void " + class_name + "::initFunctions()\n"
	code += "{\n"
	code += "}\n\n"
	return code

def generateJSCInternalCode(class_name, methods, header_file):
	code =  "// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!\n"
	code += "// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH \"--internal\" OPTION.\n\n"
	code += "#include \"../" + header_file + "\"\n"
	code += "#include \"../../js_internal/jsc/jsc_base.h\"\n\n"
	objc_protocol_name = "IJS" + class_name
	code += "@protocol " + objc_protocol_name + " <JSExport>\n"
	for method in methods:
		code += "-(JSValue*) " + method.name
		for arg in method.args:
			code += " : (JSValue*)" + arg
		code += ";\n"
	code += "@end\n\n"
	objc_class_name = "CJS" + class_name
	code += "@interface " + objc_class_name + " : NSObject<" + objc_protocol_name + ", JSEmbedObjectProtocol>\n"
	code += "{\n"
	code += "@public\n"
	code += "	" + class_name + "* m_internal;\n"
	code += "}\n"
	code += "@end\n\n"
	code += "@implementation " + objc_class_name + "\n"
	code += "EMBED_OBJECT_WRAPPER_METHODS(" + class_name + ");\n"
	code += "\n"
	for method in methods:
		code += "FUNCTION_WRAPPER_JS_" + str(len(method.args)) + "(" + method.name + ", " + method.name + ")\n"
	code += "@end\n"
	code += "\n"
	code += "void* " + class_name + "::GetDataForEmbedObject(void* data)\n"
	code += "{\n"
	code += "	" + class_name + "* pNativeObj = reinterpret_cast<" + class_name + "*>(data);\n"
	code += "	" + objc_class_name + "* pEmbedObj = [[" + objc_class_name + " alloc] init:pNativeObj];\n"
	code += "	return (void*)CFBridgingRetain(pEmbedObj);\n"
	code += "}\n\n"
	return code

def generateV8ExternalCode(class_name, methods, header_file):
	code =  "// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!\n"
	code += "// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT.\n\n"
	code += "#include \"../" + header_file + "\"\n\n"
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
	code += "}\n\n"
	code += "void* " + class_name + "::GetDataForEmbedObject(void* data) { return nullptr; }\n\n"
	return code

def generateJSCExternalCode(class_name, methods, header_file):
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
	code += "@end\n\n"
	code += "void* " + class_name + "::GetDataForEmbedObject(void* data)\n"
	code += "{\n"
	code += "	" + class_name + "* pNativeObj = reinterpret_cast<" + class_name + "*>(data);\n"
	code += "	" + objc_class_name + "* pEmbedObj = [[" + objc_class_name + " alloc] init:pNativeObj];\n"
	code += "	return (void*)CFBridgingRetain(pEmbedObj);\n"
	code += "}\n\n"
	return code

def writeToFile(file_name, content):
	with open(file_name, 'w') as file:
		file.write(content)
		print("\t" + file_name)

# MAIN
parser = argparse.ArgumentParser(description='Generate files for embedding your class into JS')
parser.add_argument('filename', help='the path to .h file with class you want to embed');
parser.add_argument('-i', '--internal', action='store_true', help='for internal library usage')

# if filename wasn't specified the programm will stop here
args = parser.parse_args()

header_file = args.filename;
if header_file[-2:] != ".h":
	print("Argument must be a header file with \".h\" extension!")
	sys.exit(1);

header_dir = os.path.dirname(header_file)
if len(header_dir) == 0:
	header_dir = "."

header_base_name = os.path.basename(header_file)
header_base_name_no_extension = header_base_name[:-2]
class_name, methods = parseHeader(header_file)

if not class_name:
	print("Proper class was not found in specified header file.")
	sys.exit(1)

v8_dir = header_dir + "/v8"
jsc_dir = header_dir + "/jsc"
makeDir(v8_dir)
makeDir(jsc_dir)

code_common = generateCommonCode(class_name)
code_v8 = ""
code_jsc = ""

if args.internal:
	code_v8 = generateV8InternalCode(class_name, methods, header_base_name)
	code_jsc = generateJSCInternalCode(class_name, methods, header_base_name)
else:
	code_v8 = generateV8ExternalCode(class_name, methods, header_base_name)
	code_jsc = generateJSCExternalCode(class_name, methods, header_base_name)

print("Generated code was written to:")
writeToFile(v8_dir + "/v8_" + header_base_name_no_extension + ".cpp", code_v8 + code_common)
writeToFile(jsc_dir + "/jsc_" + header_base_name_no_extension + ".mm", code_jsc + code_common)
