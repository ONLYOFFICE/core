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
// docbuildercom.h : Declaration of the CDocbuilder
#pragma once

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#include "stdafx.h"

using namespace ATL;
#ifdef CreateFile
#undef CreateFile
#endif

#include "../../../../DesktopEditor/doctrenderer/docbuilder.h"
#include "../../../../Common/OfficeFileFormats.h"

#define RELEASEINTERFACE(pinterface)\
{\
    if (pinterface!=NULL)\
    {\
        pinterface->Release();\
        pinterface=NULL;\
    }\
}

#define VALUE(x) *(((C_DOCBUILDER_VALUE*)(x))->m_pValue)
#define VALUE_PRIVATE(x) *(((C_DOCBUILDER_VALUE*)(x))->GetPrivate())

I_DOCBUILDER_VALUE_HEADER
__interface I_DOCBUILDER_VALUE : IDispatch
{
	[id(1)] HRESULT CreateInstance([in, optional] VARIANT value);

	[id(100)] HRESULT IsEmpty([out, retval] VARIANT_BOOL* result);
	[id(101)] HRESULT Clear();

	[id(102)] HRESULT IsNull([out, retval] VARIANT_BOOL* result);
	[id(103)] HRESULT IsUndefined([out, retval] VARIANT_BOOL* result);

	[id(104)] HRESULT IsBool ([out, retval] VARIANT_BOOL* result);
	[id(105)] HRESULT IsInt([out, retval] VARIANT_BOOL* result);
	[id(106)] HRESULT IsDouble([out, retval] VARIANT_BOOL* result);
	[id(107)] HRESULT IsString([out, retval] VARIANT_BOOL* result);
	[id(108)] HRESULT IsFunction([out, retval] VARIANT_BOOL* result);
	[id(109)] HRESULT IsObject([out, retval] VARIANT_BOOL* result);

	[id(110)] HRESULT IsArray([out, retval] VARIANT_BOOL* result);
	[id(111)] HRESULT IsTypedArray([out, retval] VARIANT_BOOL* result);

	[id(112)] HRESULT GetLength([out, retval] long* result);

	[id(113)] HRESULT ToBool([out, retval] VARIANT_BOOL* result);
	[id(114)] HRESULT ToInt([out, retval] long* result);
	[id(115)] HRESULT ToDouble([out, retval] double* result);
	[id(116)] HRESULT ToString([out, retval] BSTR* result);

	[id(117)] HRESULT GetProperty([in] BSTR name, [out, retval] I_DOCBUILDER_VALUE** result);
	[id(118)] HRESULT Get([in] long index, [out, retval] I_DOCBUILDER_VALUE** result);

	[id(119)] HRESULT SetProperty([in] BSTR name, [in] I_DOCBUILDER_VALUE* value);
	[id(120)] HRESULT Set([in] long index, [in] I_DOCBUILDER_VALUE* value);

	[id(121)] HRESULT Call([in] BSTR name, [in, optional] VARIANT val1, [in, optional] VARIANT val2, [in, optional] VARIANT val3, [in, optional] VARIANT val4, [in, optional] VARIANT val5, [in, optional] VARIANT val6, [out, retval] I_DOCBUILDER_VALUE** result);
};

I_DOCBUILDER_CONTEXT_SCOPE_HEADER
__interface I_DOCBUILDER_CONTEXT_SCOPE : IDispatch
{
	[id(100)] HRESULT Close();
};

I_DOCBUILDER_CONTEXT_HEADER
__interface I_DOCBUILDER_CONTEXT : IDispatch
{
	[id(100)] HRESULT CreateUndefined([out, retval] I_DOCBUILDER_VALUE** result);
	[id(101)] HRESULT CreateNull([out, retval] I_DOCBUILDER_VALUE** result);
	[id(102)] HRESULT CreateObject([out, retval] I_DOCBUILDER_VALUE** result);
	[id(103)] HRESULT CreateArray([in] long length, [out, retval] I_DOCBUILDER_VALUE** result);

	[id(104)] HRESULT CreateTypedArray([in] VARIANT buffer, [in] long length, [out, retval] I_DOCBUILDER_VALUE** result);

	[id(105)] HRESULT GetGlobal([out, retval] I_DOCBUILDER_VALUE** result);
	[id(106)] HRESULT CreateScope([out, retval] I_DOCBUILDER_CONTEXT_SCOPE** result);
	[id(107)] HRESULT IsError([out, retval] VARIANT_BOOL* result);
};

