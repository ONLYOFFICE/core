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
#include "stdafx.h"
#include "resource.h"       // main symbols

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;
#ifdef CreateFile
#undef CreateFile
#endif

#ifdef _WIN64
#pragma comment(lib, "../../../build/lib/win_64/doctrenderer.lib")
#else
#pragma comment(lib, "../../../build/lib/win_32/doctrenderer.lib")
#endif

#include "../../../DesktopEditor/doctrenderer/docbuilder.h"
#include "../../../Common/OfficeFileFormats.h"

// IONLYOFFICEDocBuilderValue
[object, uuid("2637FDFA-8473-4CB8-B90B-C23CB949D009"), dual, pointer_default(unique)]
__interface IONLYOFFICEDocBuilderValue : IDispatch
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

	[id(117)] HRESULT GetProperty([in] BSTR name, [out, retval] IONLYOFFICEDocBuilderValue** result);
	[id(118)] HRESULT Get([in] long index, [out, retval] IONLYOFFICEDocBuilderValue** result);

	[id(119)] HRESULT SetProperty([in] BSTR name, [in] IONLYOFFICEDocBuilderValue* value);
	[id(120)] HRESULT Set([in] long index, [in] IONLYOFFICEDocBuilderValue* value);

	[id(121)] HRESULT Call([in] BSTR name, [in, optional] VARIANT val1 = CComVariant(), [in, optional] VARIANT val2 = CComVariant(), [in, optional] VARIANT val3 = CComVariant(), [in, optional] VARIANT val4 = CComVariant(), [in, optional] VARIANT val5 = CComVariant(), [in, defaultvalue(CComVariant())] VARIANT val6 = CComVariant(), [out, retval] IONLYOFFICEDocBuilderValue** result = NULL);
};

// IONLYOFFICEDocBuilderContextScope
[object, uuid("656ae95c-ae91-4dc0-88bf-0b770fc2d552"), dual, pointer_default(unique)]
__interface IONLYOFFICEDocBuilderContextScope : IDispatch
{
	[id(100)] HRESULT Close();
};

// IONLYOFFICEDocBuilderContext
[object, uuid("0416975a-65c3-4015-85e4-55d9dafec5fc"), dual, pointer_default(unique)]
__interface IONLYOFFICEDocBuilderContext : IDispatch
{
	[id(100)] HRESULT CreateUndefined([out, retval] IONLYOFFICEDocBuilderValue** result);
	[id(101)] HRESULT CreateNull([out, retval] IONLYOFFICEDocBuilderValue** result);
	[id(102)] HRESULT CreateObject([out, retval] IONLYOFFICEDocBuilderValue** result);
	[id(103)] HRESULT CreateArray([in] long length, [out, retval] IONLYOFFICEDocBuilderValue** result);

	// CreateTypedArray
	// AllocMemoryTypedArray
	// FreeMemoryTypedArray

	[id(104)] HRESULT GetGlobal([out, retval] IONLYOFFICEDocBuilderValue** result);
	[id(105)] HRESULT CreateScope([out, retval] IONLYOFFICEDocBuilderContextScope** result);
	[id(106)] HRESULT IsError([out, retval] VARIANT_BOOL* result);
};

// IONLYOFFICEDocBuilder
[object, uuid("0C07B7E7-86A4-42E1-8E42-2FA961992E0F"), dual, pointer_default(unique)]
__interface IONLYOFFICEDocBuilder : IDispatch
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

	// try do 2 retval (out)
	[id(109)] HRESULT Execute([in] BSTR command, [out, retval] IONLYOFFICEDocBuilderValue** result);

	[id(110)] HRESULT WriteData([in] BSTR path, [in] BSTR value, [in] VARIANT_BOOL append);
	[id(111)] HRESULT IsSaveWithDoctrendererMode([out, retval] VARIANT_BOOL* result);
	// char* -> BSTR? [id(112)] HRESULT GetVersion([out, retval] BSTR* result);

	[id(112)] HRESULT GetContext([out, retval] IONLYOFFICEDocBuilderContext** result);

	[id(201)] HRESULT Initialize([in] BSTR directory);
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

