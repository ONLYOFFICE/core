#pragma once
#include "stdafx.h"
#include "../Common/DocxFormat/Source/SystemUtility/File.h"

#define GUID_STRING_LEN 40

//#pragma comment(lib, "../../../../../../../v8/build/Debug/lib/icui18n.lib")
//#pragma comment(lib, "../../../../../../../v8/build/Debug/lib/icuuc.lib")
//#pragma comment(lib, "../../../../../../../v8/build/Debug/lib/v8_base.ia32.lib")
//#pragma comment(lib, "../../../../../../../v8/build/Debug/lib/v8_nosnapshot.ia32.lib")
//#pragma comment(lib, "../../../../../../../v8/build/Debug/lib/v8_snapshot.lib")

#ifdef _DEBUG
#pragma comment(lib, "../../../../../../v8_trunk/build/Debug/lib/icui18n.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Debug/lib/icuuc.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Debug/lib/v8_libbase.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Debug/lib/v8_base.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Debug/lib/v8_nosnapshot.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Debug/lib/v8_snapshot.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Debug/lib/v8_libplatform.lib")
#else
#pragma comment(lib, "../../../../../../v8_trunk/build/Release/lib/icui18n.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Release/lib/icuuc.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Release/lib/v8_libbase.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Release/lib/v8_base.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Release/lib/v8_nosnapshot.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Release/lib/v8_snapshot.lib")
#pragma comment(lib, "../../../../../../v8_trunk/build/Release/lib/v8_libplatform.lib")
#endif

#include "../../../../../../v8_trunk/include/v8.h"
#include "../../../../../../v8_trunk/include/libplatform/libplatform.h"

class CMemoryStream
{
private:
	// сам метафайл
	BYTE* m_pBuffer;
	BYTE* m_pBufferMem;

	size_t m_lSize;

public:
	CMemoryStream()
	{
		m_pBuffer		= NULL;
		m_pBufferMem	= NULL;

		m_lSize			= 0;
	}
	~CMemoryStream()
	{
		RELEASEARRAYOBJECTS(m_pBuffer);
	}

	__forceinline BYTE* GetData()
	{
		return m_pBuffer;
	}
	__forceinline int GetSize()
	{
		return (int)(m_pBufferMem - m_pBuffer);
	}

	__forceinline void Clear()
	{
		m_lSize		= 0;

		m_pBuffer		= NULL;
		m_pBufferMem	= NULL;
	}

	__forceinline void ClearNoAttack()
	{
		m_pBufferMem = m_pBuffer;
	}

	__forceinline void Copy(const CMemoryStream* pData, const size_t& nPos, const size_t& nLen)
    {
		CheckBufferSize(nLen);
		memcpy(m_pBufferMem, pData->m_pBuffer + nPos, nLen);
		m_pBufferMem += nLen;
    };

	__forceinline void CheckBufferSize(size_t lPlus)
	{
		if (NULL != m_pBuffer)
		{
			size_t nNewSize = (m_pBufferMem - m_pBuffer) + lPlus;
			if (nNewSize >= m_lSize)
			{
				while (nNewSize >= m_lSize)
				{
					m_lSize *= 2;
				}
				
				BYTE* pNew = new BYTE[m_lSize];
				memcpy(pNew, m_pBuffer, m_pBufferMem - m_pBuffer);

				m_pBufferMem	= pNew + (m_pBufferMem - m_pBuffer);

				RELEASEARRAYOBJECTS(m_pBuffer);
				m_pBuffer		= pNew;
			}
		}
		else
		{
			m_lSize			= 1000;
			m_pBuffer		= new BYTE[m_lSize];
			m_pBufferMem	= m_pBuffer;

			CheckBufferSize(lPlus);
		}
	}

	__forceinline void WriteBYTE(const BYTE& lValue)
	{
		CheckBufferSize(sizeof(BYTE));
		*m_pBufferMem = lValue;
		m_pBufferMem += sizeof(BYTE);
	}
	__forceinline void WriteLONG(const LONG& lValue)
	{
		CheckBufferSize(sizeof(LONG));
		*((LONG*)(m_pBufferMem)) = lValue; 
		m_pBufferMem += sizeof(LONG);
	}
	__forceinline void WriteDouble(const double& dValue)
	{
		CheckBufferSize(sizeof(double));
		*((double*)(m_pBufferMem)) = dValue; 
		m_pBufferMem += sizeof(double);
	}
	__forceinline void WriteStringA(const char* pData, int nLen)
	{
		CheckBufferSize(nLen + sizeof(USHORT));
		*((USHORT*)(m_pBufferMem)) = (USHORT)nLen;
		m_pBufferMem += sizeof(USHORT);

		memcpy(m_pBufferMem, pData, nLen);
		m_pBufferMem += nLen;
	}
	__forceinline void WriteStringA2(const char* pData, int nLen)
	{
		CheckBufferSize(nLen + sizeof(LONG));
		*((LONG*)(m_pBufferMem)) = (LONG)nLen;
		m_pBufferMem += sizeof(LONG);

		memcpy(m_pBufferMem, pData, nLen);
		m_pBufferMem += nLen;
	}

