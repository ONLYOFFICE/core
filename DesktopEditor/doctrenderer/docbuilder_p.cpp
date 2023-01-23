/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "docbuilder_p.h"

std::wstring NSDoctRenderer::CDocBuilder_Private::m_sExternalDirectory = L"";

void CV8RealTimeWorker::_LOGGING_ERROR_(const std::wstring& strType, const std::wstring& strError)
{
	std::string sT = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strType);
	std::string sE = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strError);

	std::cerr << sT << ": " << sE << std::endl;
}

using namespace NSJSBase;

CV8RealTimeWorker::CV8RealTimeWorker(NSDoctRenderer::CDocBuilder* pBuilder)
{
	m_nFileType = -1;

	m_context = new CJSContext();
	m_context->Initialize();

	m_isolate_scope = m_context->CreateIsolateScope();
	m_handle_scope  = m_context->CreateLocalScope();

	m_context->CreateGlobalForContext();
	CNativeControlEmbed::CreateObjectBuilderInContext("CreateNativeEngine", m_context);
	CGraphicsEmbed::CreateObjectInContext("CreateNativeGraphics", m_context);
	NSJSBase::CreateDefaults(m_context);
	m_context->CreateContext();

	JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
	JSSmart<CJSTryCatch> try_catch = m_context->GetExceptions();

	builder_CreateNative("builderJS", m_context, pBuilder);
}
CV8RealTimeWorker::~CV8RealTimeWorker()
{
	m_oContextData.Clear();
	m_handle_scope = NULL;
	m_isolate_scope = NULL;
	m_context->Dispose();
}

bool CV8RealTimeWorker::ExecuteCommand(const std::wstring& command, NSDoctRenderer::CDocBuilderValue* retValue)
{
	LOGGER_SPEED_START

			if (retValue)
			retValue->Clear();

	std::string commandA = U_TO_UTF8(command);
	//commandA = "Api." + commandA;

	JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
	JSSmart<CJSTryCatch> try_catch = m_context->GetExceptions();

	LOGGER_SPEED_LAP("compile_command")

			JSSmart<CJSValue> retNativeVal = m_context->runScript(commandA, try_catch);
	if(try_catch->Check())
		return false;

	if (retValue)
	{
		NSDoctRenderer::CDocBuilderValue_Private* privateRet = (NSDoctRenderer::CDocBuilderValue_Private*)retValue->private_get_internal();
		privateRet->m_context = m_context;
		privateRet->m_value = retNativeVal;
	}

	LOGGER_SPEED_LAP("run_command")

			return true;
}

std::string CV8RealTimeWorker::GetGlobalVariable()
{
	std::string commandA = "JSON.stringify(GlobalVariable);";

	JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
	JSSmart<CJSTryCatch> try_catch = m_context->GetExceptions();

	JSSmart<CJSValue> _value = m_context->runScript(commandA, try_catch);
	if(try_catch->Check())
		return "";
	if(_value->isString())
		return _value->toStringA();
	return "{}";
}

std::wstring CV8RealTimeWorker::GetJSVariable(std::wstring sParam)
{
	std::string sParamA = U_TO_UTF8(sParam);
	NSStringUtils::string_replaceA(sParamA, "\\\"", "\"");
	std::string commandA = "(function(){ return (" + sParamA + "); })()";

	JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
	JSSmart<CJSTryCatch> try_catch = m_context->GetExceptions();

	JSSmart<CJSValue> _value = m_context->runScript(commandA, try_catch);
	if(try_catch->Check())
		return L"jsValue(" + sParam + L")";
	if(_value->isString())
		return _value->toStringW();
	return L"jsValue(" + sParam + L")";
}