// CONLYOFFICEDocBuilderValue
[coclass, uuid("85C41585-25D7-40F1-9CC6-FA17052650F4"), threading(apartment), vi_progid("ONLYOFFICE.BuilderValue"), progid("ONLYOFFICE.BuilderValue.1"), version(1.0)]
class ATL_NO_VTABLE CONLYOFFICEDocBuilderValue : public IONLYOFFICEDocBuilderValue
{
protected:
	NSDoctRenderer::CDocBuilderValue* m_pValue;

public:
	CONLYOFFICEDocBuilderValue()
	{
		m_pValue = NULL;
		VARIANT val;
		val.vt = VT_ERROR;
		this->CreateInstance(val);
	}
	~CONLYOFFICEDocBuilderValue()
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

		if (value.vt == VT_ERROR)
			m_pValue = new NSDoctRenderer::CDocBuilderValue();

		if(value.vt == VT_I4)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.lVal);

		if (value.vt == VT_I2)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.iVal);

		if (value.vt == VT_R4)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.fltVal);

		if (value.vt == VT_R8)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.dblVal);

		if (value.vt == VT_BOOL)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.boolVal);

		if (value.vt == VT_BSTR)
			m_pValue = new NSDoctRenderer::CDocBuilderValue(value.bstrVal);

		if (value.vt == VT_DISPATCH)
		{	
			IONLYOFFICEDocBuilderValue* val = NULL;
			value.pdispVal->QueryInterface(__uuidof(IONLYOFFICEDocBuilderValue), (void**)&val);
			m_pValue = ((CONLYOFFICEDocBuilderValue*)(val))->GetPrivate();
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

	STDMETHOD(GetProperty)(BSTR name, IONLYOFFICEDocBuilderValue** result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
		{
			CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)result);
			*((CONLYOFFICEDocBuilderValue*)(*result))->m_pValue = m_pValue->GetProperty(name);
		}
		return S_OK;
	}
	STDMETHOD(Get)(long index, IONLYOFFICEDocBuilderValue** result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (result)
		{
			CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)result);
			*((CONLYOFFICEDocBuilderValue*)(*result))->m_pValue = m_pValue->Get(index);
		}
		return S_OK;
	}

	STDMETHOD(SetProperty)(BSTR name, IONLYOFFICEDocBuilderValue* value)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (value)
			m_pValue->SetProperty(name, ((CONLYOFFICEDocBuilderValue*)(value))->m_pValue);
		return S_OK;
	}
	STDMETHOD(Set)(long index, IONLYOFFICEDocBuilderValue* value)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		if (value)
			m_pValue->Set(index, ((CONLYOFFICEDocBuilderValue*)(value))->m_pValue);
		return S_OK;
	}

	STDMETHOD(Call)(BSTR name, VARIANT val1, VARIANT val2, VARIANT val3, VARIANT val4, VARIANT val5, VARIANT val6, IONLYOFFICEDocBuilderValue** result)
	{
		if (NULL == m_pValue)
			return S_FALSE;

		int val_c = 0;

		IONLYOFFICEDocBuilderValue* p1 = NULL;
		IONLYOFFICEDocBuilderValue* p2 = NULL;
		IONLYOFFICEDocBuilderValue* p3 = NULL;
		IONLYOFFICEDocBuilderValue* p4 = NULL;
		IONLYOFFICEDocBuilderValue* p5 = NULL;
		IONLYOFFICEDocBuilderValue* p6 = NULL;

		if (val1.vt != VT_ERROR && val1.vt != VT_EMPTY)
		{
			val_c++;
			CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)&p1);
			p1->CreateInstance(val1);
		}
		if (val2.vt != VT_ERROR && val2.vt != VT_EMPTY)
		{
			val_c++;
			CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)&p2);
			p2->CreateInstance(val2);
		}
		if (val3.vt != VT_ERROR && val3.vt != VT_EMPTY)
		{
			val_c++;
			CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)&p3);
			p3->CreateInstance(val3);
		}
		if (val4.vt != VT_ERROR && val4.vt != VT_EMPTY)
		{
			val_c++;
			CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)&p4);
			p4->CreateInstance(val4);
		}
		if (val5.vt != VT_ERROR && val5.vt != VT_EMPTY)
		{
			val_c++;
			CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)&p5);
			p5->CreateInstance(val5);
		}
		if (val6.vt != VT_ERROR && val6.vt != VT_EMPTY)
		{
			val_c++;
			CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)&p6);
			p6->CreateInstance(val6);
		}

		if (result)
		{
			CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)result);

			switch (val_c)
			{
			case 0:
				*((CONLYOFFICEDocBuilderValue*)(*result))->m_pValue = m_pValue->Call(name);
				break;
			case 1:
				*((CONLYOFFICEDocBuilderValue*)(*result))->m_pValue = m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue);
				break;
			case 2:
				*((CONLYOFFICEDocBuilderValue*)(*result))->m_pValue = m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p2))->m_pValue);
				break;
			case 3:
				*((CONLYOFFICEDocBuilderValue*)(*result))->m_pValue = m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p2))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p3))->m_pValue);
				break;
			case 4:
				*((CONLYOFFICEDocBuilderValue*)(*result))->m_pValue = m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p2))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p3))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p4))->m_pValue);
				break;
			case 5:
				*((CONLYOFFICEDocBuilderValue*)(*result))->m_pValue = m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p2))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p3))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p4))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p5))->m_pValue);
				break;
			case 6:
				*((CONLYOFFICEDocBuilderValue*)(*result))->m_pValue = m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p2))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p3))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p4))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p5))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p6))->m_pValue);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (val_c)
			{
			case 0:
				m_pValue->Call(name);
				break;
			case 1:
				m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue);
				break;
			case 2:
				m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p2))->m_pValue);
				break;
			case 3:
				m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p2))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p3))->m_pValue);
				break;
			case 4:
				m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p2))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p3))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p4))->m_pValue);
				break;
			case 5:
				m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p2))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p3))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p4))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p5))->m_pValue);
				break;
			case 6:
				m_pValue->Call(name,
					((CONLYOFFICEDocBuilderValue*)(p1))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p2))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p3))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p4))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p5))->m_pValue,
					((CONLYOFFICEDocBuilderValue*)(p6))->m_pValue);
				break;
			default:
				break;
			}
		}
		return S_OK;
	}

	NSDoctRenderer::CDocBuilderValue* GetPrivate()
	{
		return m_pValue;
	}
};