I_DOCBUILDER_HEADER
__interface I_DOCBUILDER : IDispatch
{
	[id(1)] HRESULT CreateInstance(void);

	[id(100)] HRESULT OpenFile([in] BSTR path, [in] BSTR params, [out, retval] VARIANT_BOOL* result);
	[id(101)] HRESULT CreateFile([in] BSTR type, [out, retval] VARIANT_BOOL* result);
	[id(102)] HRESULT SetTmpFolder([in] BSTR folder);
	[id(103)] HRESULT SaveFile([in] BSTR type, [in] BSTR path, [out, retval] VARIANT_BOOL* result);
	[id(104)] HRESULT CloseFile();
	[id(105)] HRESULT ExecuteCommand([in] BSTR command, [out, retval] VARIANT_BOOL* result);
	[id(106)] HRESULT Run([in] BSTR path, [out, retval] VARIANT_BOOL* result);
	[id(107)] HRESULT RunText([in] BSTR commands, [out, retval] VARIANT_BOOL* result);
	[id(108)] HRESULT SetProperty([in] BSTR key, [in] BSTR value);
	[id(109)] HRESULT Execute([in] BSTR command, [out, retval] I_DOCBUILDER_VALUE** result);

	[id(110)] HRESULT WriteData([in] BSTR path, [in] BSTR value, [in] VARIANT_BOOL append);
	[id(111)] HRESULT IsSaveWithDoctrendererMode([out, retval] VARIANT_BOOL* result);

	[id(112)] HRESULT GetContext([out, retval] I_DOCBUILDER_CONTEXT** result);

	[id(201)] HRESULT Initialize();
	[id(202)] HRESULT Dispose();
};

static CStringW GetCurrentDllDirPath()
{
	CStringW thisPath = L"";
	WCHAR path[MAX_PATH];
	HMODULE hm;
	if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPWSTR)&GetCurrentDllDirPath, &hm))
	{
		GetModuleFileNameW(hm, path, sizeof(path));
		PathRemoveFileSpecW(path);
		thisPath = CStringW(path);
		if (!thisPath.IsEmpty() &&
			thisPath.GetAt(thisPath.GetLength() - 1) != '\\')
			thisPath += L"\\";
	}	
	return thisPath;
}
STDAPI MyVarI8FromCy(CY cyIn, __int64* pi64Out)
{
	LONG64 lVal = cyIn.int64 / 10000;
	LONG64 lDif = cyIn.int64 % 10000;

	if (lDif + (lVal & 1) > 5000) // IEEE rounding
	{
		if (cyIn.Hi >= 0)
			lVal++;
		else
			lVal--;
	}
	*pi64Out = lVal;
	return NOERROR;
}

C_DOCBUILDER_VALUE_HEADER
class ATL_NO_VTABLE C_DOCBUILDER_VALUE : public I_DOCBUILDER_VALUE
{
protected:
	NSDoctRenderer::CDocBuilderValue* m_pValue;

public:
	C_DOCBUILDER_VALUE()
	{
		m_pValue = NULL;
		VARIANT val;
		val.vt = VT_ERROR;
		this->CreateInstance(val);
	}
	~C_DOCBUILDER_VALUE()
	{
		if (m_pValue)
		{
			delete m_pValue;
			m_pValue = NULL;
		}
			
	}

	STDMETHOD(CreateInstance)(VARIANT value)
	{
		if (m_pValue)
		{
			delete m_pValue;
			m_pValue = NULL;
		}

		if (value.vt == VT_ERROR || value.vt == VT_EMPTY || value.vt == VT_NULL)
			m_pValue = new NSDoctRenderer::CDocBuilderValue();

		if(value.vt == VT_I4)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.lVal);