bool CV8RealTimeWorker::OpenFile(const std::wstring& sBasePath, const std::wstring& path, const std::string& sString, const std::wstring& sCachePath, CV8Params* pParams)
{
	LOGGER_SPEED_START

			JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
	JSSmart<CJSTryCatch>         try_catch = m_context->GetExceptions();

	LOGGER_SPEED_LAP("compile");

	m_context->runScript(sString, try_catch, sCachePath);
	if(try_catch->Check())
		return false;

	LOGGER_SPEED_LAP("run")

			if (true)
	{
		std::string sArg = m_sUtf8ArgumentJSON;
		if (sArg.empty())
			sArg = "{}";
		NSStringUtils::string_replaceA(sArg, "\\", "\\\\");
		NSStringUtils::string_replaceA(sArg, "\"", "\\\"");
		std::string sArgument = "var Argument = JSON.parse(\"" + sArg + "\");";

		m_context->runScript(sArgument, try_catch);
		if (try_catch->Check())
			return false;
	}

	if (true)
	{
		std::string sArg = m_sGlobalVariable;
		if (sArg.empty())
			sArg = "{}";
		NSStringUtils::string_replaceA(sArg, "\\", "\\\\");
		NSStringUtils::string_replaceA(sArg, "\"", "\\\"");

		std::string sScriptVar = "var GlobalVariable = JSON.parse(\"" + sArg + "\");";

		m_context->runScript(sScriptVar, try_catch);
		if (try_catch->Check())
			return false;
	}

	NSNativeControl::CNativeControl* pNative = NULL;
	bool bIsBreak = false;

	JSSmart<CJSObject> global_js = m_context->GetGlobal();
	JSSmart<CJSValue> args[1];
	args[0] = CJSContext::createInt(0);

	// GET_NATIVE_ENGINE
	if (!bIsBreak)
	{
		JSSmart<CJSValue> js_result2 = global_js->call_func("GetNativeEngine", 1, args);
		if (try_catch->Check())
			bIsBreak = true;
		else
		{
			JSSmart<CJSObject> objNative = js_result2->toObject();
			pNative = (NSNativeControl::CNativeControl*)objNative->getNative()->getObject();
		}
	}

	if (pNative != NULL)
	{
		if (pParams)
		{
			pNative->m_oParams = *pParams;
		}

		pNative->m_strFontsDirectory = sBasePath + L"/sdkjs/common";
		pNative->m_strImagesDirectory = path + L"/media";

		pNative->CheckFonts();

		if (0 == m_nFileType)
			pNative->m_strEditorType = L"document";
		else if (1 == m_nFileType)
			pNative->m_strEditorType = L"presentation";
		else
			pNative->m_strEditorType = L"spreadsheet";

		pNative->SetFilePath(path + L"/Editor.bin");

		pNative->m_sChangesBuilderPath = path + L"/changes/changes0.json";

		pNative->m_nMaxChangesNumber = -1;
	}

	// OPEN
	if (!bIsBreak)
	{
		CChangesWorker oWorkerLoader;
		int nVersion = oWorkerLoader.OpenNative(pNative->GetFilePath());

		JSSmart<CJSValue> args_open[3];
		args_open[0] = oWorkerLoader.GetDataFull()->toObject()->toValue();
		args_open[1] = CJSContext::createInt(nVersion);
		std::wstring sXlsx = NSFile::GetDirectoryName(pNative->GetFilePath()) + L"/Editor.xlsx";
		args_open[2] = NSFile::CFileBinary::Exists(sXlsx) ? CJSContext::createString(sXlsx) : CJSContext::createUndefined();

		global_js->call_func("NativeOpenFileData", 3, args_open);
		if (try_catch->Check())
			bIsBreak = true;
	}

	if (!bIsBreak)
		bIsBreak = !this->ExecuteCommand(L"Api.asc_nativeInitBuilder();");
	if (!bIsBreak)
		bIsBreak = !this->ExecuteCommand(L"Api.asc_SetSilentMode(true);");

	LOGGER_SPEED_LAP("open")

			return !bIsBreak;
}

bool CV8RealTimeWorker::SaveFileWithChanges(int type, const std::wstring& _path, const std::wstring& sJsonParams)
{
	NSDoctRenderer::DoctRendererFormat::FormatFile _formatDst = NSDoctRenderer::DoctRendererFormat::DOCT;
	if (type & AVS_OFFICESTUDIO_FILE_PRESENTATION)
		_formatDst = NSDoctRenderer::DoctRendererFormat::PPTT;
	else if (type & AVS_OFFICESTUDIO_FILE_SPREADSHEET)
		_formatDst = NSDoctRenderer::DoctRendererFormat::XLST;
	else if ((type & AVS_OFFICESTUDIO_FILE_CROSSPLATFORM) || (type & AVS_OFFICESTUDIO_FILE_IMAGE))
		_formatDst = NSDoctRenderer::DoctRendererFormat::PDF;

	JSSmart<CJSContextScope> context_scope = m_context->CreateContextScope();
	JSSmart<CJSTryCatch> try_catch = m_context->GetExceptions();

	NSNativeControl::CNativeControl* pNative = NULL;

	JSSmart<CJSObject> global_js = m_context->GetGlobal();
	JSSmart<CJSValue> args[1];
	args[0] = CJSContext::createInt(0);

	// GET_NATIVE_ENGINE
	if (true)
	{
		JSSmart<CJSValue> js_result2 = global_js->call_func("GetNativeEngine", 1, args);
		if (!try_catch->Check())
		{
			JSSmart<CJSObject> objNative = js_result2->toObject();
			pNative = (NSNativeControl::CNativeControl*)objNative->getNative()->getObject();
		}
	}

	if (pNative == NULL)
		return false;

	if (_formatDst == NSDoctRenderer::DoctRendererFormat::PDF)
		this->ExecuteCommand(L"Api.asc_SetSilentMode(false);");

	std::wstring strError;
	bool bIsError = Doct_renderer_SaveFile_ForBuilder(_formatDst,
													  _path,
													  pNative,
													  m_context,
													  args,
													  strError,
													  sJsonParams);

	if (_formatDst == NSDoctRenderer::DoctRendererFormat::PDF)
		this->ExecuteCommand(L"Api.asc_SetSilentMode(true);");

	return bIsError;
}

namespace NSDoctRenderer
{
	CString::CString()
	{
		m_internal = new CString_Private();
	}
	CString::CString(const CString& src)
	{
		m_internal = new CString_Private();
		src.m_internal->Copy(m_internal);
	}
	CString& CString::operator=(const CString& src)
	{
		src.m_internal->Copy(m_internal);
		return *this;
	}
	CString::~CString()
	{
		delete m_internal;
	}
	wchar_t* CString::c_str() const
	{
		return m_internal->m_data;
	}

