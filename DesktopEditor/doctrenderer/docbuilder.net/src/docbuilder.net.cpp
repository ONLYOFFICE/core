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
// This is the main DLL file.

#include "docbuilder.net.h"

#include <vcclr.h>
#include <cstring>

namespace docbuilder_net
{
	namespace FileTypes
	{
		public enum class Document : int
		{
			MASK = 0x0040,
			DOCX = MASK + 0x0001,
			DOC = MASK + 0x0002,
			ODT = MASK + 0x0003,
			RTF = MASK + 0x0004,
			TXT = MASK + 0x0005,
			DOTX = MASK + 0x000c,
			OTT = MASK + 0x000f
		};

		public enum class Spreadsheet : int
		{
			MASK = 0x0100,
			XLSX = MASK + 0x0001,
			XLS = MASK + 0x0002,
			ODS = MASK + 0x0003,
			CSV = MASK + 0x0004,
			XLTX = MASK + 0x0006,
			OTS = MASK + 0x0009,
		};

		public enum class Presentation : int
		{
			MASK = 0x0080,
			PPTX = MASK + 0x0001,
			PPT = MASK + 0x0002,
			ODP = MASK + 0x0003,
			PPSX = MASK + 0x0004,
			POTX = MASK + 0x0007,
			OTP = MASK + 0x000a
		};

		public enum class Graphics : int
		{
			PDF_MASK = 0x0200,
			PDF = PDF_MASK + 0x0001,
			PDFA = PDF_MASK + 0x0009,

			IMAGE_MASK = 0x0400,
			JPG = IMAGE_MASK + 0x0001,
			PNG = IMAGE_MASK + 0x0005,
			BMP = IMAGE_MASK + 0x0008
		};
	}
}

namespace docbuilder_net
{
	static const wchar_t* StringToStdString(String^ param)
	{
		pin_ptr<const wchar_t> ptr = PtrToStringChars(param);
		return ptr;
	}

	CDocBuilderValue::CDocBuilderValue()
	{
		m_internal = new NSDoctRenderer::CDocBuilderValue();
	}
	CDocBuilderValue::CDocBuilderValue(const CDocBuilderValue% other)
	{
		m_internal = new NSDoctRenderer::CDocBuilderValue(*other.m_internal);
	}
	CDocBuilderValue::~CDocBuilderValue()
	{
		this->!CDocBuilderValue();
	}
	CDocBuilderValue::!CDocBuilderValue()
	{
		// uncoment to check finalizer calls
		// System::Console::WriteLine("!CDocBuilderValue()");
		if (m_internal)
		{
			delete m_internal;
			m_internal = nullptr;
		}
	}

	bool CDocBuilderValue::IsEmpty()
	{
		return m_internal->IsEmpty();
	}
	void CDocBuilderValue::Clear()
	{
		m_internal->Clear();
	}

	bool CDocBuilderValue::IsNull()
	{
		return m_internal->IsNull();
	}
	bool CDocBuilderValue::IsUndefined()
	{
		return m_internal->IsUndefined();
	}
	bool CDocBuilderValue::IsInt()
	{
		return m_internal->IsInt();
	}
	bool CDocBuilderValue::IsDouble()
	{
		return m_internal->IsDouble();
	}
	bool CDocBuilderValue::IsString()
	{
		return m_internal->IsString();
	}
	bool CDocBuilderValue::IsFunction()
	{
		return m_internal->IsFunction();
	}
	bool CDocBuilderValue::IsObject()
	{
		return m_internal->IsObject();
	}

	bool CDocBuilderValue::IsArray()
	{
		return m_internal->IsArray();
	}
	bool CDocBuilderValue::IsTypedArray()
	{
		return m_internal->IsTypedArray();
	}

	unsigned int CDocBuilderValue::GetLength()
	{
		return m_internal->GetLength();
	}