		if (value.vt == VT_I2)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.iVal);

		if (value.vt == VT_R4)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.fltVal);

		if (value.vt == VT_R8)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.dblVal);

		if (value.vt == VT_I1)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.cVal);

		if (value.vt == VT_UI1)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.bVal);

		if (value.vt == VT_UI2)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.uiVal);

		if (value.vt == VT_UI4)
			m_pValue = new NSDoctRenderer::CDocBuilderValue((long)value.ulVal);

		if (value.vt == VT_UI8)
			m_pValue = new NSDoctRenderer::CDocBuilderValue((long)value.ullVal);

		if (value.vt == VT_INT)
			m_pValue = new NSDoctRenderer::CDocBuilderValue((long)value.intVal);

		if (value.vt == VT_UINT)
			m_pValue = new NSDoctRenderer::CDocBuilderValue((long)value.uintVal);

		if (value.vt == VT_CY)
		{
			__int64 t = 0;
			MyVarI8FromCy(value.cyVal, &t);
			m_pValue = new NSDoctRenderer::CDocBuilderValue((long)t);
		}

		// OneScript uses this thing
		if (value.vt == VT_DECIMAL)
		{
			VARIANT t;
			VariantInit(&t);
			VariantChangeType(&t, &value, NULL, VT_I4);
			m_pValue = new NSDoctRenderer::CDocBuilderValue(t.lVal);
			VariantClear(&t);
		}
		
		if (value.vt == VT_BOOL)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.boolVal);

		if (value.vt == VT_BSTR)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.bstrVal);

		if (value.vt == VT_DISPATCH)
		{	
			I_DOCBUILDER_VALUE* val = NULL;
			value.pdispVal->QueryInterface(__uuidof(I_DOCBUILDER_VALUE), (void**)&val);
			m_pValue = new NSDoctRenderer::CDocBuilderValue(VALUE(val));
			RELEASEINTERFACE(val);
		}
		else if (value.vt == VT_UNKNOWN)
		{
			I_DOCBUILDER_VALUE* val = NULL;
			value.punkVal->QueryInterface(__uuidof(I_DOCBUILDER_VALUE), (void**)&val);
			m_pValue = new NSDoctRenderer::CDocBuilderValue(VALUE(val));
			RELEASEINTERFACE(val);
		}
		return S_OK;
	}

	STDMETHOD(IsEmpty)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsEmpty() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(Clear)()
	{
		m_pValue->Clear();
		return S_OK;
	}

	STDMETHOD(IsNull)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsNull() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(IsUndefined)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsUndefined() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}

	STDMETHOD(IsBool)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsBool() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(IsInt)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsInt() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(IsDouble)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsDouble() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(IsString)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsString() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(IsFunction)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsFunction() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(IsObject)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsObject() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}

	STDMETHOD(IsArray)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsArray() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(IsTypedArray)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->IsTypedArray() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}

	STDMETHOD(GetLength)(long* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->GetLength();

		return S_OK;
	}

	STDMETHOD(ToBool)(VARIANT_BOOL* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->ToBool() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(ToInt)(long* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->ToInt();

		return S_OK;
	}
	STDMETHOD(ToDouble)(double* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
			*result = m_pValue->ToDouble();

		return S_OK;
	}
	STDMETHOD(ToString)(BSTR* result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		NSDoctRenderer::CString val = m_pValue->ToString();
		if (result)
		{
			*result = NULL;
			if (val.c_str())
			{
				CString sValue(val.c_str());
				*result = sValue.AllocSysString();
			}
		}
		return S_OK;
	}

	STDMETHOD(GetProperty)(BSTR name, I_DOCBUILDER_VALUE** result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
		{
			CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)result);
			VALUE(*result) = m_pValue->GetProperty(name);
		}
		return S_OK;
	}
	STDMETHOD(Get)(long index, I_DOCBUILDER_VALUE** result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
		{
			CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)result);
			VALUE(*result) = m_pValue->Get(index);
		}
		return S_OK;
	}

	STDMETHOD(SetProperty)(BSTR name, I_DOCBUILDER_VALUE* value)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (value)
			m_pValue->SetProperty(name, VALUE(value));

		return S_OK;
	}
	STDMETHOD(Set)(long index, I_DOCBUILDER_VALUE* value)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (value)
			m_pValue->Set(index, VALUE(value));

		return S_OK;
	}

	STDMETHOD(Call)(BSTR name, VARIANT val1, VARIANT val2, VARIANT val3, VARIANT val4, VARIANT val5, VARIANT val6, I_DOCBUILDER_VALUE** result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		I_DOCBUILDER_VALUE* p1 = NULL;
		I_DOCBUILDER_VALUE* p2 = NULL;
		I_DOCBUILDER_VALUE* p3 = NULL;
		I_DOCBUILDER_VALUE* p4 = NULL;
		I_DOCBUILDER_VALUE* p5 = NULL;
		I_DOCBUILDER_VALUE* p6 = NULL;

		if (val1.vt != VT_ERROR && val1.vt != VT_EMPTY)
		{
			CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)&p1);
			p1->CreateInstance(val1);
		}
		if (val2.vt != VT_ERROR && val2.vt != VT_EMPTY)
		{
			CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)&p2);
			p2->CreateInstance(val2);
		}
		if (val3.vt != VT_ERROR && val3.vt != VT_EMPTY)
		{
			CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)&p3);
			p3->CreateInstance(val3);
		}
		if (val4.vt != VT_ERROR && val4.vt != VT_EMPTY)
		{
			CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)&p4);
			p4->CreateInstance(val4);
		}
		if (val5.vt != VT_ERROR && val5.vt != VT_EMPTY)
		{
			CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)&p5);
			p5->CreateInstance(val5);
		}
		if (val6.vt != VT_ERROR && val6.vt != VT_EMPTY)
		{
			CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)&p6);
			p6->CreateInstance(val6);
		}

		NSDoctRenderer::CDocBuilderValue oResult;
		if (p6)
			oResult = m_pValue->Call(name, VALUE(p1), VALUE(p2), VALUE(p3), VALUE(p4), VALUE(p5), VALUE(p6));
		else if (p5)
			oResult = m_pValue->Call(name, VALUE(p1), VALUE(p2), VALUE(p3), VALUE(p4), VALUE(p5));
		else if (p4)
			oResult = m_pValue->Call(name, VALUE(p1), VALUE(p2), VALUE(p3), VALUE(p4));
		else if (p3)
			oResult = m_pValue->Call(name, VALUE(p1), VALUE(p2), VALUE(p3));
		else if (p2)
			oResult = m_pValue->Call(name, VALUE(p1), VALUE(p2));
		else if (p1)
			oResult = m_pValue->Call(name, VALUE(p1));
		else
			oResult = m_pValue->Call(name);

		if (result)
		{
			CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)result);
			VALUE(*result) = oResult;
		}

		RELEASEINTERFACE(p1);
		RELEASEINTERFACE(p2);
		RELEASEINTERFACE(p3);
		RELEASEINTERFACE(p4);
		RELEASEINTERFACE(p5);
		RELEASEINTERFACE(p6);

		return S_OK;
	}

	NSDoctRenderer::CDocBuilderValue* GetPrivate()
	{
		return m_pValue;
	}
};

