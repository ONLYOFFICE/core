import sys
import re
import os
import argparse

class MethodInfo:
	def __init__(self, name, args):
		self.name = name
		self.args = args

class MethodList:
	def __init__(self, ifdef, methods):
		self.ifdef = ifdef
		self.methods = methods

def makeDir(dirname):
	if not os.path.exists(dirname):
		os.mkdir(dirname)

def getMethods(content):
	methods = []
	# Extract name and argument list of the methods that return JSSmart<CJSValue>
	src_methods = re.findall(r'JSSmart\<CJSValue\>\s+(\w+)\s*\(([^\)]*)\)\s*;', content)
	# Get method names and argument lists
	for src_method in src_methods:
		args = re.findall(r'JSSmart\<CJSValue\>\s+(\w+)', src_method[1])
		
		method = MethodInfo(src_method[0], args)
		methods.append(method)
	
	return methods

def parseHeader(header_file):
	class_name = ''
	method_lists = []
	with open(header_file, 'r') as file:
		content = file.read()

		# Extract the class name
		match = re.search(r'class\s+(JS_DECL\s+)?(\w+)\s*:\s*public\s+CJSEmbedObject', content)
		if match:
			class_name = match.group(2)
		else:
			print("No class derived from CJSEmbedObject was found")
			sys.exit(1)

		# Remove all functions which start with "/*[noexport]*/" comment
		content = re.sub(r'/\*\[noexport\]\*/\s*JSSmart\<CJSValue\>\s+\w+\s*\([^\)]*\)\s*;', '', content)

		# Handle methods inside of ifdef blocks
		ifdef_blocks = re.findall(r'#ifdef\s+([\w\d_]+)\s+(.*?)#endif', content, re.DOTALL)
		for ifdef_block in ifdef_blocks:
			methods = getMethods(ifdef_block[1])
			if len(methods) > 0:
				method_lists.append(MethodList(ifdef_block[0], methods))
				content = content.replace(ifdef_block[1], '')

		# Add all other methods
		method_lists.append(MethodList(None, getMethods(content)))

	return class_name, method_lists

def generateCommonCode(class_name):
	code = "std::string " + class_name + "::getName() { return \"" + class_name + "\"; }\n"
	code += "\n"
	code += "CJSEmbedObject* " + class_name + "::getCreator()\n"
	code += "{\n"
	code += "	return new " + class_name + "();\n"
	code += "}\n"
	return code

def generateV8InternalCode(class_name, method_lists, header_file):
	code =  "// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!\n"
	code += "// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH \"--internal\" OPTION.\n\n"
	code += "#include \"../" + header_file + "\"\n"
	code += "#include \"../../js_internal/v8/v8_base.h\"\n\n"
	namespace_name = "NS" + header_file[:-2]
	code += "namespace " + namespace_name + "\n"
	code += "{\n"
	code += "#define CURRENTWRAPPER " + class_name + "\n\n"
	for method_list in method_lists:
		if method_list.ifdef:
			code += "#ifdef " + method_list.ifdef + "\n"
		for method in method_list.methods:
			code += "	FUNCTION_WRAPPER_V8_" + str(len(method.args)) + "(_" + method.name + ", " + method.name + ")\n"
		if method_list.ifdef:
			code += "#endif\n"
	code += "\n"
	code += "	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)\n"
	code += "	{\n"
	code += "		v8::EscapableHandleScope handle_scope(isolate);\n"
	code += "		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);\n"
	code += "		result->SetInternalFieldCount(1);\n"
	code += "\n"
	for method_list in method_lists:
		if method_list.ifdef:
			code += "#ifdef " + method_list.ifdef + "\n"
		for method in method_list.methods:
			code += "		NSV8Objects::Template_Set(result, \"" + method.name + "\",	_" + method.name + ");\n"
		if method_list.ifdef:
			code += "#endif\n"
	code += "\n"
	code += "		return handle_scope.Escape(result);\n"
	code += "	}\n"
	code += "}\n"
	code += "\n"
	adapter_name = class_name + "Adapter"
	code += "class " + adapter_name + " : public CJSEmbedObjectAdapterV8Template\n"
	code += "{\n"
	code += "public:\n"
	code += "	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override\n"
	code += "	{\n"
	code += "		v8::EscapableHandleScope handle_scope(isolate);\n"
	code += "		v8::Local<v8::ObjectTemplate> templ = " + namespace_name + "::CreateTemplate(isolate);\n"
	code += "		return handle_scope.Escape(templ);\n"
	code += "	}\n"
	code += "};\n"
	code += "\n"
	code += "CJSEmbedObjectAdapterBase* " + class_name + "::getAdapter()\n"
	code += "{\n"
	code += "	if (m_pAdapter == nullptr)\n"
	code += "		m_pAdapter = new " + adapter_name + "();\n"
	code += "	return m_pAdapter;\n"
	code += "}\n\n"
	return code