	bool CDocBuilderValue::ToBool()
	{
		return m_internal->ToBool();
	}
	int CDocBuilderValue::ToInt()
	{
		return m_internal->ToInt();
	}
	double CDocBuilderValue::ToDouble()
	{
		return m_internal->ToDouble();
	}
	String^ CDocBuilderValue::ToString()
	{
		wchar_t* pStr = m_internal->ToString().c_str();
		String^ str = gcnew String(pStr);
		return str;
	}

	CDocBuilderValue^ CDocBuilderValue::GetProperty(String^ name)
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = m_internal->GetProperty(StringToStdString(name));
		return value;
	}
	CDocBuilderValue^ CDocBuilderValue::Get(String^ name)
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = m_internal->Get(StringToStdString(name));
		return value;
	}
	CDocBuilderValue^ CDocBuilderValue::Get(int index)
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = m_internal->Get(index);
		return value;
	}
	
	void CDocBuilderValue::SetProperty(String^ name, CDocBuilderValue^ value)
	{
		m_internal->SetProperty(StringToStdString(name), *value->m_internal);
	}
	void CDocBuilderValue::Set(String^ name, CDocBuilderValue^ value)
	{
		m_internal->Set(StringToStdString(name), *value->m_internal);
	}
	void CDocBuilderValue::Set(int index, CDocBuilderValue^ value)
	{
		m_internal->Set(index, *value->m_internal);
	}
	
	CDocBuilderValue::CDocBuilderValue(bool value)
	{
		m_internal = new NSDoctRenderer::CDocBuilderValue(value);
	}
	CDocBuilderValue::CDocBuilderValue(int value)
	{
		m_internal = new NSDoctRenderer::CDocBuilderValue(value);
	}
	CDocBuilderValue::CDocBuilderValue(unsigned int value)
	{
		m_internal = new NSDoctRenderer::CDocBuilderValue(value);
	}
	CDocBuilderValue::CDocBuilderValue(double value)
	{
		m_internal = new NSDoctRenderer::CDocBuilderValue(value);
	}
	CDocBuilderValue::CDocBuilderValue(String^ value)
	{
		m_internal = new NSDoctRenderer::CDocBuilderValue(StringToStdString(value));
	}

	CDocBuilderValue::operator CDocBuilderValue ^ (bool value)
	{
		return gcnew CDocBuilderValue(value);
	}
	CDocBuilderValue::operator CDocBuilderValue ^ (int value)
	{
		return gcnew CDocBuilderValue(value);
	}
	CDocBuilderValue::operator CDocBuilderValue ^ (unsigned int value)
	{
		return gcnew CDocBuilderValue(value);
	}
	CDocBuilderValue::operator CDocBuilderValue ^ (double value)
	{
		return gcnew CDocBuilderValue(value);
	}
	CDocBuilderValue::operator CDocBuilderValue ^ (String^ value)
	{
		return gcnew CDocBuilderValue(value);
	}

	CDocBuilderValue^ CDocBuilderValue::CreateUndefined()
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = NSDoctRenderer::CDocBuilderValue::CreateUndefined();
		return value;
	}
	CDocBuilderValue^ CDocBuilderValue::CreateNull()
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = NSDoctRenderer::CDocBuilderValue::CreateNull();
		return value;
	}

	CDocBuilderValue^ CDocBuilderValue::Call(String^ name)
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = m_internal->Call(StringToStdString(name));
		return value;
	}
	CDocBuilderValue^ CDocBuilderValue::Call(String^ name, CDocBuilderValue^ p1)
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = m_internal->Call(StringToStdString(name), *p1->m_internal);
		return value;
	}
	CDocBuilderValue^ CDocBuilderValue::Call(String^ name, CDocBuilderValue^ p1, CDocBuilderValue^ p2)
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = m_internal->Call(StringToStdString(name), *p1->m_internal, *p2->m_internal);
		return value;
	}
	CDocBuilderValue^ CDocBuilderValue::Call(String^ name, CDocBuilderValue^ p1, CDocBuilderValue^ p2, CDocBuilderValue^ p3)
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = m_internal->Call(StringToStdString(name), *p1->m_internal, *p2->m_internal, *p3->m_internal);
		return value;
	}
	CDocBuilderValue^ CDocBuilderValue::Call(String^ name, CDocBuilderValue^ p1, CDocBuilderValue^ p2, CDocBuilderValue^ p3, CDocBuilderValue^ p4)
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = m_internal->Call(StringToStdString(name), *p1->m_internal, *p2->m_internal, *p3->m_internal, *p4->m_internal);
		return value;
	}
	CDocBuilderValue^ CDocBuilderValue::Call(String^ name, CDocBuilderValue^ p1, CDocBuilderValue^ p2, CDocBuilderValue^ p3, CDocBuilderValue^ p4, CDocBuilderValue^ p5)
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = m_internal->Call(StringToStdString(name), *p1->m_internal, *p2->m_internal, *p3->m_internal, *p4->m_internal, *p5->m_internal);
		return value;
	}
	CDocBuilderValue^ CDocBuilderValue::Call(String^ name, CDocBuilderValue^ p1, CDocBuilderValue^ p2, CDocBuilderValue^ p3, CDocBuilderValue^ p4, CDocBuilderValue^ p5, CDocBuilderValue^ p6)
	{
		CDocBuilderValue^ value = gcnew CDocBuilderValue();
		*(value->m_internal) = m_internal->Call(StringToStdString(name), *p1->m_internal, *p2->m_internal, *p3->m_internal, *p4->m_internal, *p5->m_internal, *p6->m_internal);
		return value;
	}


	CDocBuilder::CDocBuilder()
	{
		m_internal = new NSDoctRenderer::CDocBuilder();
	}
	CDocBuilder::~CDocBuilder()
	{
		this->!CDocBuilder();
	}
	CDocBuilder::!CDocBuilder()
	{
		if (m_internal)
		{
			delete m_internal;
			m_internal = nullptr;
		}
	}

	int CDocBuilder::OpenFile(String^ path, String^ params)
	{		
		return m_internal->OpenFile(StringToStdString(path), StringToStdString(params));
	}
	bool CDocBuilder::CreateFile(int type)
	{
		return m_internal->CreateFile(type);
	}
	void CDocBuilder::SetTmpFolder(String^ folder)
	{
		m_internal->SetTmpFolder(StringToStdString(folder));
	}
	bool CDocBuilder::SaveFile(int type, String^ path)
	{
		return m_internal->SaveFile(type, StringToStdString(path));
	}
	bool CDocBuilder::SaveFile(int type, String^ path, String^ params)
	{
		return m_internal->SaveFile(type, StringToStdString(path), StringToStdString(params));
	}
	void CDocBuilder::CloseFile()
	{
		m_internal->CloseFile();
	}
	bool CDocBuilder::ExecuteCommand(String^ command)
	{
		return m_internal->ExecuteCommand(StringToStdString(command));
	}
	bool CDocBuilder::ExecuteCommand(String^ command, [Runtime::InteropServices::Out] CDocBuilderValue^% retValue)
	{
		bool ret = m_internal->ExecuteCommand(StringToStdString(command), retValue->m_internal);
		return ret;
	}

	bool CDocBuilder::Run(String^ path)
	{
		return m_internal->Run(StringToStdString(path));
	}
	bool CDocBuilder::RunText(String^ text)
	{
		return m_internal->RunTextW(StringToStdString(text));
	}

	void CDocBuilder::SetProperty(String^ param, String^ value)
	{
		m_internal->SetPropertyW(StringToStdString(param), StringToStdString(value));
	}

	void CDocBuilder::WriteData(String^ path, String^ value, bool append)
	{
		m_internal->WriteData(StringToStdString(path), StringToStdString(value), append);
	}
	bool CDocBuilder::IsSaveWithDoctrendererMode()
	{
		return m_internal->IsSaveWithDoctrendererMode();
	}
	String^ CDocBuilder::GetVersion()
	{
		return gcnew String(m_internal->GetVersion());
	}

	CDocBuilderContext^ CDocBuilder::GetContext()
	{
		CDocBuilderContext^ context = gcnew CDocBuilderContext();
		*(context->m_internal) = m_internal->GetContext();
		return context;
	}

	void CDocBuilder::Initialize(String^ directory)
	{
		NSDoctRenderer::CDocBuilder::Initialize(StringToStdString(directory));
	}
	void CDocBuilder::Initialize()
	{
		NSDoctRenderer::CDocBuilder::Initialize();
	}
	void CDocBuilder::Destroy()
	{
		NSDoctRenderer::CDocBuilder::Dispose();
	}

	
	CDocBuilderContextScope::CDocBuilderContextScope()
	{
		m_internal = new NSDoctRenderer::CDocBuilderContextScope();
	}
	CDocBuilderContextScope::CDocBuilderContextScope(const CDocBuilderContextScope% other)
	{
		m_internal = new NSDoctRenderer::CDocBuilderContextScope(*other.m_internal);
	}
	CDocBuilderContextScope::~CDocBuilderContextScope()
	{
		this->!CDocBuilderContextScope();
	}
	CDocBuilderContextScope::!CDocBuilderContextScope()
	{
		if (m_internal)
		{
			delete m_internal;
			m_internal = nullptr;
		}
	}

	void CDocBuilderContextScope::Close()
	{
		m_internal->Close();
	}


	CDocBuilderContext::CDocBuilderContext()
	{
		m_internal = new NSDoctRenderer::CDocBuilderContext();
	}
	CDocBuilderContext::CDocBuilderContext(const CDocBuilderContext% other)
	{
		m_internal = new NSDoctRenderer::CDocBuilderContext(*other.m_internal);
	}
	CDocBuilderContext::~CDocBuilderContext()
	{
		this->!CDocBuilderContext();
	}
	CDocBuilderContext::!CDocBuilderContext()
	{
		if (m_internal)
		{
			delete m_internal;
			m_internal = nullptr;
		}
	}

	CDocBuilderValue^ CDocBuilderContext::CreateUndefined()
	{
		CDocBuilderValue^ val = gcnew CDocBuilderValue();
		*(val->m_internal) = m_internal->CreateUndefined();
		return val;
	}
	CDocBuilderValue^ CDocBuilderContext::CreateNull()
	{
		CDocBuilderValue^ val = gcnew CDocBuilderValue();
		*(val->m_internal) = m_internal->CreateNull();
		return val;
	}
	CDocBuilderValue^ CDocBuilderContext::CreateObject()
	{
		CDocBuilderValue^ val = gcnew CDocBuilderValue();
		*(val->m_internal) = m_internal->CreateObject();
		return val;
	}
	CDocBuilderValue^ CDocBuilderContext::CreateArray(int length)
	{
		CDocBuilderValue^ val = gcnew CDocBuilderValue();
		*(val->m_internal) = m_internal->CreateArray(length);
		return val;
	}
	CDocBuilderValue^ CDocBuilderContext::CreateTypedArray(array<Byte>^ buffer)
	{
		pin_ptr<unsigned char> cppBuff = &buffer[0];
		unsigned char* pArrayBuffer = m_internal->AllocMemoryTypedArray(buffer->Length);
		memcpy(pArrayBuffer, cppBuff, buffer->Length);

		CDocBuilderValue^ val = gcnew CDocBuilderValue();
		*(val->m_internal) = m_internal->CreateTypedArray(pArrayBuffer, buffer->Length, false);
		return val;
	}

	CDocBuilderValue^ CDocBuilderContext::GetGlobal()
	{
		CDocBuilderValue^ val = gcnew CDocBuilderValue();
		*(val->m_internal) = m_internal->GetGlobal();
		return val;
	}
	CDocBuilderContextScope^ CDocBuilderContext::CreateScope()
	{
		CDocBuilderContextScope^ scope = gcnew CDocBuilderContextScope();
		*(scope->m_internal) = m_internal->CreateScope();
		return scope;
	}

	bool CDocBuilderContext::IsError()
	{
		return m_internal->IsError();
	}
}