	__forceinline void WriteString(const wchar_t* pData, int nLen)
	{
		CheckBufferSize(nLen + sizeof(USHORT));
		*((USHORT*)(m_pBufferMem)) = (USHORT)nLen;
		m_pBufferMem += sizeof(USHORT);

		int nLen2 = nLen << 1;
		memcpy(m_pBufferMem, pData, nLen2);
		m_pBufferMem += nLen2;
	}
	__forceinline void WriteString2(const wchar_t* pData, int nLen)
	{
		int nLen2 = nLen << 1;

		CheckBufferSize(nLen2 + sizeof(LONG));
		*((LONG*)(m_pBufferMem)) = (LONG)nLen2;
		m_pBufferMem += sizeof(LONG);

		memcpy(m_pBufferMem, pData, nLen2);
		m_pBufferMem += nLen2;
	}
};

// wrap_methods -------------
CMemoryStream* unwrap_memorystream(v8::Handle<v8::Object> obj) 
{
	v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
	return static_cast<CMemoryStream*>(field->Value());
}

void _ms_write_byte(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CMemoryStream* pNative = unwrap_memorystream(args.This());

	BYTE arg = (BYTE)args[0]->Int32Value();
	pNative->WriteBYTE(arg);

	args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_write_bool(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CMemoryStream* pNative = unwrap_memorystream(args.This());

	BYTE arg = (BYTE)args[0]->BooleanValue();
	pNative->WriteBYTE(arg ? 1 : 0);

	args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_write_long(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CMemoryStream* pNative = unwrap_memorystream(args.This());

	LONG arg = (LONG)args[0]->Int32Value();
	pNative->WriteLONG(arg);

	args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_write_double(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CMemoryStream* pNative = unwrap_memorystream(args.This());

	double arg = (double)args[0]->NumberValue();
	pNative->WriteLONG((LONG)(arg * 100000));

	args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_writestring1(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CMemoryStream* pNative = unwrap_memorystream(args.This());
	v8::String::Value data(args[0]);	
	pNative->WriteString((wchar_t*)*data, data.length());
	args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void _ms_writestring2(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CMemoryStream* pNative = unwrap_memorystream(args.This());
	v8::String::Value data(args[0]);	
	pNative->WriteString2((wchar_t*)*data, data.length());
	args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_copy(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CMemoryStream* pNative = unwrap_memorystream(args.This());

	CMemoryStream* pNative2 = unwrap_memorystream(args[0]->ToObject());
	size_t pos = (size_t)args[1]->Uint32Value();
	size_t len = (size_t)args[2]->Uint32Value();

	pNative->Copy(pNative2, pos, len);

	args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_clearnoattack(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	CMemoryStream* pNative = unwrap_memorystream(args.This());
	pNative->ClearNoAttack();
	args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void _ms_pos(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	CMemoryStream* pNative = unwrap_memorystream(info.Holder());
	info.GetReturnValue().Set(v8::Integer::New(v8::Isolate::GetCurrent(), pNative->GetSize()));
}

v8::Handle<v8::ObjectTemplate> CreateMemoryStreamTemplate(v8::Isolate* isolate)
{
	//v8::HandleScope handle_scope(isolate); 

	v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New();
	result->SetInternalFieldCount(1); // отводим в нем место для хранения CNativeControl

	v8::Isolate* current = v8::Isolate::GetCurrent();

	// property
	result->SetAccessor(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "pos"), _ms_pos); // получить код ошибки

	// прописываем функции - методы объекта
	result->Set(v8::String::NewFromUtf8(current, "Copy"),			v8::FunctionTemplate::New(current, _ms_copy));
	result->Set(v8::String::NewFromUtf8(current, "ClearNoAttack"),	v8::FunctionTemplate::New(current, _ms_clearnoattack));
	result->Set(v8::String::NewFromUtf8(current, "WriteByte"),		v8::FunctionTemplate::New(current, _ms_write_byte));
	result->Set(v8::String::NewFromUtf8(current, "WriteBool"),		v8::FunctionTemplate::New(current, _ms_write_bool));
	result->Set(v8::String::NewFromUtf8(current, "WriteLong"),		v8::FunctionTemplate::New(current, _ms_write_long));
	result->Set(v8::String::NewFromUtf8(current, "WriteDouble"),	v8::FunctionTemplate::New(current, _ms_write_double));
	result->Set(v8::String::NewFromUtf8(current, "WriteString"),	v8::FunctionTemplate::New(current, _ms_writestring1));
	result->Set(v8::String::NewFromUtf8(current, "WriteString2"),	v8::FunctionTemplate::New(current, _ms_writestring2));

	// возвращаем временный хэндл хитрым образом, который переносит наш хэндл в предыдущий HandleScope и не дает ему 
	// уничтожиться при уничтожении "нашего" HandleScope - handle_scope

	//return handle_scope.Close(result);		
	return result;
}