def generateJSCInternalCode(class_name, method_lists, header_file):
	code =  "// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!\n"
	code += "// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH \"--internal\" OPTION.\n\n"
	code += "#include \"../" + header_file + "\"\n"
	code += "#include \"../../js_internal/jsc/jsc_base.h\"\n\n"
	objc_protocol_name = "IJS" + class_name
	code += "@protocol " + objc_protocol_name + " <JSExport>\n"
	for method_list in method_lists:
		if method_list.ifdef:
			code += "#ifdef " + method_list.ifdef + "\n"
		for method in method_list.methods:
			code += "-(JSValue*) " + method.name
			for arg in method.args:
				code += " : (JSValue*)" + arg
			code += ";\n"
		if method_list.ifdef:
			code += "#endif\n"
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
	for method_list in method_lists:
		if method_list.ifdef:
			code += "#ifdef " + method_list.ifdef + "\n"
		for method in method_list.methods:
			code += "FUNCTION_WRAPPER_JS_" + str(len(method.args)) + "(" + method.name + ", " + method.name + ")\n"
		if method_list.ifdef:
			code += "#endif\n"
	code += "@end\n"
	code += "\n"
	adapter_name = class_name + "Adapter"
	code += "class " + adapter_name + " : public CJSEmbedObjectAdapterJSC\n"
	code += "{\n"
	code += "public:\n"
	code += "	virtual id getExportedObject(CJSEmbedObject* pNative) override\n"
	code += "	{\n"
	code += "		return [[" + objc_class_name + " alloc] init:(" + class_name + "*)pNative];\n"
	code += "	}\n"
	code += "};\n"
	code += "\n"
	code += "CJSEmbedObjectAdapterBase* " + class_name + "::getAdapter()\n"
	code += "{\n"
	code += "	if (m_pAdapter == nullptr)\n"
	code += "		m_pAdapter = new " + adapter_name + "();\n"
	code += "	return m_pAdapter;\n"
	code += "}\n\n"
	return code

def generateV8ExternalCode(class_name, method_lists, header_file):
	code =  "// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!\n"
	code += "// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT.\n\n"
	code += "#include \"../" + header_file + "\"\n"
	code += "#include \"js_embed.h\"\n\n"
	adapter_name = class_name + "Adapter"
	code += "class " + adapter_name + " : public CJSEmbedObjectAdapterV8\n"
	code += "{\n"
	code += "public:\n"
	code += "	virtual std::vector<std::string> getMethodNames() override\n"
	code += "	{\n"
	code += "		return std::vector<std::string> {\n"
	for method_list in method_lists:
		if method_list.ifdef:
			code += "#ifdef " + method_list.ifdef + "\n"
		for method in method_list.methods:
			code += "			\"" + method.name + "\",\n"
		if method_list.ifdef:
			code += "#endif\n"
	code += "		};\n"
	code += "	}\n"
	code += "\n"
	code += "	virtual void initFunctions(CJSEmbedObject* pNativeObjBase) override\n"
	code += "	{\n"
	code += "		" + class_name + "* pNativeObj = static_cast<" + class_name + "*>(pNativeObjBase);\n"
	code += "		m_functions = std::vector<EmbedFunctionType> {\n"
	for method_list in method_lists:
		if method_list.ifdef:
			code += "#ifdef " + method_list.ifdef + "\n"
		for method in method_list.methods:
			code += "			[pNativeObj](CJSFunctionArguments* args) { return pNativeObj->" + method.name + "("
			for i in range(len(method.args)):
				code += "args->Get(" + str(i) + ")"
				if i != len(method.args) - 1:
					code += ", "
			code += "); },\n"
		if method_list.ifdef:
			code += "#endif\n"
	code += "		};\n"
	code += "	}\n"
	code += "};\n"
	code += "\n"
	code += "CJSEmbedObjectAdapterBase* " + class_name + "::getAdapter()\n"
	code += "{\n"
	code += "	if (m_pAdapter == nullptr)\n"
	code += "		m_pAdapter = new " + adapter_name + "();\n"
	code += "	return m_pAdapter;\n"
	code += "}\n\n"
	return code

