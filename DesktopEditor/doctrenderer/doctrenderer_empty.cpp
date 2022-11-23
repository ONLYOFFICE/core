/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "doctrenderer.h"
#include "docbuilder.h"

#ifdef BOOL
#undef BOOL
#endif

#include <iostream>

namespace NSDoctRenderer
{
	CString::CString() {}
	CString::CString(const CString& src) {}
	CString& CString::operator=(const CString& src) { return *this; }
	wchar_t* CString::c_str() const { return NULL; }

	CDocBuilderValue::CDocBuilderValue() {}
	CDocBuilderValue::CDocBuilderValue(const CDocBuilderValue& src) {}
	CDocBuilderValue& CDocBuilderValue::operator=(const CDocBuilderValue& src) { return *this; }
	CDocBuilderValue::~CDocBuilderValue() {}
	void* private_get_internal() { return NULL; }

	bool CDocBuilderValue::IsEmpty() { return true; }
	void CDocBuilderValue::Clear() {}

	bool CDocBuilderValue::IsNull() { return false; }
	bool CDocBuilderValue::IsUndefined() { return false; }
	bool CDocBuilderValue::IsBool() { return false; }
	bool CDocBuilderValue::IsInt() { return false; }
	bool CDocBuilderValue::IsDouble() { return false; }
	bool CDocBuilderValue::IsString() { return false; }
	bool CDocBuilderValue::IsFunction() { return false; }
	bool CDocBuilderValue::IsObject() { return false; }
	bool CDocBuilderValue::IsArray() { return false; }
	bool CDocBuilderValue::IsTypedArray() { return false; }

	unsigned int CDocBuilderValue::GetLength() { return 0; }
	bool CDocBuilderValue::ToBool() { return false; }
	int CDocBuilderValue::ToInt() { return 0; }
	double CDocBuilderValue::ToDouble() { return 0; }
	CString CDocBuilderValue::ToString() { return CString(); }

	CDocBuilderValue CDocBuilderValue::GetProperty(const wchar_t* name) { CDocBuilderValue ret; return ret; }

	CDocBuilderValue CDocBuilderValue::Get(const char* name) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Get(const wchar_t* name) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::operator[](const char* name) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::operator[](const wchar_t* name) { CDocBuilderValue ret; return ret; }

	CDocBuilderValue CDocBuilderValue::Get(const int& index) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::operator[](const int& index) { CDocBuilderValue ret; return ret; }

	void CDocBuilderValue::SetProperty(const wchar_t* name, CDocBuilderValue value) {}
	void CDocBuilderValue::Set(const wchar_t* name, CDocBuilderValue value) {}
	void CDocBuilderValue::Set(const int& index, CDocBuilderValue value) {}

	// primitives
	CDocBuilderValue::CDocBuilderValue(const bool& value) {}
	CDocBuilderValue::CDocBuilderValue(const int& value) {}
	CDocBuilderValue::CDocBuilderValue(const unsigned int& value) {}
	CDocBuilderValue::CDocBuilderValue(const double& value) {}
	CDocBuilderValue::CDocBuilderValue(const char* value) {}
	CDocBuilderValue::CDocBuilderValue(const wchar_t* value) {}

	CDocBuilderValue CDocBuilderValue::CreateUndefined() { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::CreateNull() { CDocBuilderValue ret; return ret; }

	CDocBuilderValue CDocBuilderValue::Call(const char* name) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const char* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5, CDocBuilderValue p6) { CDocBuilderValue ret; return ret; }

	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderValue::Call(const wchar_t* name, CDocBuilderValue p1, CDocBuilderValue p2, CDocBuilderValue p3, CDocBuilderValue p4, CDocBuilderValue p5, CDocBuilderValue p6) { CDocBuilderValue ret; return ret; }

	CDocBuilderContextScope::CDocBuilderContextScope() {}
	CDocBuilderContextScope::CDocBuilderContextScope(const CDocBuilderContextScope& src) {}
	CDocBuilderContextScope& CDocBuilderContextScope::operator=(const CDocBuilderContextScope& src) { return *this; }
	CDocBuilderContextScope::~CDocBuilderContextScope() {}
	void CDocBuilderContextScope::Close() {}

	CDocBuilderContext::CDocBuilderContext() {}
	CDocBuilderContext::CDocBuilderContext(const CDocBuilderContext& src) {}
	CDocBuilderContext& CDocBuilderContext::operator=(const CDocBuilderContext& src) { return *this; }
	CDocBuilderContext::~CDocBuilderContext() {}

	CDocBuilderValue CDocBuilderContext::CreateUndefined() { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderContext::CreateNull() { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderContext::CreateObject() { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderContext::CreateArray(const int& length) { CDocBuilderValue ret; return ret; }
	CDocBuilderValue CDocBuilderContext::CreateTypedArray(unsigned char* buffer, const int& length, const bool& externalize) { CDocBuilderValue ret; return ret; }

	CDocBuilderValue CDocBuilderContext::GetGlobal() { CDocBuilderValue ret; return ret; }
	CDocBuilderContextScope CDocBuilderContext::CreateScope() { CDocBuilderContextScope ret; return ret; }
	bool CDocBuilderContext::IsError() { return true; }
}

namespace NSDoctRenderer
{
	CDocBuilder::CDocBuilder() {}
	CDocBuilder::~CDocBuilder() {}

	int CDocBuilder::OpenFile(const wchar_t* path, const wchar_t* params) { return 0; }
	bool CDocBuilder::CreateFile(const int& type) { return false; }
	void CDocBuilder::SetTmpFolder(const wchar_t* folder) {}
	int CDocBuilder::SaveFile(const int& type, const wchar_t* path, const wchar_t* params) { return 0; }
	void CDocBuilder::CloseFile() {}
	bool CDocBuilder::ExecuteCommand(const wchar_t* command, CDocBuilderValue* retValue) { return false; }

	bool CDocBuilder::Run(const wchar_t* path) { return false; }

	bool CDocBuilder::RunTextW(const wchar_t* commands) { return false; }
	bool CDocBuilder::RunTextA(const char* commands) { return false; }

	void CDocBuilder::SetProperty(const char* param, const wchar_t* value) {}
	void CDocBuilder::SetPropertyW(const wchar_t* param, const wchar_t* value) {}

	void CDocBuilder::WriteData(const wchar_t* path, const wchar_t* value, const bool& append) {}
	bool CDocBuilder::IsSaveWithDoctrendererMode() { return false; }

	char* CDocBuilder::GetVersion() { return "0.0"; }

	CDocBuilderContext CDocBuilder::GetContext()
	{
		return CDocBuilderContext();
	}

	void CDocBuilder::Initialize(const wchar_t* directory) {}
	void CDocBuilder::Dispose() {}
}

namespace NSDoctRenderer
{
	CDoctrenderer::CDoctrenderer(const std::wstring& sAllFontsPath) {}

	CDoctrenderer::~CDoctrenderer() {}

	void CDoctrenderer::LoadConfig(const std::wstring& sConfigDir, const std::wstring& sAllFontsPath)
	{
	}

	bool CDoctrenderer::Execute(const std::wstring& strXml, std::wstring& strError)
	{
		return false;
	}

	std::vector<std::wstring> CDoctrenderer::GetImagesInChanges()
	{
		std::vector<std::wstring> stub;
		return stub;
	}

	void CDoctrenderer::CreateCache(const std::wstring& sAllFontsPath, const std::wstring& sCacheDir)
	{
	}
}
