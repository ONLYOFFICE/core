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
#include "WmfClip.h"

namespace MetaFile
{
	CWmfClip::CWmfClip()
	{
	}
	CWmfClip::~CWmfClip()
	{
		Clear();
	}
	void CWmfClip::operator=(CWmfClip& oClip)
	{
		Clear();
		for (unsigned int ulIndex = 0; ulIndex < oClip.m_vCommands.size(); ulIndex++)
		{
			CWmfClipCommandBase* pCommand = oClip.m_vCommands.at(ulIndex);
			CWmfClipCommandBase* pNewCommand = NULL;
			switch (pCommand->GetType())
			{
				case WMF_CLIPCOMMAND_INTERSECT:
				{
					CWmfClipCommandIntersect* pIntersect = (CWmfClipCommandIntersect*)pCommand;
					pNewCommand = new CWmfClipCommandIntersect(pIntersect->m_oClip);
					break;
				}
				case WMF_CLIPCOMMAND_EXCLUDE:
				{
					CWmfClipCommandExclude* pExclude = (CWmfClipCommandExclude*)pCommand;
					pNewCommand = new CWmfClipCommandExclude(pExclude->m_oClip, pExclude->m_oBB);
					break;
				}
			}

			if (pNewCommand)
				m_vCommands.push_back(pNewCommand);
		}
	}
	void CWmfClip::Reset()
	{
		Clear();
	}
	bool CWmfClip::Intersect(const TRectD& oClip)
	{
		CWmfClipCommandBase* pCommand = new CWmfClipCommandIntersect(oClip);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	bool CWmfClip::Exclude(const TRectD& oClip, const TRectD& oBB)
	{
		CWmfClipCommandBase* pCommand = new CWmfClipCommandExclude(oClip, oBB);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	void CWmfClip::Clear()
	{
		for (unsigned int ulIndex = 0; ulIndex < m_vCommands.size(); ulIndex++)
		{
			CWmfClipCommandBase* pCommand = m_vCommands.at(ulIndex);
			delete pCommand;
		}
		m_vCommands.clear();
	}
	void CWmfClip::ClipOnRenderer(IOutputDevice* pOutput)
	{
		if (pOutput)
		{
			pOutput->ResetClip();
			for (unsigned int ulIndex = 0; ulIndex < m_vCommands.size(); ulIndex++)
			{
				CWmfClipCommandBase* pCommand = m_vCommands.at(ulIndex);

				switch (pCommand->GetType())
				{
					case WMF_CLIPCOMMAND_INTERSECT:
					{
						CWmfClipCommandIntersect* pIntersect = (CWmfClipCommandIntersect*)pCommand;
						pOutput->IntersectClip(pIntersect->m_oClip);
						break;
					}
					case WMF_CLIPCOMMAND_EXCLUDE:
					{
						CWmfClipCommandExclude* pExclude = (CWmfClipCommandExclude*)pCommand;
						pOutput->ExcludeClip(pExclude->m_oClip, pExclude->m_oBB);
						break;
					}
				}
			}
		}
	}
}
