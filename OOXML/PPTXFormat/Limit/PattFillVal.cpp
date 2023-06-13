/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "PattFillVal.h"

namespace PPTX
{
	namespace Limit
	{		
		PattFillVal::PattFillVal()
		{
			m_strValue = L"cross";
		}
		void PattFillVal::set(const std::wstring& strValue)
		{
			if ((L"cross" == strValue) || //(Cross)
				(L"dashDnDiag" == strValue) || //(Dashed Downward Diagonal)
				(L"dashHorz" == strValue) || //(Dashed Horizontal)
				(L"dashUpDiag" == strValue) || //(Dashed Upward DIagonal)
				(L"dashVert" == strValue) || //(Dashed Vertical)
				(L"diagBrick" == strValue) || //(Diagonal Brick)
				(L"diagCross" == strValue) || //(Diagonal Cross)
				(L"divot" == strValue) || //(Divot)
				(L"dkDnDiag" == strValue) || //(Dark Downward Diagonal)
				(L"dkHorz" == strValue) || //(Dark Horizontal)
				(L"dkUpDiag" == strValue) || //(Dark Upward Diagonal)
				(L"dkVert" == strValue) || //(Dark Vertical)
				(L"dnDiag" == strValue) || //(Downward Diagonal)
				(L"dotDmnd" == strValue) || //(Dotted Diamond)
				(L"dotGrid" == strValue) || //(Dotted Grid)
				(L"horz" == strValue) || //(Horizontal)
				(L"horzBrick" == strValue) || //(Horizontal Brick)
				(L"lgCheck" == strValue) || //(Large Checker Board)
				(L"lgConfetti" == strValue) || //(Large Confetti)
				(L"lgGrid" == strValue) || //(Large Grid)
				(L"ltDnDiag" == strValue) || //(Light Downward Diagonal)
				(L"ltHorz" == strValue) || //(Light Horizontal)
				(L"ltUpDiag" == strValue) || //(Light Upward Diagonal)
				(L"ltVert" == strValue) || //(Light Vertical)
				(L"narHorz" == strValue) || //(Narrow Horizontal)
				(L"narVert" == strValue) || //(Narrow Vertical)
				(L"openDmnd" == strValue) || //(Open Diamond)
				(L"pct10" == strValue) || //(10%)
				(L"pct20" == strValue) || //(20%)
				(L"pct25" == strValue) || //(25%)
				(L"pct30" == strValue) || //(30%)
				(L"pct40" == strValue) || //(40%)
				(L"pct5" == strValue) || //(5%)
				(L"pct50" == strValue) || //(50%)
				(L"pct60" == strValue) || //(60%)
				(L"pct70" == strValue) || //(70%)
				(L"pct75" == strValue) || //(75%)
				(L"pct80" == strValue) || //(80%)
				(L"pct90" == strValue) || //(90%)
				(L"plaid" == strValue) || //(Plaid)
				(L"shingle" == strValue) || //(Shingle)
				(L"smCheck" == strValue) || //(Small Checker Board)
				(L"smConfetti" == strValue) || //(Small Confetti)
				(L"smGrid" == strValue) || //(Small Grid)
				(L"solidDmnd" == strValue) || //(Solid Diamond)
				(L"sphere" == strValue) || //(Sphere)
				(L"trellis" == strValue) || //(Trellis)
				(L"upDiag" == strValue) || //(Upward Diagonal)
				(L"vert" == strValue) || //(Vertical)
				(L"wave" == strValue) || //(Wave)
				(L"wdDnDiag" == strValue) || //(Wide Downward Diagonal)
				(L"wdUpDiag" == strValue) || //(Wide Upward Diagonal)
				(L"weave" == strValue) || //(Weave)
				(L"zigZag" == strValue))//(Zig Zag)
			{
				m_strValue = strValue;
			}
		}
		unsigned char PattFillVal::GetBYTECode() const
		{
			if (L"cross" == m_strValue)       return 0;
			if (L"dashDnDiag" == m_strValue)  return 1;
			if (L"dashHorz" == m_strValue)    return 2;
			if (L"dashUpDiag" == m_strValue)  return 3;
			if (L"dashVert" == m_strValue)    return 4;
			if (L"diagBrick" == m_strValue)   return 5;
			if (L"diagCross" == m_strValue)   return 6;
			if (L"divot" == m_strValue)       return 7;
			if (L"dkDnDiag" == m_strValue)    return 8;
			if (L"dkHorz" == m_strValue)      return 9;
			if (L"dkUpDiag" == m_strValue)    return 10;
			if (L"dkVert" == m_strValue)      return 11;
			if (L"dnDiag" == m_strValue)      return 12;
			if (L"dotDmnd" == m_strValue)     return 13;
			if (L"dotGrid" == m_strValue)     return 14;
			if (L"horz" == m_strValue)        return 15;
			if (L"horzBrick" == m_strValue)   return 16;
			if (L"lgCheck" == m_strValue)     return 17;
			if (L"lgConfetti" == m_strValue)  return 18;
			if (L"lgGrid" == m_strValue)      return 19;
			if (L"ltDnDiag" == m_strValue)    return 20;
			if (L"ltHorz" == m_strValue)      return 21;
			if (L"ltUpDiag" == m_strValue)    return 22;
			if (L"ltVert" == m_strValue)      return 23;
			if (L"narHorz" == m_strValue)     return 24;
			if (L"narVert" == m_strValue)     return 25;
			if (L"openDmnd" == m_strValue)    return 26;
			if (L"pct10" == m_strValue)       return 27;
			if (L"pct20" == m_strValue)       return 28;
			if (L"pct25" == m_strValue)       return 29;
			if (L"pct30" == m_strValue)       return 30;
			if (L"pct40" == m_strValue)       return 31;
			if (L"pct5" == m_strValue)        return 32;
			if (L"pct50" == m_strValue)       return 33;
			if (L"pct60" == m_strValue)       return 34;
			if (L"pct70" == m_strValue)       return 35;
			if (L"pct75" == m_strValue)       return 36;
			if (L"pct80" == m_strValue)       return 37;
			if (L"pct90" == m_strValue)       return 38;
			if (L"plaid" == m_strValue)       return 39;
			if (L"shingle" == m_strValue)     return 40;
			if (L"smCheck" == m_strValue)     return 41;
			if (L"smConfetti" == m_strValue)  return 42;
			if (L"smGrid" == m_strValue)      return 43;
			if (L"solidDmnd" == m_strValue)   return 44;
			if (L"sphere" == m_strValue)      return 45;
			if (L"trellis" == m_strValue)     return 46;
			if (L"upDiag" == m_strValue)      return 47;
			if (L"vert" == m_strValue)        return 48;
			if (L"wave" == m_strValue)        return 49;
			if (L"wdDnDiag" == m_strValue)    return 50;
			if (L"wdUpDiag" == m_strValue)    return 51;
			if (L"weave" == m_strValue)       return 52;
			if (L"zigZag" == m_strValue)      return 53;
			return 0;
		}
		void PattFillVal::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
			case 0: m_strValue = L"cross"; break;
			case 1: m_strValue = L"dashDnDiag"; break;
			case 2: m_strValue = L"dashHorz"; break;
			case 3: m_strValue = L"dashUpDiag"; break;
			case 4: m_strValue = L"dashVert"; break;
			case 5: m_strValue = L"diagBrick"; break;
			case 6: m_strValue = L"diagCross"; break;
			case 7: m_strValue = L"divot"; break;
			case 8: m_strValue = L"dkDnDiag"; break;
			case 9: m_strValue = L"dkHorz"; break;
			case 10: m_strValue = L"dkUpDiag"; break;
			case 11: m_strValue = L"dkVert"; break;
			case 12: m_strValue = L"dnDiag"; break;
			case 13: m_strValue = L"dotDmnd"; break;
			case 14: m_strValue = L"dotGrid"; break;
			case 15: m_strValue = L"horz"; break;
			case 16: m_strValue = L"horzBrick"; break;
			case 17: m_strValue = L"lgCheck"; break;
			case 18: m_strValue = L"lgConfetti"; break;
			case 19: m_strValue = L"lgGrid"; break;
			case 20: m_strValue = L"ltDnDiag"; break;
			case 21: m_strValue = L"ltHorz"; break;
			case 22: m_strValue = L"ltUpDiag"; break;
			case 23: m_strValue = L"ltVert"; break;
			case 24: m_strValue = L"narHorz"; break;
			case 25: m_strValue = L"narVert"; break;
			case 26: m_strValue = L"openDmnd"; break;
			case 27: m_strValue = L"pct10"; break;
			case 28: m_strValue = L"pct20"; break;
			case 29: m_strValue = L"pct25"; break;
			case 30: m_strValue = L"pct30"; break;
			case 31: m_strValue = L"pct40"; break;
			case 32: m_strValue = L"pct5"; break;
			case 33: m_strValue = L"pct50"; break;
			case 34: m_strValue = L"pct60"; break;
			case 35: m_strValue = L"pct70"; break;
			case 36: m_strValue = L"pct75"; break;
			case 37: m_strValue = L"pct80"; break;
			case 38: m_strValue = L"pct90"; break;
			case 39: m_strValue = L"plaid"; break;
			case 40: m_strValue = L"shingle"; break;
			case 41: m_strValue = L"smCheck"; break;
			case 42: m_strValue = L"smConfetti"; break;
			case 43: m_strValue = L"smGrid"; break;
			case 44: m_strValue = L"solidDmnd"; break;
			case 45: m_strValue = L"sphere"; break;
			case 46: m_strValue = L"trellis"; break;
			case 47: m_strValue = L"upDiag"; break;
			case 48: m_strValue = L"vert"; break;
			case 49: m_strValue = L"wave"; break;
			case 50: m_strValue = L"wdDnDiag"; break;
			case 51: m_strValue = L"wdUpDiag"; break;
			case 52: m_strValue = L"weave"; break;
			case 53: m_strValue = L"zigZag"; break;
			default:
				m_strValue = L"cross";
				break;
			}
		}
	} // namespace Limit
} // namespace PPTX