C_DOCBUILDER_CONTEXT_SCOPE_HEADER
class ATL_NO_VTABLE C_DOCBUILDER_CONTEXT_SCOPE : public I_DOCBUILDER_CONTEXT_SCOPE
{
protected:
	NSDoctRenderer::CDocBuilderContextScope* m_pBuilderContextScope;

public:
	C_DOCBUILDER_CONTEXT_SCOPE()
	{
		m_pBuilderContextScope = NULL;
		m_pBuilderContextScope = new NSDoctRenderer::CDocBuilderContextScope();
	}
	~C_DOCBUILDER_CONTEXT_SCOPE()
	{
		if (m_pBuilderContextScope)
		{
			delete m_pBuilderContextScope;
			m_pBuilderContextScope = NULL;
		}
	}

	STDMETHOD(Close)()
	{
		if (NULL == m_pBuilderContextScope)
			return S_FALSE;

		m_pBuilderContextScope->Close();
		return S_OK;
	}

	NSDoctRenderer::CDocBuilderContextScope* GetPrivate()
	{
		return m_pBuilderContextScope;
	}
};

C_DOCBUILDER_CONTEXT_HEADER
class ATL_NO_VTABLE C_DOCBUILDER_CONTEXT : public I_DOCBUILDER_CONTEXT
{
protected:
	NSDoctRenderer::CDocBuilderContext* m_pBuilderContext;

public:
	C_DOCBUILDER_CONTEXT()
	{
		m_pBuilderContext = NULL;
		m_pBuilderContext = new NSDoctRenderer::CDocBuilderContext();
	}
	~C_DOCBUILDER_CONTEXT()
	{
		if (m_pBuilderContext)
		{
			delete m_pBuilderContext;
			m_pBuilderContext = NULL;
		}
	}

