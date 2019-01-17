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
#ifndef _BUILD_ARROW_HEAD_H_
#define _BUILD_ARROW_HEAD_H_

#include "GraphicsPath.h"

namespace Aggplus
{
	class CArrowHead
	{
	private:
		LONG			m_lType;
		CMatrix			m_oTransform;

		bool m_bIsMoveTo;		
		CGraphicsPath	m_oPath;
		
	public:
		double m_dX1;
		double m_dY1;
		double m_dX2;
		double m_dY2;

	public:
		CArrowHead();
		~CArrowHead();

	public:

		void SetType(LONG lType);
		bool IsNeed();
		
		void PathCommandMoveTo(double fX, double fY);
		void PathCommandLineTo(double fX, double fY);
		void PathCommandLinesTo(double* pPoints, LONG lCount);
		void PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3);
		void PathCommandCurvesTo(double* pPoints, LONG lCount);
		void PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
		void PathCommandClose();
		void PathCommandEnd();
		void PathCommandStart();

		bool DrawPath(LONG lType);
	};
}

#endif // _BUILD_ARROW_HEAD_H_