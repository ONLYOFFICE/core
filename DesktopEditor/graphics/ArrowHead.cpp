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
#include "ArrowHead.h"

namespace Aggplus
{
	CArrowHead::CArrowHead()
	{
		m_lType		= 0;
		m_bIsMoveTo = false;

		m_dX1 = 0;
		m_dY1 = 0;
		m_dX2 = 1;
		m_dY2 = 1;
	}

	CArrowHead::~CArrowHead()
	{
	}

	void CArrowHead::SetType(LONG lType)
	{
		m_lType = lType;
		
		switch (m_lType)
		{
		case Aggplus::LineCapSquareAnchor:
		case Aggplus::LineCapRoundAnchor:
		case Aggplus::LineCapDiamondAnchor:
		case Aggplus::LineCapArrowAnchor:
			break;
		default:
			m_lType = 0;
			break;
		}
	}
	bool CArrowHead::IsNeed()
	{
		return (0 != m_lType);
	}
	
	void CArrowHead::PathCommandMoveTo(double fX, double fY)
	{
		if (!IsNeed())
			return;

		m_bIsMoveTo = true;

		m_oPath.MoveTo(fX, fY);
		m_oPath.LineTo(fX, fY);
	}
	void CArrowHead::PathCommandLineTo(double fX, double fY)
	{
		if (!IsNeed())
			return;

		m_oPath.LineTo(fX, fY);
	}
	void CArrowHead::PathCommandLinesTo(double* pPoints, LONG lCount)
	{
		if (!IsNeed())
			return;
		
		if (!m_bIsMoveTo && (lCount >= 2))
		{
			PathCommandMoveTo(pPoints[0], pPoints[1]);
		}

		m_oPath.AddLines(pPoints, lCount);
	}

	void CArrowHead::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		if (!IsNeed())
			return;

		m_oPath.CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	void CArrowHead::PathCommandCurvesTo(double* pPoints, LONG lCount)
	{
		if (!IsNeed())
			return;

		if (!m_bIsMoveTo && (lCount >= 2))
		{
			PathCommandMoveTo(pPoints[0], pPoints[1]);
		}

		m_oPath.AddBeziers(pPoints, lCount);
	}
	void CArrowHead::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		if (!IsNeed())
			return;

		m_oPath.AddArc2(fX, fY, fWidth, fHeight, -fStartAngle, -fSweepAngle);
	}
	void CArrowHead::PathCommandClose()
	{
		if (!IsNeed())
			return;

		m_oPath.Reset();
	}
	void CArrowHead::PathCommandEnd()
	{
		if (!IsNeed())
			return;

		m_oPath.Reset();
		m_bIsMoveTo = false;
	}

	void CArrowHead::PathCommandStart()
	{
		if (!IsNeed())
			return;

		m_oPath.StartFigure();
	}

	bool CArrowHead::DrawPath(LONG lType)
	{
		if (!IsNeed())
			return false;

		if (lType & 0x01)
		{
			m_oPath.Transform(&m_oTransform);

			LONG lCount		= (LONG)m_oPath.GetPointCount();

			if (2 > lCount)
				return false;

			double* pPoints = new double[2 * lCount];

			m_oPath.GetPathPoints(pPoints, lCount);

			m_dX1 = pPoints[2 * lCount - 4];
			m_dY1 = pPoints[2 * lCount - 3];
			m_dX2 = pPoints[2 * lCount - 2];
			m_dY2 = pPoints[2 * lCount - 1];

			RELEASEARRAYOBJECTS(pPoints);

			return true;
		}

		return false;
	}
}