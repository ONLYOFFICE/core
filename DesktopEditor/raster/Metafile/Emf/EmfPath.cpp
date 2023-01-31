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
#include "EmfPath.h"

namespace MetaFile
{
	CEmfPath::CEmfPath()
	{
	}
	CEmfPath::CEmfPath(CEmfPath* pPath)
	{
		if (pPath)
		{
			for (unsigned int unIndex = 0; unIndex < pPath->m_pCommands.size(); unIndex++)
			{
				CEmfPathCommandBase* pCommand = pPath->m_pCommands.at(unIndex);
				switch (pCommand->GetType())
				{
					case EMF_PATHCOMMAND_MOVETO:
					{
						CEmfPathMoveTo* pMoveTo = (CEmfPathMoveTo*)pCommand;
						MoveTo(pMoveTo->x, pMoveTo->y);
						break;
					}
					case EMF_PATHCOMMAND_LINETO:
					{
						CEmfPathLineTo* pLineTo = (CEmfPathLineTo*)pCommand;
						LineTo(pLineTo->x, pLineTo->y);
						break;
					}
					case EMF_PATHCOMMAND_CURVETO:
					{
						CEmfPathCurveTo* pCurveTo = (CEmfPathCurveTo*)pCommand;
						CurveTo(pCurveTo->x1, pCurveTo->y1, pCurveTo->x2, pCurveTo->y2, pCurveTo->xE, pCurveTo->yE);
						break;
					}
					case EMF_PATHCOMMAND_ARCTO:
					{
						CEmfPathArcTo* pArcTo = (CEmfPathArcTo*)pCommand;
						ArcTo(pArcTo->left, pArcTo->top, pArcTo->right, pArcTo->bottom, pArcTo->start, pArcTo->sweep);
						break;
					}
					case EMF_PATHCOMMAND_CLOSE:
					{
						Close();
						break;
					}
				}
			}
		}
	}
	CEmfPath::~CEmfPath()
	{
		Clear();
	}	
	bool CEmfPath::MoveTo(double dX, double dY)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathMoveTo(dX, dY);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}	
	bool CEmfPath::LineTo(double dX, double dY)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathLineTo(dX, dY);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}	
	bool CEmfPath::CurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathCurveTo(dX1, dY1, dX2, dY2, dXE, dYE);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::ArcTo(double dL, double dT, double dR, double dB, double dStart, double dSweep)
	{
		CEmfPathCommandBase* pCommand = new CEmfPathArcTo(dL, dT, dR, dB, dStart, dSweep);
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	bool CEmfPath::Close()
	{
		CEmfPathCommandBase* pCommand = new CEmfPathClose();
		if (!pCommand)
			return false;

		m_pCommands.push_back(pCommand);

		return true;
	}
	void CEmfPath::Draw(IOutputDevice* pOutput, bool bStroke, bool bFill, int nClipMode)
	{
		if (pOutput)
		{	
			if (-1 != nClipMode)
				pOutput->StartClipPath(nClipMode);
			else
				pOutput->StartPath();

			for (unsigned int ulIndex = 0; ulIndex < m_pCommands.size(); ulIndex++)
			{
				CEmfPathCommandBase* pCommand = m_pCommands.at(ulIndex);
				switch (pCommand->GetType())
				{
					case EMF_PATHCOMMAND_MOVETO:
					{
						CEmfPathMoveTo* pMoveTo = (CEmfPathMoveTo*)pCommand;
						pOutput->MoveTo(pMoveTo->x, pMoveTo->y);
						break;
					}
					case EMF_PATHCOMMAND_LINETO:
					{
						CEmfPathLineTo* pLineTo = (CEmfPathLineTo*)pCommand;
						pOutput->LineTo(pLineTo->x, pLineTo->y);
						break;
					}
					case EMF_PATHCOMMAND_CURVETO:
					{
						CEmfPathCurveTo* pCurveTo = (CEmfPathCurveTo*)pCommand;
						pOutput->CurveTo(pCurveTo->x1, pCurveTo->y1, pCurveTo->x2, pCurveTo->y2, pCurveTo->xE, pCurveTo->yE);
						break;
					}
					case EMF_PATHCOMMAND_ARCTO:
					{
						CEmfPathArcTo* pArcTo = (CEmfPathArcTo*)pCommand;
						pOutput->ArcTo(pArcTo->left, pArcTo->top, pArcTo->right, pArcTo->bottom, pArcTo->start, pArcTo->sweep);
						break;
					}
					case EMF_PATHCOMMAND_CLOSE:
					{
						pOutput->ClosePath();
						break;
					}
				}
			}

			if (-1 == nClipMode)
			{
				int lType = (bStroke ? 1 : 0) + (bFill ? 2 : 0);
				pOutput->DrawPath(lType);
				pOutput->EndPath();
			}
			else
				pOutput->EndClipPath(nClipMode);
		}

		// При клипе пат не очищаем
		if (-1 == nClipMode)
			Clear();
	}
	void CEmfPath::DrawWithoutClean(IOutputDevice* pOutput, bool bStroke, bool bFill)
	{
		if (pOutput)
		{
			for (unsigned int ulIndex = 0; ulIndex < m_pCommands.size(); ulIndex++)
			{
				CEmfPathCommandBase* pCommand = m_pCommands.at(ulIndex);
				switch (pCommand->GetType())
				{
					case EMF_PATHCOMMAND_MOVETO:
					{
						CEmfPathMoveTo* pMoveTo = (CEmfPathMoveTo*)pCommand;
						pOutput->MoveTo(pMoveTo->x, pMoveTo->y);
						break;
					}
					case EMF_PATHCOMMAND_LINETO:
					{
						CEmfPathLineTo* pLineTo = (CEmfPathLineTo*)pCommand;
						pOutput->LineTo(pLineTo->x, pLineTo->y);
						break;
					}
					case EMF_PATHCOMMAND_CURVETO:
					{
						CEmfPathCurveTo* pCurveTo = (CEmfPathCurveTo*)pCommand;
						pOutput->CurveTo(pCurveTo->x1, pCurveTo->y1, pCurveTo->x2, pCurveTo->y2, pCurveTo->xE, pCurveTo->yE);
						break;
					}
					case EMF_PATHCOMMAND_ARCTO:
					{
						CEmfPathArcTo* pArcTo = (CEmfPathArcTo*)pCommand;
						pOutput->ArcTo(pArcTo->left, pArcTo->top, pArcTo->right, pArcTo->bottom, pArcTo->start, pArcTo->sweep);
						break;
					}
					case EMF_PATHCOMMAND_CLOSE:
					{
						pOutput->ClosePath();
						break;
					}
				}
			}

			int lType = (bStroke ? 1 : 0) + (bFill ? 2 : 0);
			pOutput->DrawPath(lType);
			pOutput->EndPath();
		}
	}

	void CEmfPath::Clear()
	{
		for (unsigned int ulIndex = 0; ulIndex < m_pCommands.size(); ulIndex++)
		{
			CEmfPathCommandBase* pCommand = m_pCommands.at(ulIndex);
			delete pCommand;
		}
		m_pCommands.clear();
	}
}
