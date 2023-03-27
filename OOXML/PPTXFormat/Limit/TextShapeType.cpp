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

#include "TextShapeType.h"

namespace PPTX
{
	namespace Limit
	{		
		TextShapeType::TextShapeType()
		{
			m_strValue = L"textNoShape";
		}
		void TextShapeType::set(const std::wstring& strValue)
		{
			if ((L"textArchDown" == strValue) ||
				(L"textArchDownPour" == strValue) ||
				(L"textArchUp" == strValue) ||
				(L"textArchUpPour" == strValue) ||
				(L"textButton" == strValue) ||
				(L"textButtonPour" == strValue) ||
				(L"textCanDown" == strValue) ||
				(L"textCanUp" == strValue) ||
				(L"textCascadeDown" == strValue) ||
				(L"textCascadeUp" == strValue) ||
				(L"textChevron" == strValue) ||
				(L"textChevronInverted" == strValue) ||
				(L"textCircle" == strValue) ||
				(L"textCirclePour" == strValue) ||
				(L"textCurveDown" == strValue) ||
				(L"textCurveUp" == strValue) ||
				(L"textDeflate" == strValue) ||
				(L"textDeflateBottom" == strValue) ||
				(L"textDeflateInflate" == strValue) ||
				(L"textDeflateInflateDeflate" == strValue) ||
				(L"textDeflateTop" == strValue) ||
				(L"textDoubleWave1" == strValue) ||
				(L"textFadeDown" == strValue) ||
				(L"textFadeLeft" == strValue) ||
				(L"textFadeRight" == strValue) ||
				(L"textFadeUp" == strValue) ||
				(L"textInflate" == strValue) ||
				(L"textInflateBottom" == strValue) ||
				(L"textInflateTop" == strValue) ||
				(L"textNoShape" == strValue) ||
				(L"textPlain" == strValue) ||
				(L"textRingInside" == strValue) ||
				(L"textRingOutside" == strValue) ||
				(L"textSlantDown" == strValue) ||
				(L"textSlantUp" == strValue) ||
				(L"textStop" == strValue) ||
				(L"textTriangle" == strValue) ||
				(L"textTriangleInverted" == strValue) ||
				(L"textWave1" == strValue) ||
				(L"textWave2" == strValue) ||
				(L"textWave4" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char TextShapeType::GetBYTECode() const
		{
			if (L"textArchDown" == m_strValue) return 0;
			if (L"textArchDownPour" == m_strValue) return 1;
			if (L"textArchUp" == m_strValue) return 2;
			if (L"textArchUpPour" == m_strValue) return 3;
			if (L"textButton" == m_strValue) return 4;
			if (L"textButtonPour" == m_strValue) return 5;
			if (L"textCanDown" == m_strValue) return 6;
			if (L"textCanUp" == m_strValue) return 7;
			if (L"textCascadeDown" == m_strValue) return 8;
			if (L"textCascadeUp" == m_strValue) return 9;
			if (L"textChevron" == m_strValue) return 10;
			if (L"textChevronInverted" == m_strValue) return 11;
			if (L"textCircle" == m_strValue) return 12;
			if (L"textCirclePour" == m_strValue) return 13;
			if (L"textCurveDown" == m_strValue) return 14;
			if (L"textCurveUp" == m_strValue) return 15;
			if (L"textDeflate" == m_strValue) return 16;
			if (L"textDeflateBottom" == m_strValue) return 17;
			if (L"textDeflateInflate" == m_strValue) return 18;
			if (L"textDeflateInflateDeflate" == m_strValue) return 19;
			if (L"textDeflateTop" == m_strValue) return 20;
			if (L"textDoubleWave1" == m_strValue) return 21;
			if (L"textFadeDown" == m_strValue) return 22;
			if (L"textFadeLeft" == m_strValue) return 23;
			if (L"textFadeRight" == m_strValue) return 24;
			if (L"textFadeUp" == m_strValue) return 25;
			if (L"textInflate" == m_strValue) return 26;
			if (L"textInflateBottom" == m_strValue) return 27;
			if (L"textInflateTop" == m_strValue) return 28;
			if (L"textNoShape" == m_strValue) return 29;
			if (L"textPlain" == m_strValue) return 30;
			if (L"textRingInside" == m_strValue) return 31;
			if (L"textRingOutside" == m_strValue) return 32;
			if (L"textSlantDown" == m_strValue) return 33;
			if (L"textSlantUp" == m_strValue) return 34;
			if (L"textStop" == m_strValue) return 35;
			if (L"textTriangle" == m_strValue) return 36;
			if (L"textTriangleInverted" == m_strValue) return 37;
			if (L"textWave1" == m_strValue) return 38;
			if (L"textWave2" == m_strValue) return 39;
			if (L"textWave4" == m_strValue) return 40;
			return 29;
		}
		void TextShapeType::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
			case 0: m_strValue = L"textArchDown"; break;
			case 1: m_strValue = L"textArchDownPour"; break;
			case 2: m_strValue = L"textArchUp"; break;
			case 3: m_strValue = L"textArchUpPour"; break;
			case 4: m_strValue = L"textButton"; break;
			case 5: m_strValue = L"textButtonPour"; break;
			case 6: m_strValue = L"textCanDown"; break;
			case 7: m_strValue = L"textCanUp"; break;
			case 8: m_strValue = L"textCascadeDown"; break;
			case 9: m_strValue = L"textCascadeUp"; break;
			case 10: m_strValue = L"textChevron"; break;
			case 11: m_strValue = L"textChevronInverted"; break;
			case 12: m_strValue = L"textCircle"; break;
			case 13: m_strValue = L"textCirclePour"; break;
			case 14: m_strValue = L"textCurveDown"; break;
			case 15: m_strValue = L"textCurveUp"; break;
			case 16: m_strValue = L"textDeflate"; break;
			case 17: m_strValue = L"textDeflateBottom"; break;
			case 18: m_strValue = L"textDeflateInflate"; break;
			case 19: m_strValue = L"textDeflateInflateDeflate"; break;
			case 20: m_strValue = L"textDeflateTop"; break;
			case 21: m_strValue = L"textDoubleWave1"; break;
			case 22: m_strValue = L"textFadeDown"; break;
			case 23: m_strValue = L"textFadeLeft"; break;
			case 24: m_strValue = L"textFadeRight"; break;
			case 25: m_strValue = L"textFadeUp"; break;
			case 26: m_strValue = L"textInflate"; break;
			case 27: m_strValue = L"textInflateBottom"; break;
			case 28: m_strValue = L"textInflateTop"; break;
			case 29: m_strValue = L"textNoShape"; break;
			case 30: m_strValue = L"textPlain"; break;
			case 31: m_strValue = L"textRingInside"; break;
			case 32: m_strValue = L"textRingOutside"; break;
			case 33: m_strValue = L"textSlantDown"; break;
			case 34: m_strValue = L"textSlantUp"; break;
			case 35: m_strValue = L"textStop"; break;
			case 36: m_strValue = L"textTriangle"; break;
			case 37: m_strValue = L"textTriangleInverted"; break;
			case 38: m_strValue = L"textWave1"; break;
			case 39: m_strValue = L"textWave2"; break;
			case 40: m_strValue = L"textWave4"; break;
			default:m_strValue = L"textNoShape"; break;
			}
		}
	} // namespace Limit
} // namespace PPTX