	CDocBuilderContextScope::CDocBuilderContextScope()
	{
		m_internal = new CDocBuilderContextScope_Private();
	}
	CDocBuilderContextScope::CDocBuilderContextScope(const CDocBuilderContextScope& src)
	{
		m_internal = new CDocBuilderContextScope_Private();
		m_internal->m_scope_wrap = src.m_internal->m_scope_wrap;
		m_internal->m_context_data = src.m_internal->m_context_data;
	}
	CDocBuilderContextScope& CDocBuilderContextScope::operator=(const CDocBuilderContextScope& src)
	{
		m_internal->m_scope_wrap = src.m_internal->m_scope_wrap;
		m_internal->m_context_data = src.m_internal->m_context_data;
		return *this;
	}
	CDocBuilderContextScope::~CDocBuilderContextScope()
	{
		Close();
		RELEASEOBJECT(m_internal);
	}
	void CDocBuilderContextScope::Close()
	{
		if (m_internal->m_scope_wrap.is_init() && 2 == m_internal->m_scope_wrap.GetCountReference())
		{
			m_internal->m_context_data->RemoveScope(m_internal->m_scope_wrap);
			m_internal->m_scope_wrap->Close();
		}
	}

	CDocBuilderValue::CDocBuilderValue()
	{
		m_internal = new CDocBuilderValue_Private();
	}
	CDocBuilderValue::CDocBuilderValue(const CDocBuilderValue& src)
	{
		m_internal = new CDocBuilderValue_Private();
		*this = src;

		// only for constructor
		if (src.m_internal->m_parent.is_init())
			m_internal->m_parent = src.m_internal->m_parent;
	}
	CDocBuilderValue& CDocBuilderValue::operator=(const CDocBuilderValue& src)
	{
		m_internal->m_context = src.m_internal->m_context;
		m_internal->m_value = src.m_internal->m_value;

		m_internal->m_nativeType = src.m_internal->m_nativeType;
		m_internal->m_nativeValue = src.m_internal->m_nativeValue;

		switch (m_internal->m_nativeType)
		{
		case CDocBuilderValue_Private::ptString:
		{
			size_t len = wcslen(m_internal->m_nativeValue.sValue);
			wchar_t* copy_ptr = new wchar_t[len + 1];
			memcpy(copy_ptr, m_internal->m_nativeValue.sValue, (len + 1) * sizeof(wchar_t));
			m_internal->m_nativeValue.sValue = copy_ptr;
			break;
		}
		default:
			break;
		}

		if (m_internal->m_parent.is_init())
		{
			m_internal->CheckNative();

			JSSmart<CJSValue> oParent = m_internal->m_parent->m_parent;

			if (oParent->isArray())
			{
				JSSmart<CJSArray> oParentArray = oParent->toArray();
				oParentArray->set(m_internal->m_parent->m_parent_index, m_internal->m_value.GetPointer());
			}
			else if (oParent->isObject() && !m_internal->m_parent->m_parent_prop_name.empty())
			{
				JSSmart<CJSObject> oParentObject = oParent->toObject();
				oParentObject->set(m_internal->m_parent->m_parent_prop_name.c_str(), m_internal->m_value.GetPointer());
			}
		}

		return *this;
	}
	CDocBuilderValue::~CDocBuilderValue()
	{
		delete m_internal;
	}
	void* CDocBuilderValue::private_get_internal()
	{
		return (void*)m_internal;
	}

	bool CDocBuilderValue::IsEmpty()
	{
		if (!m_internal->m_value.is_init())
			return true;
		return m_internal->m_value->isEmpty();
	}
	void CDocBuilderValue::Clear()
	{
		m_internal->Clear();
	}
	bool CDocBuilderValue::IsNull()
	{
		if (!m_internal->m_value.is_init())
			return (m_internal->m_nativeType == CDocBuilderValue_Private::ptNull) ? true : false;
		return m_internal->m_value->isNull();
	}
	bool CDocBuilderValue::IsUndefined()
	{
		if (!m_internal->m_value.is_init())
			return (m_internal->m_nativeType == CDocBuilderValue_Private::ptUndefined) ? true : false;
		return m_internal->m_value->isUndefined();
	}
	bool CDocBuilderValue::IsBool()
	{
		if (!m_internal->m_value.is_init())
			return (m_internal->m_nativeType == CDocBuilderValue_Private::ptBool) ? true : false;
		return m_internal->m_value->isBool();
	}
	bool CDocBuilderValue::IsInt()
	{
		if (!m_internal->m_value.is_init())
			return (m_internal->m_nativeType == CDocBuilderValue_Private::ptInt) ? true : false;
		return m_internal->m_value->isNumber();
	}
	bool CDocBuilderValue::IsDouble()
	{
		if (!m_internal->m_value.is_init())
			return (m_internal->m_nativeType == CDocBuilderValue_Private::ptDouble) ? true : false;
		return m_internal->m_value->isNumber();
	}
	bool CDocBuilderValue::IsString()
	{
		if (!m_internal->m_value.is_init())
			return (m_internal->m_nativeType == CDocBuilderValue_Private::ptString) ? true : false;
		return m_internal->m_value->isString();
	}
	bool CDocBuilderValue::IsObject()
	{
		if (!m_internal->m_value.is_init())
			return false;
		return m_internal->m_value->isObject();
	}
	bool CDocBuilderValue::IsFunction()
	{
		if (!m_internal->m_value.is_init())
			return false;
		return m_internal->m_value->isFunction();
	}
	bool CDocBuilderValue::IsArray()
	{
		if (!m_internal->m_value.is_init())
			return false;
		return m_internal->m_value->isArray();
	}
	bool CDocBuilderValue::IsTypedArray()
	{
		if (!m_internal->m_value.is_init())
			return false;
		return m_internal->m_value->isTypedArray();
	}

