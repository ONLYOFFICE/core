// DoctRenderer.h : Declaration of the CFontConverter

#pragma once
#include "resource.h"       // main symbols
#include "stdafx.h"
#include "NativeControl.h"

#include "../../../../../../Common/TimeMeasurer.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

void CreateNativeObject(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	
	v8::Handle<v8::ObjectTemplate> NativeObjectTemplate = CreateNativeControlTemplate(isolate);
	CNativeControl* pNativeObject = new CNativeControl();

	v8::Local<v8::Object> obj = NativeObjectTemplate->NewInstance();
	obj->SetInternalField(0, v8::External::New(pNativeObject));

	args.GetReturnValue().Set(obj);
}

void CreateNativeMemoryStream(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	
	v8::Handle<v8::ObjectTemplate> MemoryObjectTemplate = CreateMemoryStreamTemplate(isolate);
	CMemoryStream* pMemoryObject = new CMemoryStream();

	v8::Local<v8::Object> obj = MemoryObjectTemplate->NewInstance();
	obj->SetInternalField(0, v8::External::New(pMemoryObject));

	args.GetReturnValue().Set(obj);
}

// IDoctRenderer
[ object, uuid("353508C9-F3EA-4ceb-8AF6-A5FF4498998C"), dual, pointer_default(unique) ]
__interface IDoctRenderer : IDispatch
{
	[id(100)]	HRESULT Execute([in] BSTR bsXml, [out] BSTR* pbsError);
	
//----- Для дополнительных функций ----------------------------------------------------------------
	[id(10001)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT	ParamValue);
	[id(10002)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out] VARIANT *	ParamValue);
};

namespace DoctRendererFormat
{
	enum FormatFile
	{
		DOCT	= 0,
		XLST	= 1,
		PPTT	= 2,
		PDF		= 3,

		INVALID = 255
	};
}

class CExecuteParams
{
public:
	DoctRendererFormat::FormatFile m_eSrcFormat;
	DoctRendererFormat::FormatFile m_eDstFormat;

	CString m_strFontsDirectory;
	CString m_strImagesDirectory;
	CString m_strThemesDirectory;

	CString m_strSrcFilePath;
	CString m_strDstFilePath;

	CAtlArray<CString> m_arChanges;
	int m_nCountChangesItems;

public:
	CExecuteParams() : m_arChanges()
	{
		m_eSrcFormat = DoctRendererFormat::INVALID;
		m_eDstFormat = DoctRendererFormat::INVALID;

		m_strFontsDirectory = _T("");
		m_strImagesDirectory = _T("");
		m_strThemesDirectory = _T("");

		m_strSrcFilePath = _T("");
		m_strDstFilePath = _T("");

		m_nCountChangesItems = -1;
	}
	~CExecuteParams()
	{
		m_arChanges.RemoveAll();
	}

public:
	
	BOOL FromXml(BSTR bsXml)
	{
		CString strXml = (CString)bsXml;
		XmlUtils::CXmlNode oNode;
		if (!oNode.FromXmlString(strXml))
			return FALSE;

		m_strSrcFilePath = oNode.ReadValueString(_T("SrcFilePath"));
		m_strDstFilePath = oNode.ReadValueString(_T("DstFilePath"));

		m_eSrcFormat = (DoctRendererFormat::FormatFile)(oNode.ReadValueInt(_T("SrcFileType")));
		m_eDstFormat = (DoctRendererFormat::FormatFile)(oNode.ReadValueInt(_T("DstFileType")));

		m_strFontsDirectory = oNode.ReadValueString(_T("FontsDirectory"));
		m_strImagesDirectory = oNode.ReadValueString(_T("ImagesDirectory"));
		m_strThemesDirectory = oNode.ReadValueString(_T("ThemesDirectory"));

		XmlUtils::CXmlNode oNodeChanges;
		if (oNode.GetNode(_T("Changes"), oNodeChanges))
		{
			m_nCountChangesItems = oNodeChanges.ReadAttributeInt(_T("TopItem"), -1);

			XmlUtils::CXmlNodes oNodes;
			oNodeChanges.GetNodes(_T("Change"), oNodes);

			int nCount = oNodes.GetCount();
			for (int i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode _node;
				oNodes.GetAt(i, _node);

				m_arChanges.Add(_node.GetText());
			}
		}

		return TRUE;
	}
};

// CDoctRenderer
[coclass, default(IDoctRenderer), threading(apartment), vi_progid("DoctRenderer.DoctRenderer"), progid("DoctRenderer.DoctRenderer.1"), version(1.0), uuid("E5FD7681-B077-48df-93E3-0345B6DBE8EE") ]
class ATL_NO_VTABLE CDoctRenderer : public IDoctRenderer
{
private:
	// конвертация и применение изменений
	CExecuteParams m_oParams;
	IASCRenderer* m_pRenderer;

	CString m_strConfigDir;
	CString m_strConfigPath;
	CAtlArray<CString> m_arrFiles;

	CString m_strDoctSDK;
	CString m_strPpttSDK;
	CString m_strXlstSDK;

	CString m_strEditorType;
	CString m_strFilePath;

