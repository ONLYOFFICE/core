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

#include "stdafx.h"

#include "docbuilder.net.h"
#include "../docbuilder.h"

#ifdef _WIN64
#pragma comment(lib, "../../../build/lib/win_64/doctrenderer.lib")
#else
#pragma comment(lib, "../../../build/lib/win_32/doctrenderer.lib")
#endif

namespace docbuilder_net
{
	static wchar_t* StringToStdString(String^ param)
	{
		return (wchar_t*)System::Runtime::InteropServices::Marshal::StringToHGlobalUni(param).ToPointer();
	}

	ref class CDocBuilder_Private
	{
	public:
		NSDoctRenderer::CDocBuilder* m_pBuilder;

		CDocBuilder_Private()
		{
			m_pBuilder = new NSDoctRenderer::CDocBuilder();
		}
		~CDocBuilder_Private()
		{
			delete m_pBuilder;
		}
	};


	CDocBuilder::CDocBuilder()
	{
		m_pInternal = gcnew CDocBuilder_Private();
	}
	CDocBuilder::~CDocBuilder()
	{
		delete m_pInternal;
	}

	bool CDocBuilder::OpenFile(String^ path, String^ params)
	{		
		return m_pInternal->m_pBuilder->OpenFile(StringToStdString(path), StringToStdString(params));
	}
	bool CDocBuilder::CreateFile(int type)
	{
		return m_pInternal->m_pBuilder->CreateFile(type);
	}
	void CDocBuilder::SetTmpFolder(String^ folder)
	{
		m_pInternal->m_pBuilder->SetTmpFolder(StringToStdString(folder));
	}
	bool CDocBuilder::SaveFile(int type, String^ path)
	{
		return m_pInternal->m_pBuilder->SaveFile(type, StringToStdString(path));
	}
	void CDocBuilder::CloseFile()
	{
		m_pInternal->m_pBuilder->CloseFile();
	}
	bool CDocBuilder::ExecuteCommand(String^ command)
	{
		return m_pInternal->m_pBuilder->ExecuteCommand(StringToStdString(command));
	}

	bool CDocBuilder::Run(String^ path)
	{
		return m_pInternal->m_pBuilder->Run(StringToStdString(path));
	}

	bool CDocBuilder::RunText(String^ text)
	{
		return m_pInternal->m_pBuilder->RunTextW(StringToStdString(text));
	}

	void CDocBuilder::SetProperty(String^ key, String^ value)
	{		
		m_pInternal->m_pBuilder->SetPropertyW(StringToStdString(key), StringToStdString(value));
	}

	void CDocBuilder::Initialize()
	{
		NSDoctRenderer::CDocBuilder::Initialize();
	}
	void CDocBuilder::Destroy()
	{
		NSDoctRenderer::CDocBuilder::Dispose();
	}

}