	unsigned int CDocBuilderValue::GetLength()
	{
		if (IsArray())
		{
			JSSmart<CJSArray> array = m_internal->m_value->toArray();
			return (unsigned int)array->getCount();
		}
		if (IsTypedArray())
		{
			JSSmart<CJSTypedArray> array = m_internal->m_value->toTypedArray();
			return (unsigned int)array->getCount();
		}
		return 0;
	}

	bool CDocBuilderValue::ToBool()
	{
		if (!m_internal->m_value.is_init() && (m_internal->m_nativeType == CDocBuilderValue_Private::ptBool))
			return m_internal->m_nativeValue.bValue;

		if (IsEmpty() || !m_internal->m_value->isBool())
			return false;
		return m_internal->m_value->toBool();
	}
	int CDocBuilderValue::ToInt()
	{
		if (!m_internal->m_value.is_init())
		{
			if (m_internal->m_nativeType == CDocBuilderValue_Private::ptInt)
				return m_internal->m_nativeValue.nValue;
			if (m_internal->m_nativeType == CDocBuilderValue_Private::ptUInt)
				return (int)m_internal->m_nativeValue.unValue;
		}

		if (IsEmpty() || !m_internal->m_value->isNumber())
			return 0;
		return m_internal->m_value->toInt32();
	}
	double CDocBuilderValue::ToDouble()
	{
		if (!m_internal->m_value.is_init())
		{
			if (m_internal->m_nativeType == CDocBuilderValue_Private::ptDouble)
				return m_internal->m_nativeValue.dValue;
			if (m_internal->m_nativeType == CDocBuilderValue_Private::ptInt)
				return (double)m_internal->m_nativeValue.nValue;
			if (m_internal->m_nativeType == CDocBuilderValue_Private::ptUInt)
				return (double)m_internal->m_nativeValue.unValue;
		}

		if (IsEmpty() || !m_internal->m_value->isNumber())
			return 0;
		return m_internal->m_value->toDouble();
	}
	CString CDocBuilderValue::ToString()
	{
		CString ret;
		if (!m_internal->m_value.is_init() && (m_internal->m_nativeType == CDocBuilderValue_Private::ptString))
		{
			wchar_t* pValue = m_internal->m_nativeValue.sValue;
			size_t len = wcslen(pValue);
			wchar_t* buffer = new wchar_t[len + 1];
			memcpy(buffer, pValue, len * sizeof(wchar_t));
			buffer[len] = '\0';
			ret.m_internal->Attach(buffer);
			return ret;
		}

		if (IsEmpty() || !m_internal->m_value->isString())
			return ret;
		std::wstring sValue = m_internal->m_value->toStringW();
		if (sValue.empty())
			return ret;
		size_t len = sValue.length();
		wchar_t* buffer = new wchar_t[len + 1];
		memcpy(buffer, sValue.c_str(), len * sizeof(wchar_t));
		buffer[len] = '\0';
		ret.m_internal->Attach(buffer);
		return ret;
	}

	CDocBuilderValue CDocBuilderValue::Get(const char* name)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->get(name);

		ret.m_internal->m_parent = new CDocBuilderValue_Private::CParentValueInfo();
		ret.m_internal->m_parent->m_parent = m_internal->m_value;
		ret.m_internal->m_parent->m_parent_prop_name = std::string(name);
		ret.m_internal->m_parent->m_parent_index = -1;

		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Get(const wchar_t* name)
	{
		std::wstring sProp(name);
		std::string sPropA = U_TO_UTF8(sProp);
		return Get(sPropA.c_str());
	}
	CDocBuilderValue CDocBuilderValue::GetProperty(const wchar_t* name)
	{
		return Get(name);
	}
	CDocBuilderValue CDocBuilderValue::operator[](const char* name)
	{
		return Get(name);
	}
	CDocBuilderValue CDocBuilderValue::operator[](const wchar_t *name)
	{
		return Get(name);
	}