	BOOL	m_bIsInitTypedArrays;
	
public:
	CDoctRenderer()
	{
		m_pRenderer = NULL;
		m_bIsInitTypedArrays = FALSE;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_strConfigDir = _T("");
		m_strConfigPath = _T("");

        HINSTANCE hModule  = _AtlBaseModule.GetModuleInstance();
        TCHAR szPathDLL[MAX_PATH] = {0}; ::GetModuleFileName(hModule, szPathDLL, MAX_PATH);

		m_strConfigDir = CString(szPathDLL);

		int nFind = m_strConfigDir.ReverseFind(TCHAR('\\'));
		if (-1 != nFind)
			m_strConfigDir.Delete(nFind + 1, m_strConfigDir.GetLength() - nFind - 1);

		m_strConfigPath = m_strConfigDir + _T("DoctRenderer.config");

		XmlUtils::CXmlNode oNode;
		if (oNode.FromXmlFile(m_strConfigPath))
		{
			XmlUtils::CXmlNodes oNodes;
			if (oNode.GetNodes(_T("file"), oNodes))
			{
				int nCount = oNodes.GetCount();
				XmlUtils::CXmlNode _node;
				for (int i = 0; i < nCount; ++i)
				{
					oNodes.GetAt(i, _node);
					CString strFilePath = _node.GetText();

					if (IsFileExists(strFilePath))
						m_arrFiles.Add(_node.GetText());
					else
						m_arrFiles.Add(m_strConfigDir + strFilePath);
				}
			}
		}

		m_strDoctSDK = _T("");
		m_strPpttSDK = _T("");
		m_strXlstSDK = _T("");

		XmlUtils::CXmlNode oNodeSdk = oNode.ReadNode(_T("DoctSdk"));
		if (oNodeSdk.IsValid())
			m_strDoctSDK = oNodeSdk.GetText();

		oNodeSdk = oNode.ReadNode(_T("PpttSdk"));
		if (oNodeSdk.IsValid())
			m_strPpttSDK = oNodeSdk.GetText();

		oNodeSdk = oNode.ReadNode(_T("XlstSdk"));
		if (oNodeSdk.IsValid())
			m_strXlstSDK = oNodeSdk.GetText();

		if (!IsFileExists(m_strDoctSDK))
			m_strDoctSDK = m_strConfigDir + m_strDoctSDK;

		if (!IsFileExists(m_strPpttSDK))
			m_strPpttSDK = m_strConfigDir + m_strPpttSDK;

		if (!IsFileExists(m_strXlstSDK))
			m_strXlstSDK = m_strConfigDir + m_strXlstSDK;
		
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(SetAdditionalParam)(BSTR bsParamName, VARIANT vParamValue)
	{
		/*
		CString sParamName; sParamName = bsParamName;
		if (_T("Parent") == sParamName && vParamValue.punkVal != NULL)
		{		
			vParamValue.punkVal->QueryInterface(AVSGraphics::IID_IAVSDocumentPainter, (void**)&m_pPainter);			
		}
		*/
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR bsParamName, VARIANT *pvParamValue)
	{
		return S_OK;
	}

	STDMETHOD(Execute)(BSTR bsXml, BSTR* pbsError)
	{
		*pbsError = NULL;
		m_oParams.FromXml(bsXml);

		BOOL bIsInnerFonts = FALSE;
		if (m_oParams.m_strFontsDirectory == _T(""))
			bIsInnerFonts = TRUE;

		CString strMainPart = _T("");
		for (size_t i = 0; i < m_arrFiles.GetCount(); ++i)
		{
			if (bIsInnerFonts && (m_arrFiles[i].Find(_T("AllFonts.js")) != -1))
				continue;

			strMainPart += ReadScriptFile(m_arrFiles[i]);
			strMainPart += _T("\n\n");
		}

		CString strCorrector = _T("");

		CString sResourceFile;
		switch (m_oParams.m_eSrcFormat)
		{
		case DoctRendererFormat::DOCT:
			{
				switch (m_oParams.m_eDstFormat)
				{
				case DoctRendererFormat::DOCT:
				case DoctRendererFormat::PDF:
					{
						sResourceFile = m_strDoctSDK;
						m_strEditorType = _T("document");
						break;
					}
				default:
					return S_FALSE;
				}
				break;
			}
		case DoctRendererFormat::PPTT:
			{
				switch (m_oParams.m_eDstFormat)
				{
				case DoctRendererFormat::PPTT:
				case DoctRendererFormat::PDF:
					{
						sResourceFile = m_strPpttSDK;
						m_strEditorType = _T("presentation");
						break;
					}
				default:
					return S_FALSE;
				}
				break;
			}
		case DoctRendererFormat::XLST:
			{
				switch (m_oParams.m_eDstFormat)
				{
				case DoctRendererFormat::XLST:
				case DoctRendererFormat::PDF:
					{
						sResourceFile = m_strXlstSDK;
						m_strEditorType = _T("spreadsheet");
						break;
					}
				default:
					return S_FALSE;
				}
				break;
			}
		default:
			return S_FALSE;
		}

		if (bIsInnerFonts)
		{
			ASCGraphics::IASCFontManager* pFontManager = NULL;
			CoCreateInstance(ASCGraphics::CLSID_CASCFontManager, NULL, CLSCTX_ALL, ASCGraphics::IID_IASCFontManager, (void**)&pFontManager);
			pFontManager->Initialize(L"");
			VARIANT var;
			pFontManager->GetAdditionalParam(L"AllFonts.js", &var);
			CString strAllFonts = (CString)var.bstrVal;
			SysFreeString(var.bstrVal);

			strMainPart += strAllFonts;
		}

		CString strFileName = m_oParams.m_strSrcFilePath;
		strFileName += _T("\\");
		strFileName.Replace(_T("/"), _T("\\"));
		strFileName.Replace(_T("\\\\"), _T("\\"));
		strFileName.Replace(_T("\\\\"), _T("\\"));

		strFileName.Replace(_T("\\"), _T("\\\\"));
		strFileName += _T("Editor.bin");
		
		m_strFilePath = strFileName;
		
		CString strScript = strMainPart;
		strScript += ReadScriptFile(sResourceFile);

		if (m_strEditorType == _T("spreadsheet"))
			strScript += _T("\n$.ready();");

		CString strError = _T("");
		BOOL bResult = ExecuteScript(strScript, strError);

		if (_T("") != strError)
		{
			CString sDestError = _T("<result><error ") + strError + _T(" /></result>");
			*pbsError = sDestError.AllocSysString();
		}

		return bResult ? S_OK : S_FALSE;
	}

private:
	WCHAR* LoadResourceFile(HINSTANCE hInst, LPCTSTR sResName, LPCTSTR sResType)
	{		
		HRSRC hrRes = FindResource(hInst, sResName, sResType);
		if (!hrRes)
			return NULL;

		HGLOBAL hGlobal = LoadResource(hInst, hrRes);
		DWORD sz = SizeofResource(hInst, hrRes);
		void* ptrRes = LockResource(hGlobal);
		
		// utf bom
		WCHAR* pUnicodeString = GetCStringFromUTF8(((BYTE*)ptrRes) + 3, (LONG)sz - 3);

		UnlockResource(hGlobal);
		FreeResource(hGlobal);

		return pUnicodeString;
	}

	WCHAR* GetCStringFromUTF8( BYTE* pBuffer, LONG lCount )
	{
		LONG lLenght = 0;

		WCHAR* pUnicodeString = new WCHAR[lCount + 1];
		LONG lIndexUnicode = 0;

		for (LONG lIndex = 0; lIndex < lCount; ++lIndex)
		{
			if (0x00 == (0x80 & pBuffer[lIndex]))
			{
				//strRes += (TCHAR)pBuffer[lIndex];
				pUnicodeString[lIndexUnicode++] = (WCHAR)pBuffer[lIndex];
				continue;
			}
			else if (0x00 == (0x20 & pBuffer[lIndex]))
			{
				WCHAR mem = (WCHAR)(((pBuffer[lIndex] & 0x1F) << 6) + (pBuffer[lIndex + 1] & 0x3F));
				
				//strRes += mem;
				pUnicodeString[lIndexUnicode++] = mem;

				lIndex += 1;
			}
			else if (0x00 == (0x10 & pBuffer[lIndex]))
			{
				WCHAR mem = (WCHAR)(((pBuffer[lIndex] & 0x0F) << 12) + ((pBuffer[lIndex + 1] & 0x3F) << 6) + (pBuffer[lIndex + 2] & 0x3F));
				
				//strRes += mem;
				pUnicodeString[lIndexUnicode++] = mem;

				lIndex += 2;
			}
			else
			{
				BYTE mem = pBuffer[lIndex];
				//pUnicodeString[lIndexUnicode++] = mem;
			}
		}

		pUnicodeString[lIndexUnicode] = 0;
		
		return pUnicodeString;
	}


	CString ReadScriptFile(const CString& strFile)
	{
		CFile oFile;

		HRESULT hr = oFile.OpenFile(strFile);
		if (S_OK != hr)
			return _T("");

		int nSize = (int)oFile.GetFileSize();
		if (nSize < 3)
			return _T("");

		BYTE* pData = new BYTE[nSize];
		oFile.ReadFile(pData, (DWORD)nSize);
		CString strResult = _T("");

		if (pData[0] == 0xEF && pData[1] == 0xBB && pData[2] == 0xBF)
		{
			WCHAR* pUnicode = GetCStringFromUTF8(pData + 3, nSize - 3);
			strResult = CString(pUnicode);
			RELEASEARRAYOBJECTS(pUnicode);
		}
		else
		{
			strResult = CString((char*)pData, nSize);
		}

		RELEASEARRAYOBJECTS(pData);
		return strResult;
	}

	bool IsFileExists(LPCTSTR path)
	{
		WIN32_FIND_DATA findData;
		ZeroMemory(&findData, sizeof(findData));

		HANDLE handle = ::FindFirstFile(path, &findData);

		bool fileExists = true;
		if (handle == INVALID_HANDLE_VALUE)
			fileExists = false;
		
		FindClose(handle);

		return fileExists;
	}

private:
	BOOL ExecuteScript(CString& strScript, CString& strError)
	{
		CString strException = _T("");

		if (!m_bIsInitTypedArrays)
		{
			enableTypedArrays();
			m_bIsInitTypedArrays = TRUE;
		}

		WCHAR* javascript = (WCHAR*)strScript.GetBuffer();

		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::HandleScope handle_scope(isolate);

		v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
		global->Set(v8::String::NewSymbol("CreateNativeEngine"), v8::FunctionTemplate::New(CreateNativeObject));
		global->Set(v8::String::NewSymbol("CreateNativeMemoryStream"), v8::FunctionTemplate::New(CreateNativeMemoryStream));
		
		v8::Local<v8::Context> context = v8::Context::New(isolate, NULL, global);
		v8::Context::Scope context_scope(context);
		v8::TryCatch try_catch;
		v8::Local<v8::String> source = v8::String::New((uint16_t*)javascript);
		v8::Local<v8::Script> script = v8::Script::Compile(source);

		if (try_catch.HasCaught()) 
		{
			CString strCode = to_cstring(try_catch.Message()->GetSourceLine());
			strException = to_cstring(try_catch.Message()->Get());

			strError = _T("code=\"compile\"");
			return FALSE;
		}
		
		v8::Local<v8::Value> result = script->Run();
		
		if (try_catch.HasCaught()) 
		{
			CString strCode = to_cstring(try_catch.Message()->GetSourceLine());
			strException = to_cstring(try_catch.Message()->Get());

			strError = _T("code=\"run\"");
			return FALSE;
		}

		//---------------------------------------------------------------
		v8::Local<v8::Object> global_js = context->Global();
		v8::Handle<v8::Value> args[1];
		args[0] = v8::Int32::New(0);

		// all
		v8::Handle<v8::Value> js_func_open			= global_js->Get(v8::String::New("NativeOpenFile"));
		v8::Handle<v8::Value> js_func_id			= global_js->Get(v8::String::New("GetNativeId"));

		// changes
		v8::Handle<v8::Value> js_func_apply_changes	= global_js->Get(v8::String::New("NativeApplyChanges"));

		// save T format
		v8::Handle<v8::Value> js_func_get_file_s	= global_js->Get(v8::String::New("NativeGetFileString"));

		// pdf
		v8::Handle<v8::Value> js_func_calculate		= global_js->Get(v8::String::New("NativeCalculateFile"));
		v8::Handle<v8::Value> js_func_pages_count	= global_js->Get(v8::String::New("GetNativeCountPages"));
		v8::Handle<v8::Value> js_func_page			= global_js->Get(v8::String::New("GetNativePageBase64"));

		CString strDocumentId	= _T("");
		LONG lPagesCount		= 0;

		if (js_func_id->IsFunction()) 
		{
			v8::Handle<v8::Function> func_id = v8::Handle<v8::Function>::Cast(js_func_id);
			v8::Local<v8::Value> js_result2 = func_id->Call(global_js, 1, args);
			
			if (try_catch.HasCaught()) 
			{
				int nLineError = try_catch.Message()->GetLineNumber();
				strException = to_cstring(try_catch.Message()->Get()); // ошибка компиляции? исключение бросаем

				strError = _T("code=\"run\"");
				return FALSE;
			}

			strDocumentId = to_cstring(js_result2);
		}

		CNativeControl* pNative = NULL;

		v8::Handle<v8::Value> js_func_get_native = global_js->Get(v8::String::New("GetNativeEngine"));
		v8::Local<v8::Object> objNative;
		if (js_func_get_native->IsFunction()) 
		{
			v8::Handle<v8::Function> func_get_native = v8::Handle<v8::Function>::Cast(js_func_get_native);
			v8::Local<v8::Value> js_result2 = func_get_native->Call(global_js, 1, args);
			
			if (try_catch.HasCaught()) 
			{
				int nLineError = try_catch.Message()->GetLineNumber();
				strException = to_cstring(try_catch.Message()->Get()); // ошибка компиляции? исключение бросаем
				
				isolate->Exit();
				
				strError = _T("code=\"run\"");
				return FALSE;
			}

			objNative = js_result2->ToObject();
			v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(objNative->GetInternalField(0));
			
			pNative = static_cast<CNativeControl*>(field->Value());
		}

		pNative->m_pChanges				= &m_oParams.m_arChanges;
		pNative->m_strFontsDirectory	= m_oParams.m_strFontsDirectory;
		pNative->m_strImagesDirectory	= m_oParams.m_strImagesDirectory;

		pNative->m_strEditorType		= m_strEditorType;
		pNative->SetFilePath(m_strFilePath);

		pNative->m_nMaxChangesNumber	= m_oParams.m_nCountChangesItems;

		if (js_func_open->IsFunction()) 
		{
			v8::Handle<v8::Function> func_open = v8::Handle<v8::Function>::Cast(js_func_open);

			func_open->Call(global_js, 1, args);
			
			if (try_catch.HasCaught()) 
			{
				CString strCode = to_cstring(try_catch.Message()->GetSourceLine());
				strException = to_cstring(try_catch.Message()->Get()); // ошибка компиляции? исключение бросаем

				strError = _T("code=\"open\"");
				return FALSE;
			}
		}

		if (m_oParams.m_arChanges.GetCount() != 0)
		{
			if (js_func_apply_changes->IsFunction()) 
			{
				v8::Handle<v8::Function> func_apply_changes = v8::Handle<v8::Function>::Cast(js_func_apply_changes);
				func_apply_changes->Call(global_js, 1, args);
				
				if (try_catch.HasCaught()) 
				{
					int nLineError = try_catch.Message()->GetLineNumber();
					CString strCode = to_cstring(try_catch.Message()->GetSourceLine());
					strException = to_cstring(try_catch.Message()->Get()); // ошибка компиляции? исключение бросаем

					strError = _T("");
					strError.Format(_T("index=\"%d\""), pNative->m_nCurrentChangesNumber);
					return FALSE;
				}
			}
		}

		switch (m_oParams.m_eDstFormat)
		{
		case DoctRendererFormat::DOCT:
		case DoctRendererFormat::PPTT:
		case DoctRendererFormat::XLST:
			{
				if (js_func_get_file_s->IsFunction()) 
				{
					v8::Handle<v8::Function> func_get_file_s = v8::Handle<v8::Function>::Cast(js_func_get_file_s);
					v8::Local<v8::Value> js_result2 = func_get_file_s->Call(global_js, 1, args);
					
					if (try_catch.HasCaught()) 
					{
						int nLineError = try_catch.Message()->GetLineNumber();
						CString strCode = to_cstring(try_catch.Message()->GetSourceLine());
						strException = to_cstring(try_catch.Message()->Get()); // ошибка компиляции? исключение бросаем

						strError = _T("code=\"save\"");
						return FALSE;
					}

					CStringA strSave = to_cstringA(js_result2);
					CFile oFile;
					if (S_OK == oFile.CreateFile(m_oParams.m_strDstFilePath))
					{
						oFile.WriteFile((void*)strSave.GetBuffer(), (DWORD)strSave.GetLength());
						oFile.CloseFile();
						return TRUE;
					}
				}
				break;
			}
		case DoctRendererFormat::PDF:
			{
				if (js_func_calculate->IsFunction()) 
				{
					v8::Handle<v8::Function> func_calculate = v8::Handle<v8::Function>::Cast(js_func_calculate);
					func_calculate->Call(global_js, 1, args);
					
					if (try_catch.HasCaught()) 
					{
						int nLineError = try_catch.Message()->GetLineNumber();
						CString strCode = to_cstring(try_catch.Message()->GetSourceLine());
						strException = to_cstring(try_catch.Message()->Get()); // ошибка компиляции? исключение бросаем

						strError = _T("code=\"calculate\"");
						return FALSE;
					}
				}

				LONG lPagesCount = 0;
				if (js_func_pages_count->IsFunction()) 
				{
					v8::Handle<v8::Function> func_pages_count = v8::Handle<v8::Function>::Cast(js_func_pages_count);
					v8::Local<v8::Value> js_result1 = func_pages_count->Call(global_js, 1, args);
					
					if (try_catch.HasCaught()) 
					{
						CString strCode = to_cstring(try_catch.Message()->GetSourceLine());
						strException = to_cstring(try_catch.Message()->Get()); // ошибка компиляции? исключение бросаем

						strError = _T("code=\"calculate\"");
						return FALSE;
					}

					v8::Local<v8::Int32> intValue = js_result1->ToInt32();
					lPagesCount = (LONG)intValue->Value();
				}

				if (js_func_page->IsFunction()) 
				{
					PDFWriter::IPDFWriter* pPDF = NULL;
					CoCreateInstance(PDFWriter::CLSID_CPDFWriter, NULL, CLSCTX_ALL, PDFWriter::IID_IPDFWriter, (void**)&pPDF);
					VARIANT var;
					var.vt = VT_BSTR;
					var.bstrVal = m_oParams.m_strFontsDirectory.AllocSysString();
					pPDF->SetAdditionalParam(L"InitializeFromFolder", var);
					SysFreeString(var.bstrVal);

					pPDF->CreatePDF();
					pPDF->SetPDFCompressionMode(15);
					
					RELEASEINTERFACE(m_pRenderer);
					pPDF->QueryInterface(__uuidof(IASCRenderer), (void**)&m_pRenderer);

					v8::Handle<v8::Function> func_page = v8::Handle<v8::Function>::Cast(js_func_page);
					
					for (LONG i = 0; i < lPagesCount; i++)
					{
						args[0] = v8::Int32::New(i);
						v8::Local<v8::Value> js_result3 = func_page->Call(global_js, 1, args);

						if (try_catch.HasCaught()) 
						{
							int nLineError = try_catch.Message()->GetLineNumber();
							CString strCode = to_cstring(try_catch.Message()->GetSourceLine());
							strException = to_cstring(try_catch.Message()->Get()); // ошибка компиляции? исключение бросаем

							strError = _T("code=\"render\"");
							return FALSE;
						}

						CMemoryStream* pPageStream = unwrap_memorystream(js_result3->ToObject());
						ParsePageBinary(i, pPageStream->GetData(), pPageStream->GetSize(), true);

#if 0
						CFile oFile;
						CString sPageSave = _T("");
						sPageSave.Format(_T("C:\\test\\DOCTRENDERER\\page%d.pagebin"), i + 1);
						oFile.CreateFile(sPageSave);
						oFile.WriteFile(pPageStream->GetData(), pPageStream->GetSize());
						oFile.CloseFile();
#endif
					}

					RELEASEINTERFACE(m_pRenderer);

					BSTR bsFileDst = m_oParams.m_strDstFilePath.AllocSysString();
					HRESULT hr = pPDF->SaveToFile(bsFileDst);
					SysFreeString(bsFileDst);

					RELEASEINTERFACE(pPDF);

					if (S_OK != hr)
						strError = _T("code=\"save\"");

					return (hr == S_OK) ? TRUE : FALSE;
				}

				break;
			}
		default:
			break;
		}
		

		return FALSE;
	}

	void ParsePageBinary(LONG i, BYTE* pOutput, int lOutputLen, bool bIsPDF = false)
	{
		int* m = NULL;
		USHORT* ms = NULL;
        int _sLen = 0;
        CString s = _T("");

        double m1 = 0;
        double m2 = 0;
        double m3 = 0;
        double m4 = 0;
        double m5 = 0;
        double m6 = 0;

		CString imgPath = _T("");
		CString base64Temp = _T("");

		bool bIsPathOpened = false;
		int nCountPages = 0;
		int curindex = 0;
		BYTE* current = pOutput;
		CommandType eCommand;
        while (curindex < lOutputLen)
        {
            eCommand = (CommandType)(*current);
            current++;
            curindex++;
            switch (eCommand)
            {
                case ctPageWidth:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;
					m_pRenderer->put_Width((*m) / 100000.0);
                    break;
				}
                case ctPageHeight:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;
					m_pRenderer->put_Height((*m) / 100000.0);
                    break;
				}
                case ctPageStart:
				{
					m_pRenderer->NewPage();
					m_pRenderer->BeginCommand(1);
                    ++nCountPages;
                    break;
				}
                case ctPageEnd:
				{
					if (bIsPathOpened)
                    {
						m_pRenderer->PathCommandEnd();
						m_pRenderer->EndCommand(4);                        
                    }

                    bIsPathOpened = false;
					m_pRenderer->EndCommand(1);
                    break;
				}
                case ctPenColor:
				{
                    m = (int*)current;
					m_pRenderer->put_PenColor(*m);
                    current += 4;
                    curindex += 4;
                    break;
				}
                case ctPenAlpha:
				{
					m_pRenderer->put_PenAlpha(*current);                    
                    current++;
                    curindex++;
                    break;
				}
                case ctPenSize:
				{
                    m = (int*)current;
					m_pRenderer->put_PenSize(*m / 100000.0);
                    current += 4;
                    curindex += 4;
                    break;
				}
                case ctPenLineJoin:
				{
					m_pRenderer->put_PenLineJoin(*current);                    
                    current++;
                    curindex++;
                    break;
				}
                case ctBrushType:
				{
                    m = (int*)current;
					m_pRenderer->put_BrushType(*m);
                    current += 4;
                    curindex += 4;
                    break;
				}
                case ctBrushColor1:
				{
                    m = (int*)current;
					m_pRenderer->put_BrushColor1(*m);
                    current += 4;
                    curindex += 4;
                    break;
				}
                case ctBrushAlpha1:
				{
					m_pRenderer->put_BrushAlpha1(*current);
                    current++;
                    curindex++;
                    break;
				}
                case ctBrushColor2:
				{
                    m = (int*)current;
					m_pRenderer->put_BrushColor2(*m);
                    current += 4;
                    curindex += 4;
                    break;
				}
                case ctBrushAlpha2:
				{
                    m_pRenderer->put_BrushAlpha2(*current);
                    current++;
                    curindex++;
                    break;
				}
                case ctBrushRectable:
				{
                    m = (int*)current;
                    current += 4 * 4;
                    curindex += 4 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    m3 = (*m++) / 100000.0;
                    m4 = (*m++) / 100000.0;

					// TODO:
					//m_pRenderer->BrushRect(0, m1, m2, m3, m4);                    
                    break;
				}
                case ctBrushRectableEnabled:
				{
                    VARIANT var;
					var.vt = VT_BOOL;
					var.boolVal = (1 == *current) ? VARIANT_TRUE : VARIANT_FALSE;
                    m_pRenderer->SetAdditionalParam(L"BrushFillBoundsEnable", var);

                    current += 1;
                    curindex += 1;
                    break;
				}
                case ctBrushTexturePath:
				{
					ms = (USHORT*)current;
                    current += 2;
                    curindex += 2;

                    _sLen = (int)(*ms);
                    CString s((WCHAR*)current, _sLen);
                    imgPath = s;

					if (0 != s.Find(_T("http:")) &&
                        0 != s.Find(_T("https:")) &&
                        0 != s.Find(_T("ftp:")) &&
                        0 != s.Find(_T("file:")))
                    {
						imgPath = m_oParams.m_strImagesDirectory + _T("\\") + s;

                        int _len = imgPath.GetLength();
                        int ind = imgPath.Find(_T(".svg"));
                        if (ind != -1)
                        {
                            if (ind == (_len - 4))
                            {
                                CString sInterest = imgPath.Mid(0, ind);

								CFile oFile;
								if (oFile.OpenFile(sInterest + _T(".emf")) == S_OK)
                                    imgPath = sInterest + _T(".emf");
								else if (oFile.OpenFile(sInterest + _T(".wmf")))
                                    imgPath = sInterest + _T(".wmf");
                            }
                        }
                    }

                    base64Temp = _T("");
                    if (0 == s.Find(_T("data:")))
                    {
                        // TODO:
                    }

                    current += 2 * _sLen;
                    curindex += 2 * _sLen;

					BSTR bsPath = imgPath.AllocSysString();
					m_pRenderer->put_BrushTexturePath(bsPath);
					SysFreeString(bsPath);
                    break;
				}
                case ctBrushGradient:
				{
                    current++;
                    curindex++;

                    CString strAttrMain = _T("");
                    CString strColors = _T("");
                    bool bIsLinear = true;

                    while (true)
                    {
                        BYTE _command = *current;
                        current++;
                        curindex++;

                        if (251 == _command)
                            break;

						LONG _R = 0;
						LONG _G = 0;
						LONG _B = 0;
						LONG _A = 0;


                        switch (_command)
                        {
                            case 0:
                                {
                                    current += 5;
                                    curindex += 5;

                                    m = (int*)current;
                                    current += 4 * 4;
                                    curindex += 4 * 4;

                                    double d1 = (*m++) / 100000.0;
                                    double d2 = (*m++) / 100000.0;
                                    double d3 = (*m++) / 100000.0;
                                    double d4 = (*m++) / 100000.0;

                                    strAttrMain.Format(_T("x1=\"%.2lf\" y1=\"%.2lf\" x2=\"%.2lf\" y2=\"%.2lf\" gradientUnits=\"userSpaceOnUse\""), d1, d2, d3, d4);
                                    strAttrMain.Replace(_T(","), _T("."));
                                    break;
                                }
                            case 1:
                                {
                                    bIsLinear = false;

                                    current++;
                                    curindex++;

                                    m = (int*)current;
                                    current += 6 * 4;
                                    curindex += 6 * 4;

                                    double d1 = (*m++) / 100000.0;
                                    double d2 = (*m++) / 100000.0;
                                    double d3 = (*m++) / 100000.0;
                                    double d4 = (*m++) / 100000.0;
                                    double d5 = (*m++) / 100000.0;
                                    double d6 = (*m++) / 100000.0;

                                    strAttrMain.Format(_T("cx=\"%.2lf\" cy=\"%.2lf\" r0=\"%.2lf\" r1=\"%.2lf\" rx=\"%.2lf\" ry=\"%.2lf\" gradientUnits=\"userSpaceOnUse\""), d1, d2, d5, d6, d1, d2);
                                    strAttrMain.Replace(_T(","), _T("."));
                                    break;
                                }
                            case 2:
                                {
                                    int nCountColors = *((int*)current);

                                    current += 4;
                                    curindex += 4;

                                    for (int nI = 0; nI < nCountColors; ++nI)
                                    {
                                        int pos = *((int*)current);
                                        current += 4;
                                        curindex += 4;

                                        double dPos = pos / 100000.0;
                                        BYTE _r = *current++;
                                        BYTE _g = *current++;
                                        BYTE _b = *current++;
                                        BYTE _a = *current++;

										_R += _r;
										_G += _g;
										_B += _b;
										_A += _a;

                                        curindex += 4;

                                        int _color = ((_b << 16) & 0xFF0000) | ((_g << 8) & 0xFF00) | _r;
                                        CString sColor;
										sColor.Format(_T("<stop stop-color=\"%d\" stop-opacity=\"%.2lf\" offset=\"%.2lf\" />"), _color, _a / 255.0, dPos);

                                        sColor.Replace(_T(","), _T("."));
                                        strColors += sColor;
                                    }

									if (nCountColors != 0)
									{
										// TODO:
										m_pRenderer->put_BrushType(1000);

										_R = (BYTE)(_R / nCountColors);
										_G = (BYTE)(_G / nCountColors);
										_B = (BYTE)(_B / nCountColors);
										_A = (BYTE)(_A / nCountColors);

										LONG _Color = _B * 256 * 256 + _G * 256 + _R;
										m_pRenderer->put_BrushColor1(_Color);
										m_pRenderer->put_BrushAlpha1(_A);
									}

                                    break;
                                }
                            default:
                                break;
                        };
                    }

                    CString strXml = _T("");

					// TODO:
                    if (bIsLinear)
                    {
                        strXml = _T("<linearGradient ") + strAttrMain + _T(">") + strColors + _T("</linearGradient>");
						//m_pRenderer->SetAdditionalParam(L"Fill-LinearGradient", strXml);						
                    }
                    else
                    {
                        strXml = _T("<radialGradient ") + strAttrMain + _T(">") + strColors + _T("</radialGradient>");
                        //m_pRenderer->SetAdditionalParam(L"Fill-RadialGradient", strXml);
                    }                                        

                    break;
				}
                case ctBrushTextureMode:
				{
                    int mode = (int)(*current);
					m_pRenderer->put_BrushTextureMode(mode);
                    current += 1;
                    curindex += 1;
                    break;
				}
                case ctBrushTextureAlpha:
				{
                    int txalpha = (int)(*current);
					m_pRenderer->put_BrushTextureAlpha(txalpha);

                    current += 1;
                    curindex += 1;
                    break;
				}
                case ctSetTransform:
				{
                    m = (int*)current;
                    current += 6 * 4;
                    curindex += 6 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    m3 = (*m++) / 100000.0;
                    m4 = (*m++) / 100000.0;
                    m5 = (*m++) / 100000.0;
                    m6 = (*m++) / 100000.0;

                    m_pRenderer->SetTransform(m1, m2, m3, m4, m5, m6);
                    break;
				}
                case ctPathCommandStart:
				{
                    if (bIsPathOpened)
                    {
                        m_pRenderer->PathCommandEnd();
                        m_pRenderer->EndCommand(4);
                        m_pRenderer->BeginCommand(4);
                        m_pRenderer->PathCommandStart();
                    }
                    else
                    {
                        m_pRenderer->BeginCommand(4);
                        m_pRenderer->PathCommandStart();
                    }
                    bIsPathOpened = true;
                    break;
				}
                case ctPathCommandEnd:
				{
                    if (bIsPathOpened)
                    {
                        m_pRenderer->PathCommandEnd();
                        m_pRenderer->EndCommand(4);
                        bIsPathOpened = false;
                    }
                    break;
				}
                case ctPathCommandMoveTo:
				{
                    m = (int*)current;
                    current += 2 * 4;
                    curindex += 2 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    m_pRenderer->PathCommandMoveTo(m1, m2);
                    break;
				}
                case ctPathCommandLineTo:
				{
                    m = (int*)current;
                    current += 2 * 4;
                    curindex += 2 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    m_pRenderer->PathCommandLineTo(m1, m2);
                    break;
				}
                case ctPathCommandCurveTo:
				{
                    m = (int*)current;
                    current += 6 * 4;
                    curindex += 6 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    m3 = (*m++) / 100000.0;
                    m4 = (*m++) / 100000.0;
                    m5 = (*m++) / 100000.0;
                    m6 = (*m++) / 100000.0;

                    m_pRenderer->PathCommandCurveTo(m1, m2, m3, m4, m5, m6);
                    break;
				}
                case ctPathCommandClose:
				{
                    m_pRenderer->PathCommandClose();
                    break;
				}
                case ctDrawPath:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;

                    m_pRenderer->DrawPath(*m);
                    break;
				}
                case ctDrawImageFromFile:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;

                    _sLen = (int)(*m);
                    _sLen /= 2;
                    CString s((WCHAR*)current, _sLen);

                    imgPath = s;

					if (0 != s.Find(_T("http:")) &&
                        0 != s.Find(_T("https:")) &&
                        0 != s.Find(_T("ftp:")) &&
                        0 != s.Find(_T("file:")))
                    {
						imgPath = m_oParams.m_strImagesDirectory + _T("\\") + s;

                        int _len = imgPath.GetLength();
                        int ind = imgPath.Find(_T(".svg"));
                        if (ind != -1)
                        {
                            if (ind == (_len - 4))
                            {
                                CString sInterest = imgPath.Mid(0, ind);

								CFile oFile;
								if (oFile.OpenFile(sInterest + _T(".emf")) == S_OK)
                                    imgPath = sInterest + _T(".emf");
								else if (oFile.OpenFile(sInterest + _T(".wmf")))
                                    imgPath = sInterest + _T(".wmf");
                            }
                        }
                    }

                    if (0 == s.Find(_T("data:")))
                    {
                        // TODO:
                    }

                    current += 2 * _sLen;
                    curindex += 2 * _sLen;

                    m = (int*)current;
                    current += 4 * 4;
                    curindex += 4 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    m3 = (*m++) / 100000.0;
                    m4 = (*m++) / 100000.0;

                    try
                    {
						BSTR bsFile = imgPath.AllocSysString();
                        m_pRenderer->DrawImageFromFile(bsFile, m1, m2, m3, m4);
						SysFreeString(bsFile);
                    }
                    catch (...) 
					{
					}
                    break;
				}
                case ctFontName:
				{
					ms = (USHORT*)current;
                    current += 2;
                    curindex += 2;

                    _sLen = (int)(*ms);
                    CString s((WCHAR*)current, _sLen);

                    current += 2 * _sLen;
                    curindex += 2 * _sLen;

					BSTR bsName = s.AllocSysString();
					m_pRenderer->put_FontName(bsName);
					SysFreeString(bsName);
                    break;
				}
                case ctFontSize:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;