	STDMETHOD(CreateUndefined)(I_DOCBUILDER_VALUE** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)result);
		VALUE_PRIVATE(*result) = m_pBuilderContext->CreateUndefined();

		return S_OK;
	}
	STDMETHOD(CreateNull)(I_DOCBUILDER_VALUE** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)result);
		VALUE_PRIVATE(*result) = m_pBuilderContext->CreateNull();

		return S_OK;

	}
	STDMETHOD(CreateObject)(I_DOCBUILDER_VALUE** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)result);
		VALUE_PRIVATE(*result) = m_pBuilderContext->CreateObject();

		return S_OK;
	}
	STDMETHOD(CreateArray)(long length, I_DOCBUILDER_VALUE** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)result);
		VALUE_PRIVATE(*result) = m_pBuilderContext->CreateArray(length);

		return S_OK;
	}

	STDMETHOD(CreateTypedArray)(VARIANT buffer, long length, I_DOCBUILDER_VALUE** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		if(buffer.vt != (VT_ARRAY | VT_UI1))
			return S_FALSE;

		unsigned char* p_array = m_pBuilderContext->AllocMemoryTypedArray(length);
		memcpy(p_array, (unsigned char*)(buffer.parray->pvData), length);

		CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)result);
		VALUE_PRIVATE(*result) = m_pBuilderContext->CreateTypedArray(p_array, length, false);

		return S_OK;
	}

	STDMETHOD(GetGlobal)(I_DOCBUILDER_VALUE** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)result);
		VALUE_PRIVATE(*result) = m_pBuilderContext->GetGlobal();

		return S_OK;
	}
	STDMETHOD(CreateScope)(I_DOCBUILDER_CONTEXT_SCOPE** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(C_DOCBUILDER_CONTEXT_SCOPE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_CONTEXT_SCOPE), (void**)result);
		*(((C_DOCBUILDER_CONTEXT_SCOPE*)(*result))->GetPrivate()) = m_pBuilderContext->CreateScope();

		return S_OK;
	}
	STDMETHOD(IsError)(VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		if(result)
			*result = m_pBuilderContext->IsError() ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}

	NSDoctRenderer::CDocBuilderContext* GetPrivate()
	{
		return m_pBuilderContext;
	}
};

C_DOCBUILDER_HEADER
class ATL_NO_VTABLE C_DOCBUILDER : public I_DOCBUILDER
{
protected:
	NSDoctRenderer::CDocBuilder* m_pBuilder;

public:
	C_DOCBUILDER()
	{
		m_pBuilder = NULL;
		this->CreateInstance();
	}
	~C_DOCBUILDER()
	{
		if (m_pBuilder)
		{
			delete m_pBuilder;
			m_pBuilder = NULL;
		}
	}

