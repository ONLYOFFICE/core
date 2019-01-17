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
#include "SvmClip.h"

namespace MetaFile
{
	CSvmClip::CSvmClip()
	{
	}
	CSvmClip::~CSvmClip()
	{
		Clear();
	}
	void CSvmClip::operator=(CSvmClip& oClip)
	{
		Clear();
		for (unsigned int ulIndex = 0; ulIndex < oClip.m_vCommands.size(); ulIndex++)
		{
			CSvmClipCommandBase* pCommand = oClip.m_vCommands.at(ulIndex);
			CSvmClipCommandBase* pNewCommand = NULL;
			switch (pCommand->GetType())
			{
				case SVM_CLIPCOMMAND_INTERSECT:
				{
					CSvmClipCommandIntersect* pI = (CSvmClipCommandIntersect*)pCommand;
					pNewCommand = new CSvmClipCommandIntersect(pI->m_dL, pI->m_dT, pI->m_dR, pI->m_dB);
					break;
				}
				case SVM_CLIPCOMMAND_EXCLUDE:
				{
					CSvmClipCommandExclude* pE = (CSvmClipCommandExclude*)pCommand;
					pNewCommand = new CSvmClipCommandExclude(pE->m_dL, pE->m_dT, pE->m_dR, pE->m_dB, pE->m_dWindowL, pE->m_dWindowT, pE->m_dWindowR, pE->m_dWindowB);
					break;
				}
			}

			if (pNewCommand)
				m_vCommands.push_back(pNewCommand);
		}
	}
	void CSvmClip::Reset()
	{
		Clear();
	}
	bool CSvmClip::Intersect(double dL, double dT, double dR, double dB)
	{
		CSvmClipCommandBase* pCommand = new CSvmClipCommandIntersect(dL, dT, dR, dB);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	bool CSvmClip::Exclude(double dL, double dT, double dR, double dB, double dWindowL, double dWindowT, double dWindowR, double dWindowB)
	{
		CSvmClipCommandBase* pCommand = new CSvmClipCommandExclude(dL, dT, dR, dB, dWindowL, dWindowT, dWindowR, dWindowB);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	void CSvmClip::Clear()
	{
		for (unsigned int ulIndex = 0; ulIndex < m_vCommands.size(); ulIndex++)
		{
			CSvmClipCommandBase* pCommand = m_vCommands.at(ulIndex);
			delete pCommand;
		}
		m_vCommands.clear();
	}
	void CSvmClip::ClipOnRenderer(IOutputDevice* pOutput)
	{
		if (pOutput)
		{
			pOutput->ResetClip();
			for (unsigned int ulIndex = 0; ulIndex < m_vCommands.size(); ulIndex++)
			{
				CSvmClipCommandBase* pCommand = m_vCommands.at(ulIndex);

				switch (pCommand->GetType())
				{
					case SVM_CLIPCOMMAND_INTERSECT:
					{
						CSvmClipCommandIntersect* pIntersect = (CSvmClipCommandIntersect*)pCommand;

						pOutput->StartClipPath(RGN_AND, ALTERNATE);
						pOutput->MoveTo(pIntersect->m_dL, pIntersect->m_dT);
						pOutput->LineTo(pIntersect->m_dR, pIntersect->m_dT);
						pOutput->LineTo(pIntersect->m_dR, pIntersect->m_dB);
						pOutput->LineTo(pIntersect->m_dL, pIntersect->m_dB);
						pOutput->ClosePath();
						pOutput->EndClipPath(RGN_AND);

						break;
					}
					case SVM_CLIPCOMMAND_EXCLUDE:
					{
						CSvmClipCommandExclude* pExclude = (CSvmClipCommandExclude*)pCommand;

						pOutput->StartClipPath(RGN_AND, ALTERNATE);

						pOutput->MoveTo(pExclude->m_dL, pExclude->m_dT);
						pOutput->LineTo(pExclude->m_dR, pExclude->m_dT);
						pOutput->LineTo(pExclude->m_dR, pExclude->m_dB);
						pOutput->LineTo(pExclude->m_dL, pExclude->m_dB);
						pOutput->ClosePath();

						pOutput->MoveTo(pExclude->m_dWindowL, pExclude->m_dWindowT);
						pOutput->LineTo(pExclude->m_dWindowR, pExclude->m_dWindowT);
						pOutput->LineTo(pExclude->m_dWindowR, pExclude->m_dWindowB);
						pOutput->LineTo(pExclude->m_dWindowL, pExclude->m_dWindowB);
						pOutput->ClosePath();


						pOutput->EndClipPath(RGN_AND);

						break;
					}
				}
			}
		}
	}
}