                    m1 = (*m++) / 100000.0;
                    // в PDF стоит ограничение на размер шрифта
					m_pRenderer->put_FontSize(min(m1, 1000));
                    break;
				}
                case ctFontStyle:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;

					m_pRenderer->put_FontStyle(*m);
                    break;
				}
                case ctDrawText:
				{
                    ms = (USHORT*)current;
                    current += 2;
                    curindex += 2;

                    _sLen = (int)(*ms);
                    CString s((WCHAR*)current, _sLen);

                    current += 2 * _sLen;
                    curindex += 2 * _sLen;

                    m = (int*)current;
                    current += 2 * 4;
                    curindex += 2 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;

					BSTR bsText = s.AllocSysString();
					m_pRenderer->CommandDrawText(bsText, m1, m2, 0, 0, 0);
					SysFreeString(bsText);
                    break;
				}
                case ctBeginCommand:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;

					if (bIsPDF)
					{
						if (bIsPathOpened)
						{
							m_pRenderer->PathCommandEnd();
							m_pRenderer->EndCommand(4);
							bIsPathOpened = false;
						}
						m_pRenderer->BeginCommand(*m);
					}
					else
					{
						m_pRenderer->BeginCommand(*m);
					}
                    break;
				}
                case ctEndCommand:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;
					
					if (bIsPDF)
					{
						if (bIsPathOpened)
						{
							m_pRenderer->PathCommandEnd();
							m_pRenderer->EndCommand(4);
							bIsPathOpened = false;
						}
						m_pRenderer->EndCommand(*m);
						m_pRenderer->PathCommandEnd();
					}
					else
					{
						m_pRenderer->EndCommand(*m);
					}
                    
                    break;
				}
                default:
                    break;
            };

            //if (nCountPages == 1 && eCommand == CommandType.ctPageEnd)
            //    break;
        }

		/*
		if (NULL != m_pPainter)
		{
			VARIANT var;
			var.vt = VT_I4;
			m_pPainter->SetAdditionalParam(L"OnCompletePage", var);
		}
		*/
	}

};