/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "CPathConverter.h"

namespace MetaFile
{
	CPathConverter::CPathConverter()
	{}

	CPathConverter::~CPathConverter()
	{}

	void CPathConverter::GetUpdatedPath(CPath &oNewPath, CPath &oLineCapPath, const CPath &oPath, const CEmfPlusPen &oPen)
	{
		TEmfPlusPointF oLastPoint(0, 0);

		for (const CPathCommandBase* pCommand : oPath.GetCommands())
		{
			switch (pCommand->GetType())
			{
				case PATH_COMMAND_MOVETO:
				{
					CPathCommandMoveTo *pCommandMoveTo = (CPathCommandMoveTo*)pCommand;
					oNewPath.MoveTo(pCommandMoveTo->GetX(), pCommandMoveTo->GetY());

					oLastPoint.X = pCommandMoveTo->GetX();
					oLastPoint.Y = pCommandMoveTo->GetY();

					break;
				}
				case PATH_COMMAND_LINETO:
				{
					CPathCommandLineTo *pCommandLineTo = (CPathCommandLineTo*)pCommand;

					double dAngle = -90 + (atan((pCommandLineTo->GetY() - oLastPoint.Y) /
					                            (pCommandLineTo->GetX() - oLastPoint.X)) * 180 / 3.14);

					if (AddLineStartCap(oLineCapPath, oPen, oLastPoint.X, oLastPoint.Y, dAngle + 180))
						oNewPath.MoveTo(oLastPoint.X, oLastPoint.Y);

					oLastPoint.X = pCommandLineTo->GetX();
					oLastPoint.Y = pCommandLineTo->GetY();

					AddLineEndCap(oLineCapPath, oPen, oLastPoint.X, oLastPoint.Y, dAngle);

					oNewPath.LineTo(oLastPoint.X,  oLastPoint.Y);

					break;
				}
				case PATH_COMMAND_CURVETO:
				{
					CPathCommandCurveTo *pCommandCurveTo = (CPathCommandCurveTo*)pCommand;

					oNewPath.CurveTo(pCommandCurveTo->GetX1(), pCommandCurveTo->GetY1(),
					                 pCommandCurveTo->GetX2(), pCommandCurveTo->GetY2(),
					                 pCommandCurveTo->GetXE(), pCommandCurveTo->GetYE());

					break;
				}
				case PATH_COMMAND_ARCTO:
				{
					CPathCommandArcTo *pCommandArcTo = (CPathCommandArcTo*)pCommand;

					oNewPath.ArcTo(pCommandArcTo->GetLeft(),       pCommandArcTo->GetTop(),
					               pCommandArcTo->GetRight(),      pCommandArcTo->GetBottom(),
					               pCommandArcTo->GetStartAngle(), pCommandArcTo->GetSweepAngle());

					break;
				}
				case PATH_COMMAND_CLOSE:
				{
					oNewPath.Close();
					break;
				}
				case PATH_COMMAND_UNKNOWN: break;
			}
		}

	}

	bool CPathConverter::AddLineStartCap(CPath &oPath, const CEmfPlusPen &oPen, double& dX, double& dY, double dAngle)
	{
		if (NULL != oPen.pLineStartCapData)
		{
			if (CustomLineCapDataTypeDefault == oPen.pLineStartCapData->GetType() && NULL != ((TEmfPlusCustomLineCapData*)oPen.pLineStartCapData)->pPath)
			{
				return AddLineCap(oPath, *(((TEmfPlusCustomLineCapData*)oPen.pLineStartCapData)->pPath), dX, dY, dAngle, oPen.dWidth);
			}
		}

		return false;
	}

	bool CPathConverter::AddLineEndCap(CPath &oPath, const CEmfPlusPen &oPen, double &dX, double &dY, double dAngle)
	{
		if (NULL != oPen.pLineEndCapData)
		{
			if (CustomLineCapDataTypeDefault == oPen.pLineEndCapData->GetType() && NULL != ((TEmfPlusCustomLineCapData*)oPen.pLineEndCapData)->pPath)
			{
				return AddLineCap(oPath, *((TEmfPlusCustomLineCapData*)oPen.pLineEndCapData)->pPath, dX, dY, dAngle, oPen.dWidth);
			}
		}

		return false;
	}

	bool CPathConverter::AddLineCap(CPath &oPath, const CPath &oLineCapPath, double &dX, double &dY, double dAngle, double dPenWidth)
	{
		double dNewX = 0;
		double dNewY = 0;

		double dSin = sin(dAngle * 3.14 / 180);
		double dCos = cos(dAngle * 3.14 / 180);

		const std::vector<CPathCommandBase*> arCommands(oLineCapPath.GetCommands());
		
		for (const CPathCommandBase* pCommand : arCommands)
		{
			switch (pCommand->GetType())
			{
			case PATH_COMMAND_MOVETO:
			{
				CPathCommandMoveTo *pCommandMoveTo = (CPathCommandMoveTo*)pCommand;

				double dTempX = pCommandMoveTo->GetX() * dPenWidth * dCos - pCommandMoveTo->GetY() * dPenWidth * dSin;
				double dTempY = pCommandMoveTo->GetX() * dPenWidth * dSin + pCommandMoveTo->GetY() * dPenWidth * dCos;

				dNewX += dTempX;
				dNewY += dTempY;

				oPath.MoveTo(dX + dTempX, dY + dTempY);
				break;
			}
			case PATH_COMMAND_LINETO:
			{
				CPathCommandLineTo *pCommandLineTo = (CPathCommandLineTo*)pCommand;

				double dTempX = pCommandLineTo->GetX() * dPenWidth * dCos - pCommandLineTo->GetY() * dPenWidth * dSin;
				double dTempY = pCommandLineTo->GetX() * dPenWidth * dSin + pCommandLineTo->GetY() * dPenWidth * dCos;

				dNewX += dTempX;
				dNewY += dTempY;

				oPath.LineTo(dX + dTempX, dY + dTempY);
				break;
			}
			case PATH_COMMAND_CURVETO:
			{
				break;
			}
			case PATH_COMMAND_ARCTO:
			{
				break;
			}
			case PATH_COMMAND_CLOSE:
			{
				oPath.Close();
				break;
			}
			case PATH_COMMAND_UNKNOWN: break;
			}
		}

		if (arCommands.size() > 1)
		{
			dX = dX + dNewX / (arCommands.size() - 1);
			dY = dY + dNewY / (arCommands.size() - 1);

			return true;
		}

		return false;
	}
}