	CDocBuilderValue CDocBuilderValue::Get(const int& index)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isArray())
			return ret;

		ret.m_internal->m_context = m_internal->m_context;
		JSSmart<CJSArray> array = m_internal->m_value->toArray();
		ret.m_internal->m_value = array->get(index);

		ret.m_internal->m_parent = new CDocBuilderValue_Private::CParentValueInfo();
		ret.m_internal->m_parent->m_parent = m_internal->m_value;
		ret.m_internal->m_parent->m_parent_index = index;

		return ret;
	}
	CDocBuilderValue CDocBuilderValue::operator[](const int &index)
	{
		return Get(index);
	}

	void CDocBuilderValue::Set(const wchar_t* name, CDocBuilderValue value)
	{
		if (IsEmpty() || !m_internal->m_value->isObject())
			return;

		std::wstring sProp(name);
		std::string sPropA = U_TO_UTF8(sProp);

		value.m_internal->CheckNative();
		m_internal->m_value->toObjectSmart()->set(sPropA.c_str(), value.m_internal->m_value.GetPointer());
	}
	void CDocBuilderValue::SetProperty(const wchar_t* name, CDocBuilderValue value)
	{
		Set(name, value);
	}
	void CDocBuilderValue::Set(const int& index, CDocBuilderValue value)
	{
		if (IsEmpty() || !m_internal->m_value->isArray())
			return;

		JSSmart<CJSArray> array = m_internal->m_value->toArray();
		value.m_internal->CheckNative();
		array->set(index, value.m_internal->m_value.GetPointer());
	}

	// primitives
	CDocBuilderValue::CDocBuilderValue(const bool& value)
	{
		m_internal = new CDocBuilderValue_Private();
		m_internal->CreateBool(value);
	}
	CDocBuilderValue::CDocBuilderValue(const int& value)
	{
		m_internal = new CDocBuilderValue_Private();
		m_internal->CreateInt(value);
	}
	CDocBuilderValue::CDocBuilderValue(const unsigned int& value)
	{
		m_internal = new CDocBuilderValue_Private();
		m_internal->CreateUInt(value);
	}
	CDocBuilderValue::CDocBuilderValue(const double& value)
	{
		m_internal = new CDocBuilderValue_Private();
		m_internal->CreateDouble(value);
	}
	CDocBuilderValue::CDocBuilderValue(const char* value)
	{
		m_internal = new CDocBuilderValue_Private();
		m_internal->CreateString(value);
	}
	CDocBuilderValue::CDocBuilderValue(const wchar_t* value)
	{
		m_internal = new CDocBuilderValue_Private();
		m_internal->CreateString(value);
	}

	CDocBuilderValue CDocBuilderValue::CreateUndefined()
	{
		CDocBuilderValue ret;
		ret.m_internal->CreateUndefined();
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::CreateNull()
	{
		CDocBuilderValue ret;
		ret.m_internal->CreateNull();
		return ret;
	}

	// Functions
	CDocBuilderValue CDocBuilderValue::Call(const char* name)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(name);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		p1.m_internal->CheckNative();
		JSSmart<CJSValue> argv[1];
		argv[0] = p1.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(name, 1, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		p1.m_internal->CheckNative();
		p2.m_internal->CheckNative();
		JSSmart<CJSValue> argv[2];
		argv[0] = p1.m_internal->m_value;
		argv[1] = p2.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(name, 2, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		p1.m_internal->CheckNative();
		p2.m_internal->CheckNative();
		p3.m_internal->CheckNative();
		JSSmart<CJSValue> argv[3];
		argv[0] = p1.m_internal->m_value;
		argv[1] = p2.m_internal->m_value;
		argv[2] = p3.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(name, 3, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		p1.m_internal->CheckNative();
		p2.m_internal->CheckNative();
		p3.m_internal->CheckNative();
		p4.m_internal->CheckNative();
		JSSmart<CJSValue> argv[4];
		argv[0] = p1.m_internal->m_value;
		argv[1] = p2.m_internal->m_value;
		argv[2] = p3.m_internal->m_value;
		argv[3] = p4.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(name, 4, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		p1.m_internal->CheckNative();
		p2.m_internal->CheckNative();
		p3.m_internal->CheckNative();
		p4.m_internal->CheckNative();
		p5.m_internal->CheckNative();
		JSSmart<CJSValue> argv[5];
		argv[0] = p1.m_internal->m_value;
		argv[1] = p2.m_internal->m_value;
		argv[2] = p3.m_internal->m_value;
		argv[3] = p4.m_internal->m_value;
		argv[4] = p5.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(name, 5, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5, CDocBuilderValue p6)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		p1.m_internal->CheckNative();
		p2.m_internal->CheckNative();
		p3.m_internal->CheckNative();
		p4.m_internal->CheckNative();
		p5.m_internal->CheckNative();
		p6.m_internal->CheckNative();
		JSSmart<CJSValue> argv[6];
		argv[0] = p1.m_internal->m_value;
		argv[1] = p2.m_internal->m_value;
		argv[2] = p3.m_internal->m_value;
		argv[3] = p4.m_internal->m_value;
		argv[4] = p5.m_internal->m_value;
		argv[5] = p6.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(name, 6, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		std::wstring sProp(name);
		std::string sPropA = U_TO_UTF8(sProp);

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(sPropA.c_str());
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		std::wstring sProp(name);
		std::string sPropA = U_TO_UTF8(sProp);

		p1.m_internal->CheckNative();
		JSSmart<CJSValue> argv[1];
		argv[0] = p1.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(sPropA.c_str(), 1, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		std::wstring sProp(name);
		std::string sPropA = U_TO_UTF8(sProp);

		p1.m_internal->CheckNative();
		p2.m_internal->CheckNative();
		JSSmart<CJSValue> argv[2];
		argv[0] = p1.m_internal->m_value;
		argv[1] = p2.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(sPropA.c_str(), 2, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		std::wstring sProp(name);
		std::string sPropA = U_TO_UTF8(sProp);

		p1.m_internal->CheckNative();
		p2.m_internal->CheckNative();
		p3.m_internal->CheckNative();
		JSSmart<CJSValue> argv[3];
		argv[0] = p1.m_internal->m_value;
		argv[1] = p2.m_internal->m_value;
		argv[2] = p3.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(sPropA.c_str(), 3, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		std::wstring sProp(name);
		std::string sPropA = U_TO_UTF8(sProp);

		p1.m_internal->CheckNative();
		p2.m_internal->CheckNative();
		p3.m_internal->CheckNative();
		p4.m_internal->CheckNative();
		JSSmart<CJSValue> argv[4];
		argv[0] = p1.m_internal->m_value;
		argv[1] = p2.m_internal->m_value;
		argv[2] = p3.m_internal->m_value;
		argv[3] = p4.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(sPropA.c_str(), 4, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		std::wstring sProp(name);
		std::string sPropA = U_TO_UTF8(sProp);

		p1.m_internal->CheckNative();
		p2.m_internal->CheckNative();
		p3.m_internal->CheckNative();
		p4.m_internal->CheckNative();
		p5.m_internal->CheckNative();
		JSSmart<CJSValue> argv[5];
		argv[0] = p1.m_internal->m_value;
		argv[1] = p2.m_internal->m_value;
		argv[2] = p3.m_internal->m_value;
		argv[3] = p4.m_internal->m_value;
		argv[4] = p5.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(sPropA.c_str(), 5, argv);
		return ret;
	}
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5, CDocBuilderValue p6)
	{
		CDocBuilderValue ret;
		if (IsEmpty() || !m_internal->m_value->isObject())
			return ret;

		std::wstring sProp(name);
		std::string sPropA = U_TO_UTF8(sProp);

		p1.m_internal->CheckNative();
		p2.m_internal->CheckNative();
		p3.m_internal->CheckNative();
		p4.m_internal->CheckNative();
		p5.m_internal->CheckNative();
		p6.m_internal->CheckNative();
		JSSmart<CJSValue> argv[6];
		argv[0] = p1.m_internal->m_value;
		argv[1] = p2.m_internal->m_value;
		argv[2] = p3.m_internal->m_value;
		argv[3] = p4.m_internal->m_value;
		argv[4] = p5.m_internal->m_value;
		argv[5] = p6.m_internal->m_value;

		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = m_internal->m_value->toObjectSmart()->call_func(sPropA.c_str(), 6, argv);
		return ret;
	}

	CDocBuilderContext::CDocBuilderContext()
	{
		m_internal = new CDocBuilderContext_Private();
	}
	CDocBuilderContext::CDocBuilderContext(const CDocBuilderContext& src)
	{
		m_internal = new CDocBuilderContext_Private();
		m_internal->m_context = src.m_internal->m_context;
		m_internal->m_context_data = src.m_internal->m_context_data;
	}
	CDocBuilderContext& CDocBuilderContext::operator=(const CDocBuilderContext& src)
	{
		m_internal->m_context = src.m_internal->m_context;
		m_internal->m_context_data = src.m_internal->m_context_data;
		return *this;
	}
	CDocBuilderContext::~CDocBuilderContext()
	{
		RELEASEOBJECT(m_internal);
	}

	CDocBuilderValue CDocBuilderContext::CreateUndefined()
	{
		CDocBuilderValue ret;
		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = NSJSBase::CJSContext::createUndefined();
		return ret;
	}
	CDocBuilderValue CDocBuilderContext::CreateNull()
	{
		CDocBuilderValue ret;
		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = NSJSBase::CJSContext::createNull();
		return ret;
	}
	CDocBuilderValue CDocBuilderContext::CreateObject()
	{
		CDocBuilderValue ret;
		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = NSJSBase::CJSContext::createObject();
		return ret;
	}
	CDocBuilderValue CDocBuilderContext::CreateArray(const int& length)
	{
		CDocBuilderValue ret;
		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = NSJSBase::CJSContext::createArray(length);
		return ret;
	}
	CDocBuilderValue CDocBuilderContext::CreateTypedArray(unsigned char* buffer, const int& length, const bool& externalize)
	{
		CDocBuilderValue ret;
		ret.m_internal->m_context = m_internal->m_context;
		ret.m_internal->m_value = NSJSBase::CJSContext::createUint8Array(buffer, length, externalize);
		return ret;
	}

	unsigned char* CDocBuilderContext::AllocMemoryTypedArray(const size_t& size)
	{
		return NSJSBase::NSAllocator::Alloc(size);
	}

	void CDocBuilderContext::FreeMemoryTypedArray(unsigned char* data, const size_t& size)
	{
		return NSJSBase::NSAllocator::Free(data, size);
	}

	CDocBuilderValue CDocBuilderContext::GetGlobal()
	{
		CDocBuilderValue ret;
		ret.m_internal->m_context = m_internal->m_context;
		JSSmart<CJSObject> obj = m_internal->m_context->GetGlobal();
		ret.m_internal->m_value = obj->toValue();
		return ret;
	}

	CDocBuilderContextScope CDocBuilderContext::CreateScope()
	{
		CDocBuilderContextScope ret;
		ret.m_internal->m_scope_wrap = new CDocBuilderContextScopeWrap();
		ret.m_internal->m_scope_wrap->m_scope = m_internal->m_context->CreateContextScope();
		ret.m_internal->m_context_data = m_internal->m_context_data;

		m_internal->m_context_data->AddScope(ret.m_internal->m_scope_wrap);
		return ret;
	}

	bool CDocBuilderContext::IsError()
	{
		JSSmart<CJSTryCatch> oTry = m_internal->m_context->GetExceptions();
		return oTry->Check();
	}
}

namespace NSDoctRenderer
{
	void ParceParameters(const std::string& command, std::wstring* params, int& nCount)
	{
		const char* _commandsPtr = command.c_str();
		size_t _commandsLen = command.length();
		size_t _currentPos = 0;

		int nIndex = 0;

		while (true)
		{
			while (_currentPos < _commandsLen && !(_commandsPtr[_currentPos] == '\"' && _commandsPtr[_currentPos - 1] != '\\'))
				++_currentPos;

			++_currentPos;
			size_t _start = _currentPos;

			while (_currentPos < _commandsLen && !(_commandsPtr[_currentPos] == '\"' && _commandsPtr[_currentPos - 1] != '\\'))
				++_currentPos;

			if (_currentPos > _start)
			{
				if (_currentPos == (_start + 1))
					params[nIndex++] = L"";
				else
					params[nIndex++] = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)(_commandsPtr + _start), (LONG)(_currentPos - _start));
			}

			++_currentPos;

			if (_currentPos >= _commandsLen)
				break;
		}

		nCount = nIndex;
	}

	bool CDocBuilder::CreateFile(const int& type)
	{
		m_pInternal->m_nFileType = -1;
		if (!NSDirectory::Exists(m_pInternal->m_sTmpFolder))
			NSDirectory::CreateDirectory(m_pInternal->m_sTmpFolder);

		return m_pInternal->CreateFile(type);
	}
	void CDocBuilder::SetTmpFolder(const wchar_t* folder)
	{
		if (m_pInternal->m_bIsServerSafeVersion)
			return;
		m_pInternal->m_sTmpFolder = std::wstring(folder);
	}
	void CDocBuilder::CloseFile()
	{
		m_pInternal->CloseFile();
	}

	char* CDocBuilder::GetVersion()
	{
		m_pInternal->Init();

		if (0 == m_pInternal->m_arDoctSDK.size())
			return NULL;

		std::wstring sFile;
		for (std::vector<std::wstring>::iterator i = m_pInternal->m_arDoctSDK.begin(); i != m_pInternal->m_arDoctSDK.end(); i++)
		{
			if (std::wstring::npos != i->find(L"sdk-all-min.js"))
			{
				sFile = *i;
				break;
			}
		}

		if (sFile.empty())
			return NULL;

		std::string sData;
		if (!NSFile::CFileBinary::ReadAllTextUtf8A(sFile, sData))
			return NULL;

		std::string::size_type startPos = sData.find("Version:");
		if (std::string::npos == startPos)
			return NULL;

		startPos += 8;

		std::string::size_type endPos = sData.find(')', startPos);
		if (std::string::npos == endPos)
			return NULL;

		size_t sSrcLen = endPos - startPos + 1;
		if (sSrcLen == 0)
			return NULL;

		char* sRet = new char[sSrcLen + 1];
		memcpy(sRet, sData.c_str() + startPos, sSrcLen);
		sRet[sSrcLen] = '\0';
		return sRet;
	}

	bool CDocBuilder::Run(const wchar_t* path)
	{
		std::wstring sPath(path);
		if (!NSFile::CFileBinary::Exists(sPath))
			sPath = NSFile::GetProcessDirectory() + L"/" + sPath;

		std::string sCommands;
		bool bRet = NSFile::CFileBinary::ReadAllTextUtf8A(sPath, sCommands);

		if (!bRet)
		{
			CV8RealTimeWorker::_LOGGING_ERROR_(L"error", L"cannot read run file");
			return bRet;
		}

		return this->RunTextA(sCommands.c_str());
	}

	bool CDocBuilder::RunTextW(const wchar_t* commands)
	{
		std::wstring sCommandsW(commands);
		std::string sCommands = U_TO_UTF8(sCommandsW);
		return this->RunTextA(sCommands.c_str());
	}

	bool CDocBuilder::RunTextA(const char* commands)
	{
		m_pInternal->Init();
		std::list<std::string> _commands;
		size_t _commandsLen = strlen(commands);
		size_t _currentPos = 0;

		while (true)
		{
			while (_currentPos < _commandsLen && (commands[_currentPos] == 0x0d || commands[_currentPos] == 0x0a))
				++_currentPos;

			size_t _start = _currentPos;

			while (_currentPos < _commandsLen && (commands[_currentPos] != 0x0d && commands[_currentPos] != 0x0a))
				++_currentPos;

			if (_currentPos > _start)
			{
				size_t _start2 = _start;
				while (_start2 < _currentPos && (commands[_start2] == '\t' || commands[_start2] == ' '))
					++_start2;

				if (_currentPos > _start2 && (commands[_start2] != '#' && commands[_start2] != '/'))
				{
					_commands.push_back(std::string(commands + _start2, _currentPos - _start2));
					// DEBUG
					//std::cout << std::string(commands + _start2, _currentPos - _start2) << std::endl;
				}
			}

			if (_currentPos >= _commandsLen)
				break;
		}

		std::string sJsCommands = "";
		std::wstring _builder_params[4]; // с запасом
		for (std::list<std::string>::iterator i = _commands.begin(); i != _commands.end(); i++)
		{
			const std::string& command = *i;
			const char* _data = command.c_str();
			size_t _len = command.length();

			bool bIsBuilder = false;
			if (_len > 8)
			{
				if (_data[0] == 'b' &&
						_data[1] == 'u' &&
						_data[2] == 'i' &&
						_data[3] == 'l' &&
						_data[4] == 'd' &&
						_data[5] == 'e' &&
						_data[6] == 'r' &&
						_data[7] == '.')
					bIsBuilder = true;
			}

			bool bIsNoError = true;
			if (bIsBuilder)
			{
				if (!sJsCommands.empty())
				{
					std::wstring sUnicodeCommand = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sJsCommands.c_str(), (LONG)sJsCommands.length());
					bIsNoError = this->ExecuteCommand(sUnicodeCommand.c_str());
					sJsCommands = "";
					if (!bIsNoError)
						return false;
				}

				size_t _pos = 8;
				while (_data[_pos] != '(')
					++_pos;

				std::string sFuncNum(_data + 8, _pos - 8);
				int nCountParameters = 0;
				ParceParameters(command, _builder_params, nCountParameters);

				for (int nCheckParam = 0; nCheckParam < nCountParameters; ++nCheckParam)
				{
					if (0 == _builder_params[nCheckParam].find(L"jsValue(") && _builder_params[nCheckParam].length() > 9)
					{
						std::wstring sParam = _builder_params[nCheckParam].substr(8, _builder_params[nCheckParam].length() - 9);
						_builder_params[nCheckParam] = m_pInternal->m_pWorker->GetJSVariable(sParam);
					}
				}

				if ("OpenFile" == sFuncNum)
					bIsNoError = (0 == this->OpenFile(_builder_params[0].c_str(), _builder_params[1].c_str()));
				else if ("CreateFile" == sFuncNum)
				{
					if (L"docx" == _builder_params[0])
						bIsNoError = this->CreateFile(AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX);
					else if (L"pptx" == _builder_params[0])
						bIsNoError = this->CreateFile(AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX);
					else if (L"xlsx" == _builder_params[0])
						bIsNoError = this->CreateFile(AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX);
				}
				else if ("SetTmpFolder" == sFuncNum)
					this->SetTmpFolder(_builder_params[0].c_str());
				else if ("CloseFile" == sFuncNum)
					this->CloseFile();
				else if ("SaveFile" == sFuncNum)
				{
					int nFormat = NSDoctRenderer::GetFormatByTexExtention(_builder_params[0]);

					if (m_pInternal->m_oParams.m_bSaveWithDoctrendererMode)
					{
						// перед сохранением в такой схеме нужно скинуть изменения
						this->ExecuteCommand(L"Api.asc_Save();");
					}

					const wchar_t* sParams = NULL;
					if (nCountParameters > 2)
						sParams = _builder_params[2].c_str();

					this->SaveFile(nFormat, _builder_params[1].c_str(), sParams);
				}
				else if ("WriteData" == sFuncNum)
				{
					bool isAppend = true;
					if (nCountParameters > 2)
						isAppend = (L"true" == _builder_params[2]) ? true : false;

					this->WriteData(_builder_params[0].c_str(), _builder_params[1].c_str(), isAppend);
				}
			}
			else
			{
				//bIsNoError = this->m_pInternal->ExecuteCommand(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)_data, (LONG)_len));
				sJsCommands += command;
				sJsCommands += "\n";
			}

			if (!bIsNoError)
				return false;
		}

		if (!sJsCommands.empty())
		{
			// Такого быть не должно!!! Так как результат никуда не сохранится. пустое действие.
			std::wstring sUnicodeCommand = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sJsCommands.c_str(), (LONG)sJsCommands.length());
			bool bIsNoError = this->ExecuteCommand(sUnicodeCommand.c_str());
			sJsCommands = "";
			if (!bIsNoError)
				return false;
		}

		return true;
	}

	void CDocBuilder::SetProperty(const char* param, const wchar_t* value)
	{
		std::string sParam = std::string(param);
		if (sParam == "--use-doctrenderer-scheme")
			m_pInternal->m_oParams.m_bSaveWithDoctrendererMode = true;
		else if (sParam == "--check-fonts")
		{
			if (value && value[0] == '0')
				m_pInternal->m_oParams.m_bCheckFonts = false;
			else
				m_pInternal->m_oParams.m_bCheckFonts = true;
		}
		else if (sParam == "--work-directory")
			m_pInternal->m_oParams.m_sWorkDir = std::wstring(value);
		else if (sParam == "--cache-scripts")
			m_pInternal->m_bIsCacheScript = (std::wstring(value) == L"true");
		else if (sParam == "--save-use-only-names")
		{
			m_pInternal->m_bIsServerSafeVersion = true;
			m_pInternal->m_sFolderForSaveOnlyUseNames = std::wstring(value);
		}
		else if (sParam == "--all-fonts-path")
		{
			m_pInternal->SetAllFontsExternal(std::wstring(value));
		}
		else if (sParam == "--argument")
		{
			std::wstring sArg(value);
			m_pInternal->m_oParams.m_sArgumentJSON = U_TO_UTF8(sArg);
		}
		else if (sParam == "--fonts-system")
		{
			m_pInternal->m_oParams.m_bIsSystemFonts = (std::wstring(value) == L"true");
		}
		else if (sParam == "--fonts-dir")
		{
			m_pInternal->m_oParams.m_arFontDirs.push_back(std::wstring(value));
		}
	}
	void CDocBuilder::SetPropertyW(const wchar_t* param, const wchar_t* value)
	{
		std::wstring sW(param);
		std::string sA = U_TO_UTF8(sW);
		return this->SetProperty(sA.c_str(), value);
	}

	void CDocBuilder::Initialize(const wchar_t* directory)
	{
		std::wstring sDirectory = L"";
		if (directory)
			sDirectory = std::wstring(directory);

		NSDoctRenderer::CDocBuilder_Private::m_sExternalDirectory = sDirectory;
		CJSContext::ExternalInitialize(sDirectory);
	}

	void CDocBuilder::Dispose()
	{
		CJSContext::ExternalDispose();
	}

	void CDocBuilder::WriteData(const wchar_t* path, const wchar_t* value, const bool& append)
	{
		return m_pInternal->WriteData(path, value, append);
	}

	bool CDocBuilder::IsSaveWithDoctrendererMode()
	{
		return m_pInternal->m_oParams.m_bSaveWithDoctrendererMode;
	}
}
