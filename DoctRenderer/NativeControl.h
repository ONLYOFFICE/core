#pragma once
#include "stdafx.h"
#include "DocRenderer.h"

// string convert
static CString to_cstring(v8::Local<v8::Value> v)
{
	v8::String::Value data(v);
	const WCHAR* p = (WCHAR*)*data;
	if (NULL == p)
		return _T("");
	return CString(p);
}

static CStringA to_cstringA(v8::Local<v8::Value> v)
{
	v8::String::AsciiValue data(v);
	const char* p = (char*)*data;
	if (NULL == p)
		return "";
	return CStringA(p);
}

class CNativeControl
{
private:
	CString m_strFilePath;
	CString m_strFileId;

public:
	CAtlArray<CString>* m_pChanges;
	CString m_strFontsDirectory;
	CString m_strImagesDirectory;

public:
	CMemoryStream* m_pStream;

	CNativeControl()
	{
		m_pStream = NULL;
		m_pChanges = NULL;
	}
	~CNativeControl()
	{
		RELEASEOBJECT(m_pStream);
		m_pChanges = NULL;
	}

public:	
	void getFileData(CString& strFile, BYTE*& pData, DWORD& dwLen)
	{
		CFile oFile;
		oFile.OpenFile(strFile);
		dwLen = (DWORD)oFile.GetFileSize();
		pData = (BYTE*)malloc((size_t)dwLen);
		oFile.ReadFile(pData, dwLen);
	}

	void SetFilePath(const CString& strPath)
	{
		m_strFilePath = strPath;
	}
	CString GetFilePath()
	{
		return m_strFilePath;
	}

	void SetFileId(const CString& strId)
	{
		m_strFileId = strId;
	}
	CString GetFileId()
	{
		return m_strFileId;
	}

	void ConsoleLog(CString& strVal)
	{
		FILE* f = fopen("C:\\log.txt", "a+");
		fprintf(f, (CStringA)strVal);
		fprintf(f, "\n");
		fclose(f);
	}
};

// wrap_methods -------------
CNativeControl* unwrap_nativeobject(v8::Handle<v8::Object> obj) 
{
	v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
	return static_cast<CNativeControl*>(field->Value());
}

void _GetFilePath(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CNativeControl* pNative = unwrap_nativeobject(args.This());
	args.GetReturnValue().Set(v8::String::New((uint16_t*)pNative->GetFilePath().GetBuffer()));
}
void _SetFilePath(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	args.GetReturnValue().Set(v8::Undefined());

	if (args.Length() < 1) 
		return;
	
	CNativeControl* pNative = unwrap_nativeobject(args.This());
	pNative->SetFilePath(to_cstring(args[0]));
}

void _GetFontsDirectory(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CNativeControl* pNative = unwrap_nativeobject(args.This());
	args.GetReturnValue().Set(v8::String::New((uint16_t*)pNative->m_strFontsDirectory.GetBuffer()));
}

void _GetChangesCount(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CNativeControl* pNative = unwrap_nativeobject(args.This());

	int nCount = 0;
	if (pNative->m_pChanges != NULL)
		nCount = (int)pNative->m_pChanges->GetCount();
	args.GetReturnValue().Set(v8::Integer::New(nCount));
}
void _GetChangesFile(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CNativeControl* pNative = unwrap_nativeobject(args.This());

	if (args.Length() < 1) 
		args.GetReturnValue().Set(v8::Undefined());

	v8::Local<v8::Int32> intValue = args[0]->ToInt32();
	int nIndex = (int)intValue->Value();

	CString strFile = _T("");
	if (pNative->m_pChanges != NULL)
		strFile = pNative->m_pChanges->GetAt((size_t)nIndex);

	args.GetReturnValue().Set(v8::String::New((uint16_t*)strFile.GetBuffer()));
}

void _GetFileId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CNativeControl* pNative = unwrap_nativeobject(args.This());
	args.GetReturnValue().Set(v8::String::New((uint16_t*)pNative->GetFileId().GetBuffer()));
}
void _SetFileId(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	args.GetReturnValue().Set(v8::Undefined());

	if (args.Length() < 1) 
		return;
	
	CNativeControl* pNative = unwrap_nativeobject(args.This());
	pNative->SetFileId(to_cstring(args[0]));
}

void _GetFileArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (args.Length() < 1) 
	{
		args.GetReturnValue().Set(v8::Undefined());
		return;
	}

	CNativeControl* pNative = unwrap_nativeobject(args.This());

	BYTE* pData = NULL;
	DWORD len = 0;
	pNative->getFileData(to_cstring(args[0]), pData, len);

	v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New((void*)pData, (size_t)len);
	v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)len);

	args.GetReturnValue().Set(_array);
}