// CONLYOFFICEDocBuilderContextScope
[coclass, uuid("c54e2b15-ff5b-45a2-aa15-89a02605c30c"), threading(apartment), vi_progid("ONLYOFFICE.BuilderContextScope"), progid("ONLYOFFICE.BuilderContextScope.1"), version(1.0)]
class ATL_NO_VTABLE CONLYOFFICEDocBuilderContextScope : public IONLYOFFICEDocBuilderContextScope
{
protected:
	NSDoctRenderer::CDocBuilderContextScope* m_pBuilderContextScope;

public:
	CONLYOFFICEDocBuilderContextScope()
	{
		m_pBuilderContextScope = NULL;
		m_pBuilderContextScope = new NSDoctRenderer::CDocBuilderContextScope();
	}
	~CONLYOFFICEDocBuilderContextScope()
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

// CONLYOFFICEDocBuilderContext
[coclass, uuid("299250bb-16c2-4ab4-8a49-a0c350d66bb5"), threading(apartment), vi_progid("ONLYOFFICE.BuilderContext"), progid("ONLYOFFICE.BuilderContext.1"), version(1.0)]
class ATL_NO_VTABLE CONLYOFFICEDocBuilderContext : public IONLYOFFICEDocBuilderContext
{
protected:
	NSDoctRenderer::CDocBuilderContext* m_pBuilderContext;

public:
	CONLYOFFICEDocBuilderContext()
	{
		m_pBuilderContext = NULL;
		m_pBuilderContext = new NSDoctRenderer::CDocBuilderContext();
	}
	~CONLYOFFICEDocBuilderContext()
	{
		if (m_pBuilderContext)
		{
			delete m_pBuilderContext;
			m_pBuilderContext = NULL;
		}
	}