def generateJSCExternalCode(class_name, method_lists, header_file):
	code =  "// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!\n"
	code += "// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT.\n\n"
	code += "#include \"../" + header_file + "\"\n"
	code += "#import \"js_embed.h\"\n\n"
	objc_protocol_name = "IJS" + class_name
	code += "@protocol " + objc_protocol_name + " <JSExport>\n"
	for method_list in method_lists:
		if method_list.ifdef:
			code += "#ifdef " + method_list.ifdef + "\n"
		for method in method_list.methods:
			code += "-(JSValue*) " + method.name
			for arg in method.args:
				code += " : (JSValue*)" + arg
			code += ";\n"
		if method_list.ifdef:
			code += "#endif\n"
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
	for method_list in method_lists:
		if method_list.ifdef:
			code += "#ifdef " + method_list.ifdef + "\n"
		for method in method_list.methods:
			code += "-(JSValue*) " + method.name
			for arg in method.args:
				code += " : (JSValue*)" + arg
			code += "\n{\n"
			code += "	JSSmart<CJSValue> ret = m_internal->" + method.name + "("
			for arg in method.args:
				code += "CJSEmbedObjectAdapterJSC::Native2Value(" + arg + ")"
				if arg != method.args[-1]:
					code += ", "
			code += ");\n"
			code += "	return CJSEmbedObjectAdapterJSC::Value2Native(ret);\n"
			code += "}\n"
		if method_list.ifdef:
			code += "#endif\n\n"
	code += "@end\n\n"
	adapter_name = class_name + "Adapter"
	code += "class " + adapter_name + " : public CJSEmbedObjectAdapterJSC\n"
	code += "{\n"
	code += "public:\n"
	code += "	virtual id getExportedObject(CJSEmbedObject* pNative) override\n"
	code += "	{\n"
	code += "		return [[" + objc_class_name + " alloc] init:(" + class_name + "*)pNative];\n"
	code += "	}\n"
	code += "};\n"
	code += "\n"
	code += "CJSEmbedObjectAdapterBase* " + class_name + "::getAdapter()\n"
	code += "{\n"
	code += "	if (m_pAdapter == nullptr)\n"
	code += "		m_pAdapter = new " + adapter_name + "();\n"
	code += "	return m_pAdapter;\n"
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
class_name, method_lists = parseHeader(header_file)

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
	code_v8 = generateV8InternalCode(class_name, method_lists, header_base_name)
	code_jsc = generateJSCInternalCode(class_name, method_lists, header_base_name)
else:
	code_v8 = generateV8ExternalCode(class_name, method_lists, header_base_name)
	code_jsc = generateJSCExternalCode(class_name, method_lists, header_base_name)

print("Generated code was written to:")
writeToFile(v8_dir + "/v8_" + header_base_name_no_extension + ".cpp", code_v8 + code_common)
writeToFile(jsc_dir + "/jsc_" + header_base_name_no_extension + ".mm", code_jsc + code_common)