void _GetFontArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (args.Length() < 1) 
	{
		args.GetReturnValue().Set(v8::Undefined());
		return;
	}

	CNativeControl* pNative = unwrap_nativeobject(args.This());

	BYTE* pData = NULL;
	DWORD len = 0;
	CString strDir = pNative->m_strFontsDirectory;

	if (strDir != _T(""))
		strDir += _T("\\");

	strDir += to_cstring(args[0]);
	pNative->getFileData(strDir, pData, len);

	v8::Local<v8::ArrayBuffer> _buffer = v8::ArrayBuffer::New((void*)pData, (size_t)len);
	v8::Local<v8::Uint8Array> _array = v8::Uint8Array::New(_buffer, 0, (size_t)len);

	args.GetReturnValue().Set(_array);
}

void _GetFileString(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (args.Length() < 1) 
	{
		args.GetReturnValue().Set(v8::Undefined());
		return;
	}

	CNativeControl* pNative = unwrap_nativeobject(args.This());

	BYTE* pData = NULL;
	DWORD len = 0;
	pNative->getFileData(to_cstring(args[0]), pData, len);

	args.GetReturnValue().Set(v8::String::NewSymbol((char*)pData, len));
}

void _ConsoleLog(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	args.GetReturnValue().Set(v8::Undefined());
	if (args.Length() < 1) 
		return;
	
	CNativeControl* pNative = unwrap_nativeobject(args.This());
	pNative->ConsoleLog(to_cstring(args[0]));
}

v8::Handle<v8::ObjectTemplate> CreateNativeControlTemplate(v8::Isolate* isolate)
{
	v8::HandleScope handle_scope(isolate); 

	v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New();
	result->SetInternalFieldCount(1); // отводим в нем место для хранения CNativeControl

	// прописываем функции - методы объекта
	result->Set(v8::String::NewSymbol("SetFilePath"), v8::FunctionTemplate::New(_SetFilePath));    
	result->Set(v8::String::NewSymbol("GetFilePath"), v8::FunctionTemplate::New(_GetFilePath));
	result->Set(v8::String::NewSymbol("SetFileId"), v8::FunctionTemplate::New(_SetFileId));
	result->Set(v8::String::NewSymbol("GetFileId"), v8::FunctionTemplate::New(_GetFileId));
	result->Set(v8::String::NewSymbol("GetFileBinary"), v8::FunctionTemplate::New(_GetFileArrayBuffer));
	result->Set(v8::String::NewSymbol("GetFontBinary"), v8::FunctionTemplate::New(_GetFontArrayBuffer));
	result->Set(v8::String::NewSymbol("GetFontsDirectory"), v8::FunctionTemplate::New(_GetFontsDirectory));
	result->Set(v8::String::NewSymbol("GetFileString"), v8::FunctionTemplate::New(_GetFileString));

	result->Set(v8::String::NewSymbol("GetCountChanges"), v8::FunctionTemplate::New(_GetChangesCount));
	result->Set(v8::String::NewSymbol("GetChangesFile"), v8::FunctionTemplate::New(_GetChangesFile));

	result->Set(v8::String::NewSymbol("ConsoleLog"), v8::FunctionTemplate::New(_ConsoleLog));

	// возвращаем временный хэндл хитрым образом, который переносит наш хэндл в предыдущий HandleScope и не дает ему 
	// уничтожиться при уничтожении "нашего" HandleScope - handle_scope
	return handle_scope.Close(result);		
}
// --------------------------

// native object list (static)
class CNativeControlsList
{
private:
	CAtlArray<CNativeControl*> m_arrControls;

public:
	CNativeControlsList() : m_arrControls()
	{
	}
	~CNativeControlsList()
	{
		m_arrControls.RemoveAll();
	}

	CString GreateGUID()
	{
		GUID guid;
		CoCreateGuid(&guid);

		OLECHAR szGuid[GUID_STRING_LEN];
		memset(szGuid, 0, GUID_STRING_LEN * sizeof(OLECHAR));

		int nCount = ::StringFromGUID2(guid, szGuid, GUID_STRING_LEN);
		
		CString strRet(szGuid);
		return strRet;
	}

	CNativeControl* CreateControl()
	{
		CString strGuid = GreateGUID();
		CNativeControl* pControl = new CNativeControl();
		pControl->SetFileId(strGuid);

		m_arrControls.Add(pControl);
		return pControl;
	}

