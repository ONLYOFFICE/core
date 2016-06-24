/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#include "EmfClip.h"

namespace MetaFile
{
	CEmfClip::CEmfClip()
	{

	}
	CEmfClip::~CEmfClip()
	{
		Clear();
	}
	void CEmfClip::operator=(CEmfClip& oClip)
	{
		Clear();
		for (unsigned int ulIndex = 0; ulIndex < oClip.m_vCommands.size(); ulIndex++)
		{
			CEmfClipCommandBase* pCommand = oClip.m_vCommands.at(ulIndex);
			CEmfClipCommandBase* pNewCommand = NULL;
			switch (pCommand->GetType())
			{
				case EMF_CLIPCOMMAND_INTERSECT:
				{
					pNewCommand = new CEmfClipCommandIntersect(((CEmfClipCommandIntersect*)pCommand)->m_oRect);
					break;
				}
				case EMF_CLIPCOMMAND_SETPATH:
				{
					CEmfClipCommandPath* pPathCommand = (CEmfClipCommandPath*)pCommand;
					pNewCommand = new CEmfClipCommandPath(&pPathCommand->m_oPath, pPathCommand->m_unMode);
					break;
				}
				case EMF_CLIPCOMMAND_EXCLUDE:
				{
					CEmfClipCommandExclude* pExclude = (CEmfClipCommandExclude*)pCommand;
					pNewCommand = new CEmfClipCommandExclude(pExclude->m_oClip, pExclude->m_oBB);
					break;
				}
			}

			if (pNewCommand)
				m_vCommands.push_back(pNewCommand);
		}
	}
	void CEmfClip::Reset()
	{
		Clear();
	}
	bool CEmfClip::Intersect(TRectD& oRect)
	{
		CEmfClipCommandBase* pCommand = new CEmfClipCommandIntersect(oRect);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	bool CEmfClip::Exclude(TRectD& oClip, TRectD& oBB)
	{
		CEmfClipCommandBase* pCommand = new CEmfClipCommandExclude(oClip, oBB);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	bool CEmfClip::SetPath(CEmfPath* pPath, unsigned int unMode)
	{
		CEmfClipCommandBase* pCommand = new CEmfClipCommandPath(pPath, unMode);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	void CEmfClip::ClipOnRenderer(IOutputDevice* pOutput)
	{
		if (!pOutput)
			return;

		pOutput->ResetClip();
		for (unsigned int ulIndex = 0; ulIndex < m_vCommands.size(); ulIndex++)
		{
			CEmfClipCommandBase* pCommand = m_vCommands.at(ulIndex);
			switch (pCommand->GetType())
			{
				case EMF_CLIPCOMMAND_INTERSECT:
				{
					CEmfClipCommandIntersect* pIntersect = (CEmfClipCommandIntersect*)pCommand;
					pOutput->IntersectClip(pIntersect->m_oRect.dLeft, pIntersect->m_oRect.dTop, pIntersect->m_oRect.dRight, pIntersect->m_oRect.dBottom);
					break;
				}
				case EMF_CLIPCOMMAND_SETPATH:
				{
					CEmfClipCommandPath* pClipPath = (CEmfClipCommandPath*)pCommand;
					pClipPath->m_oPath.Draw(pOutput, false, false, pClipPath->m_unMode);
					break;
				}
				case EMF_CLIPCOMMAND_EXCLUDE:
				{
					CEmfClipCommandExclude* pExclude = (CEmfClipCommandExclude*)pCommand;

					pOutput->StartClipPath(RGN_AND, ALTERNATE);

					TRectD& oClip = pExclude->m_oClip;
					TRectD& oBB = pExclude->m_oBB;

					pOutput->MoveTo(oClip.dLeft,  oClip.dTop);
					pOutput->LineTo(oClip.dRight, oClip.dTop);
					pOutput->LineTo(oClip.dRight, oClip.dBottom);
					pOutput->LineTo(oClip.dLeft,  oClip.dBottom);
					pOutput->ClosePath();

					pOutput->MoveTo(oBB.dLeft,  oBB.dTop);
					pOutput->LineTo(oBB.dRight, oBB.dTop);
					pOutput->LineTo(oBB.dRight, oBB.dBottom);
					pOutput->LineTo(oBB.dLeft,  oBB.dBottom);
					pOutput->ClosePath();

					pOutput->EndClipPath(RGN_AND);
					break;
				}
			}
		}

	}
	void CEmfClip::Clear()
	{
		for (unsigned int ulIndex = 0; ulIndex < m_vCommands.size(); ulIndex++)
		{
			CEmfClipCommandBase* pCommand = m_vCommands.at(ulIndex);
			delete pCommand;
		}
		m_vCommands.clear();
	}
}