	STDMETHOD(CreateInstance)()
	{
		if (m_pBuilder)
		{
			delete m_pBuilder;
			m_pBuilder = NULL;
		}
		m_pBuilder = new NSDoctRenderer::CDocBuilder();

		return S_OK;
	}
	STDMETHOD(OpenFile)(BSTR path, BSTR params, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		bool bRet = m_pBuilder->OpenFile(path, params);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(CreateFile)(BSTR type, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		int nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		CStringW sType = (CString)type;
		if (L"pptx" == sType)
			nType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
		else if (L"xlsx" == sType)
			nType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;

		bool bRet = m_pBuilder->CreateFile(nType);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(SetTmpFolder)(BSTR folder)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		m_pBuilder->SetTmpFolder(folder);
		return S_OK;
	}
	STDMETHOD(SaveFile)(BSTR type, BSTR path, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		int nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		CStringW sType = (CString)type;
		if (L"docx" == sType)
			nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
		else if (L"doc" == sType)
			nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
		else if (L"odt" == sType)
			nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
		else if (L"rtf" == sType)
			nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
		else if (L"txt" == sType)
			nType = AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
		else if (L"pptx" == sType)
			nType = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
		else if (L"odp" == sType)
			nType = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
		else if (L"xlsx" == sType)
			nType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
		else if (L"xls" == sType)
			nType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
		else if (L"ods" == sType)
			nType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
		else if (L"csv" == sType)
			nType = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
		else if (L"pdf" == sType)
			nType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
		else if (L"image" == sType)
			nType = AVS_OFFICESTUDIO_FILE_IMAGE;
		else if (L"jpg" == sType)
			nType = AVS_OFFICESTUDIO_FILE_IMAGE;
		else if (L"png" == sType)
			nType = AVS_OFFICESTUDIO_FILE_IMAGE;

		bool bRet = m_pBuilder->SaveFile(nType, path);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(CloseFile)()
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		m_pBuilder->CloseFile();
		return S_OK;
	}
	STDMETHOD(ExecuteCommand)(BSTR command, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		bool bRet = m_pBuilder->ExecuteCommand(command);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(Run)(BSTR path, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		bool bRet = m_pBuilder->Run(path);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(RunText)(BSTR commands, VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		bool bRet = m_pBuilder->RunTextW(commands);
		*result = bRet ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(SetProperty)(BSTR key, BSTR value)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		m_pBuilder->SetPropertyW(key, value);
		return S_OK;
	}

	STDMETHOD(Execute)(BSTR command, I_DOCBUILDER_VALUE** result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		if (result)
		{
			CoCreateInstance(__uuidof(C_DOCBUILDER_VALUE), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_VALUE), (void**)result);
			m_pBuilder->ExecuteCommand(command, ((C_DOCBUILDER_VALUE*)(*result))->GetPrivate());
		}
		return S_OK;
	}
	STDMETHOD(WriteData)(BSTR path, BSTR value, VARIANT_BOOL append)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		m_pBuilder->WriteData(path, value, append);
		return S_OK;
	}

	STDMETHOD(IsSaveWithDoctrendererMode)(VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		*result = m_pBuilder->IsSaveWithDoctrendererMode() ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}

	STDMETHOD(GetContext)(I_DOCBUILDER_CONTEXT** result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		CoCreateInstance(__uuidof(C_DOCBUILDER_CONTEXT), NULL, CLSCTX_ALL, __uuidof(I_DOCBUILDER_CONTEXT), (void**)result);
		*(((C_DOCBUILDER_CONTEXT*)(*result))->GetPrivate()) = m_pBuilder->GetContext();

		return S_OK;
	}

	STDMETHOD(Initialize)()
	{
		CStringW sCurrentDir = GetCurrentDllDirPath();
		BSTR bsCurrentDir = sCurrentDir.AllocSysString();
		NSDoctRenderer::CDocBuilder::Initialize(bsCurrentDir);
		m_pBuilder->SetProperty("--work-directory", (wchar_t*)bsCurrentDir);

		SysFreeString(bsCurrentDir);
		return S_OK;
	}
	STDMETHOD(Dispose)()
	{
		NSDoctRenderer::CDocBuilder::Dispose();
		return S_OK;
	}
};