	STDMETHOD(CreateUndefined)(IONLYOFFICEDocBuilderValue** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)result);
		*(((CONLYOFFICEDocBuilderValue*)(*result))->GetPrivate()) = m_pBuilderContext->CreateUndefined();

		return S_OK;
	}
	STDMETHOD(CreateNull)(IONLYOFFICEDocBuilderValue** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)result);
		*(((CONLYOFFICEDocBuilderValue*)(*result))->GetPrivate()) = m_pBuilderContext->CreateNull();

		return S_OK;

	}
	STDMETHOD(CreateObject)(IONLYOFFICEDocBuilderValue** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)result);
		*(((CONLYOFFICEDocBuilderValue*)(*result))->GetPrivate()) = m_pBuilderContext->CreateObject();

		return S_OK;
	}
	STDMETHOD(CreateArray)(long length, IONLYOFFICEDocBuilderValue** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)result);
		*(((CONLYOFFICEDocBuilderValue*)(*result))->GetPrivate()) = m_pBuilderContext->CreateArray(length);

		return S_OK;
	}

	// CreateTypedArray
	// AllocMemoryTypedArray
	// FreeMemoryTypedArray

	STDMETHOD(GetGlobal)(IONLYOFFICEDocBuilderValue** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)result);
		*(((CONLYOFFICEDocBuilderValue*)(*result))->GetPrivate()) = m_pBuilderContext->GetGlobal();

		return S_OK;
	}
	STDMETHOD(CreateScope)(IONLYOFFICEDocBuilderContextScope** result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderContextScope), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderContextScope), (void**)result);
		*(((CONLYOFFICEDocBuilderContextScope*)(*result))->GetPrivate()) = m_pBuilderContext->CreateScope();

		return S_OK;
	}
	STDMETHOD(IsError)(VARIANT_BOOL* result)
	{
		if (NULL == m_pBuilderContext)
			return S_FALSE;

		*result = m_pBuilderContext->IsError() ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}

	NSDoctRenderer::CDocBuilderContext* GetPrivate()
	{
		return m_pBuilderContext;
	}
};

// CONLYOFFICEDocBuilder
[coclass, uuid("9BF69F3C-1506-41B9-B8EE-2839948C02E9"), threading(apartment), vi_progid("ONLYOFFICE.Builder"), progid("ONLYOFFICE.Builder.1"), version(1.0)]
class ATL_NO_VTABLE CONLYOFFICEDocBuilder : public IONLYOFFICEDocBuilder
{
protected:
	NSDoctRenderer::CDocBuilder* m_pBuilder;

public:
	CONLYOFFICEDocBuilder()
	{
		m_pBuilder = NULL;
		this->CreateInstance();
	}
	~CONLYOFFICEDocBuilder()
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

		/* do it manually
		CStringW sCurrentDir = GetCurrentDllDirPath();
		BSTR bsCurrentDir = sCurrentDir.AllocSysString();
		m_pBuilder->SetProperty("--work-directory", (wchar_t*)bsCurrentDir);
		SysFreeString(bsCurrentDir);
		*/

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

	STDMETHOD(Execute)(BSTR command, IONLYOFFICEDocBuilderValue** result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		if (result)
		{
			CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderValue), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderValue), (void**)result);
			m_pBuilder->ExecuteCommand(command, ((CONLYOFFICEDocBuilderValue*)(*result))->GetPrivate());
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

	STDMETHOD(GetContext)(IONLYOFFICEDocBuilderContext** result)
	{
		if (NULL == m_pBuilder)
			return S_FALSE;

		CoCreateInstance(__uuidof(CONLYOFFICEDocBuilderContext), NULL, CLSCTX_ALL, __uuidof(IONLYOFFICEDocBuilderContext), (void**)result);
		*(((CONLYOFFICEDocBuilderContext*)(*result))->GetPrivate()) = m_pBuilder->GetContext();

		return S_OK;
	}

	STDMETHOD(Initialize)(BSTR directory)
	{
		NSDoctRenderer::CDocBuilder::Initialize(directory);
		return S_OK;
	}
	STDMETHOD(Dispose)()
	{
		NSDoctRenderer::CDocBuilder::Dispose();
		return S_OK;
	}
};
