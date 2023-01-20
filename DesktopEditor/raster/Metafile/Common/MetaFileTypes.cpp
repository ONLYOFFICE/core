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
#include "MetaFileTypes.h"
#include "../Emf/EmfTypes.h"
#include "../Wmf/WmfTypes.h"

namespace MetaFile
{
	TRect::TRect()
	{
		nLeft   = 0;
		nTop    = 0;
		nRight  = 1024;
		nBottom = 1024;
	}

	TRect::TRect(int nNewLeft, int nNewTop, int nNewRight, int nNewBottom)
	    : nLeft(nNewLeft), nTop(nNewTop), nRight(nNewRight), nBottom(nNewBottom)
	{}

	TRect::TRect(const TWmfRect &oRect)
	{
		nLeft   = oRect.Left;
		nTop    = oRect.Top;
		nRight  = oRect.Right;
		nBottom = oRect.Bottom;
	}

	TRect::TRect(const TEmfRectL &oRect)
	{
		nLeft   = oRect.lLeft;
		nTop    = oRect.lTop;
		nRight  = oRect.lRight;
		nBottom = oRect.lBottom;
	}
	TRect& TRect::operator=(TWmfRect& oRect)
	{
		nLeft   = oRect.Left;
		nTop    = oRect.Top;
		nRight  = oRect.Right;
		nBottom = oRect.Bottom;
		return *this;
	}
	bool operator!=(const TRect& oLeftRect, const TRect& oRightRect)
	{
		return (oLeftRect.nLeft   != oRightRect.nLeft  &&
		        oLeftRect.nTop    != oRightRect.nTop   &&
		        oLeftRect.nRight  != oRightRect.nRight &&
		        oLeftRect.nBottom != oRightRect.nBottom);
	}
	bool operator==(const TRect& oLeftRect, const TRect& oRightRect)
	{
		return (oLeftRect.nLeft   == oRightRect.nLeft  &&
		        oLeftRect.nTop    == oRightRect.nTop   &&
		        oLeftRect.nRight  == oRightRect.nRight &&
		        oLeftRect.nBottom == oRightRect.nBottom);
	}


	TPointL::TPointL()
	{
		x = 0;
		y = 0;
	}
	TPointL::TPointL(TEmfPointL& oPoint)
	{
		x = oPoint.x;
		y = oPoint.y;
	}
	TPointL::TPointL(TWmfPointS& oPoint)
	{
		x = oPoint.x;
		y = oPoint.y;
	}
	TPointL& TPointL::operator=(TWmfPointS& oPoint)
	{
		x = oPoint.x;
		y = oPoint.y;
		return *this;
	}
	TPointL& TPointL::operator=(TEmfPointL& oPoint)
	{
		x = oPoint.x;
		y = oPoint.y;
		return *this;
	}
}
