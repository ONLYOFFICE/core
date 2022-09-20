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

namespace NSDoctRenderer
{
	CDocBuilder::CDocBuilder()
	{
		m_pInternal = new CDocBuilder_Private();
		m_pInternal->m_pParent = this;

		std::wstring sWorkDirectory = NSDoctRenderer::CDocBuilder_Private::m_sExternalDirectory;
		if (!sWorkDirectory.empty())
			this->SetProperty("--work-directory", sWorkDirectory.c_str());
	}
	CDocBuilder::~CDocBuilder()
	{
		RELEASEOBJECT(m_pInternal);
	}

	int CDocBuilder::OpenFile(const wchar_t* path, const wchar_t* params)
	{
		m_pInternal->m_nFileType = -1;
		if (!NSDirectory::Exists(m_pInternal->m_sTmpFolder))
			NSDirectory::CreateDirectory(m_pInternal->m_sTmpFolder);

		return m_pInternal->OpenFile(path, params);
	}
	int CDocBuilder::SaveFile(const int& type, const wchar_t* path, const wchar_t* params)
	{
		return m_pInternal->SaveFile(type, path, params);
	}
	bool CDocBuilder::ExecuteCommand(const wchar_t* command, CDocBuilderValue* retValue)
	{
		return m_pInternal->ExecuteCommand(command, retValue);
	}

	CDocBuilderContext CDocBuilder::GetContext()
	{
		return m_pInternal->GetContext();
	}
}