	size_t GetCount()
	{
		return m_arrControls.GetCount();
	}

	CNativeControl* GetControlById(const CString& strId)
	{
		size_t count = m_arrControls.GetCount();
		for (size_t i = 0; i < count; ++i)
		{
			if (strId == m_arrControls[i]->GetFileId())
			{
				return m_arrControls[i];				
			}
		}
		return NULL;
	}

	void DeleteControlById(const CString& strId)
	{
		size_t count = m_arrControls.GetCount();
		for (size_t i = 0; i < count; ++i)
		{
			if (strId == m_arrControls[i]->GetFileId())
			{
				m_arrControls.RemoveAt(i);
				return;
			}
		}
	}
};

// create work with arraytypes
class MallocArrayBufferAllocator : public v8::ArrayBuffer::Allocator 
{
public:
  	virtual void* Allocate(size_t length) 
	{ 
		void* ret = malloc(length); 
		memset(ret, 0, length);
		return ret;
	}
	virtual void* AllocateUninitialized(size_t length) 
	{ 
		return malloc(length);		
	}
	virtual void Free(void* data, size_t length) 
	{ 
		free(data); 
	}
};

static void enableTypedArrays() 
{
	v8::V8::SetArrayBufferAllocator(new MallocArrayBufferAllocator());
}

enum CommandType
{
    ctPenXML						= 0,
    ctPenColor						= 1,
    ctPenAlpha						= 2,
    ctPenSize						= 3,
    ctPenDashStyle					= 4,
    ctPenLineStartCap				= 5,
    ctPenLineEndCap				    = 6,
    ctPenLineJoin					= 7,
    ctPenDashPatern				    = 8,
    ctPenDashPatternCount			= 9,
    ctPenDashOffset				    = 10,
    ctPenAlign						= 11,
    ctPenMiterLimit				    = 12,

    // brush
    ctBrushXML						= 20,
    ctBrushType					    = 21,
    ctBrushColor1					= 22,
    ctBrushColor2					= 23,
    ctBrushAlpha1					= 24,
    ctBrushAlpha2					= 25,
    ctBrushTexturePath				= 26,
    ctBrushTextureAlpha			    = 27,
    ctBrushTextureMode				= 28,
    ctBrushRectable				    = 29,
    ctBrushRectableEnabled 		    = 30,
    ctBrushGradient                 = 31,

    // font
    ctFontXML						= 40,
    ctFontName						= 41,
    ctFontSize						= 42,
    ctFontStyle					    = 43,
    ctFontPath						= 44,
    ctFontGID						= 45,
    ctFontCharSpace				    = 46,

    // shadow
    ctShadowXML					    = 50,
    ctShadowVisible				    = 51,
    ctShadowDistanceX				= 52,
    ctShadowDistanceY				= 53,
    ctShadowBlurSize				= 54,
    ctShadowColor					= 55,
    ctShadowAlpha					= 56,

    // edge
    ctEdgeXML						= 70,
    ctEdgeVisible					= 71,
    ctEdgeDistance					= 72,
    ctEdgeColor					    = 73,
    ctEdgeAlpha					    = 74,

    // text
    ctDrawText						= 80,
    ctDrawTextEx					= 81,

    // pathcommands
    ctPathCommandMoveTo			    = 91,
    ctPathCommandLineTo			    = 92,
    ctPathCommandLinesTo			= 93,
    ctPathCommandCurveTo			= 94,
    ctPathCommandCurvesTo			= 95,
    ctPathCommandArcTo		        = 96,
    ctPathCommandClose				= 97,
    ctPathCommandEnd				= 98,
    ctDrawPath						= 99,
    ctPathCommandStart				= 100,
    ctPathCommandGetCurrentPoint	= 101,
    ctPathCommandText				= 102,
    ctPathCommandTextEx			    = 103,

    // image
    ctDrawImage					    = 110,
    ctDrawImageFromFile			    = 111,

    ctSetParams					    = 120,

    ctBeginCommand					= 121,
    ctEndCommand					= 122,

    ctSetTransform					= 130,
    ctResetTransform				= 131,

    ctClipMode						= 140,

    ctCommandLong1					= 150,
    ctCommandDouble1				= 151,
    ctCommandString1				= 152,
    ctCommandLong2					= 153,
    ctCommandDouble2				= 154,
    ctCommandString2				= 155,

    ctPageWidth                     = 200,
    ctPageHeight                    = 201,

    ctPageStart                     = 202,
    ctPageEnd                       = 203,

    ctError						    = 255
};