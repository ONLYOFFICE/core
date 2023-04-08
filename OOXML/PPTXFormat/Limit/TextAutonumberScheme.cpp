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

#include "TextAutonumberScheme.h"

namespace PPTX
{
	namespace Limit
	{		
		TextAutonumberScheme::TextAutonumberScheme()
		{
			m_strValue = L"arabicPlain";
		}
		void TextAutonumberScheme::set(const std::wstring& strValue)
		{
			if ((L"alphaLcParenBoth" == strValue) ||
				(L"alphaLcParenR" == strValue) ||
				(L"alphaLcPeriod" == strValue) ||
				(L"alphaUcParenBoth" == strValue) ||
				(L"alphaUcParenR" == strValue) ||
				(L"alphaUcPeriod" == strValue) ||
				(L"arabic1Minus" == strValue) ||
				(L"arabic2Minus" == strValue) ||
				(L"arabicDbPeriod" == strValue) ||
				(L"arabicDbPlain" == strValue) ||
				(L"arabicParenBoth" == strValue) ||
				(L"arabicParenR" == strValue) ||
				(L"arabicPeriod" == strValue) ||
				(L"arabicPlain" == strValue) ||
				(L"circleNumDbPlain" == strValue) ||
				(L"circleNumWdBlackPlain" == strValue) ||
				(L"circleNumWdWhitePlain" == strValue) ||
				(L"ea1ChsPeriod" == strValue) ||
				(L"ea1ChsPlain" == strValue) ||
				(L"ea1ChtPeriod" == strValue) ||
				(L"ea1ChtPlain" == strValue) ||
				(L"ea1JpnChsDbPeriod" == strValue) ||
				(L"ea1JpnKorPeriod" == strValue) ||
				(L"ea1JpnKorPlain" == strValue) ||
				(L"hebrew2Minus" == strValue) ||
				(L"hindiAlpha1Period" == strValue) ||
				(L"hindiAlphaPeriod" == strValue) ||
				(L"hindiNumParenR" == strValue) ||
				(L"hindiNumPeriod" == strValue) ||
				(L"romanLcParenBoth" == strValue) ||
				(L"romanLcParenR" == strValue) ||
				(L"romanLcPeriod" == strValue) ||
				(L"romanUcParenBoth" == strValue) ||
				(L"romanUcParenR" == strValue) ||
				(L"romanUcPeriod" == strValue) ||
				(L"thaiAlphaParenBoth" == strValue) ||
				(L"thaiAlphaParenR" == strValue) ||
				(L"thaiAlphaPeriod" == strValue) ||
				(L"thaiNumParenBoth" == strValue) ||
				(L"thaiNumParenR" == strValue) ||
				(L"thaiNumPeriod" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char TextAutonumberScheme::GetBYTECode() const
		{
			if (L"alphaLcParenBoth" == m_strValue) return 0;
			if (L"alphaLcParenR" == m_strValue) return 1;
			if (L"alphaLcPeriod" == m_strValue) return 2;
			if (L"alphaUcParenBoth" == m_strValue) return 3;
			if (L"alphaUcParenR" == m_strValue) return 4;
			if (L"alphaUcPeriod" == m_strValue) return 5;
			if (L"arabic1Minus" == m_strValue) return 6;
			if (L"arabic2Minus" == m_strValue) return 7;
			if (L"arabicDbPeriod" == m_strValue) return 8;
			if (L"arabicDbPlain" == m_strValue) return 9;
			if (L"arabicParenBoth" == m_strValue) return 10;
			if (L"arabicParenR" == m_strValue) return 11;
			if (L"arabicPeriod" == m_strValue) return 12;
			if (L"arabicPlain" == m_strValue) return 13;
			if (L"circleNumDbPlain" == m_strValue) return 14;
			if (L"circleNumWdBlackPlain" == m_strValue) return 15;
			if (L"circleNumWdWhitePlain" == m_strValue) return 16;
			if (L"ea1ChsPeriod" == m_strValue) return 17;
			if (L"ea1ChsPlain" == m_strValue) return 18;
			if (L"ea1ChtPeriod" == m_strValue) return 19;
			if (L"ea1ChtPlain" == m_strValue) return 20;
			if (L"ea1JpnChsDbPeriod" == m_strValue) return 21;
			if (L"ea1JpnKorPeriod" == m_strValue) return 22;
			if (L"ea1JpnKorPlain" == m_strValue) return 23;
			if (L"hebrew2Minus" == m_strValue) return 24;
			if (L"hindiAlpha1Period" == m_strValue) return 25;
			if (L"hindiAlphaPeriod" == m_strValue) return 26;
			if (L"hindiNumParenR" == m_strValue) return 27;
			if (L"hindiNumPeriod" == m_strValue) return 28;
			if (L"romanLcParenBoth" == m_strValue) return 29;
			if (L"romanLcParenR" == m_strValue) return 30;
			if (L"romanLcPeriod" == m_strValue) return 31;
			if (L"romanUcParenBoth" == m_strValue) return 32;
			if (L"romanUcParenR" == m_strValue) return 33;
			if (L"romanUcPeriod" == m_strValue) return 34;
			if (L"thaiAlphaParenBoth" == m_strValue) return 35;
			if (L"thaiAlphaParenR" == m_strValue) return 36;
			if (L"thaiAlphaPeriod" == m_strValue) return 37;
			if (L"thaiNumParenBoth" == m_strValue) return 38;
			if (L"thaiNumParenR" == m_strValue) return 39;
			if (L"thaiNumPeriod" == m_strValue) return 40;
			return 13;
		}
		void TextAutonumberScheme::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
				case 0: m_strValue = L"alphaLcParenBoth"; break;
				case 1: m_strValue = L"alphaLcParenR"; break;
				case 2: m_strValue = L"alphaLcPeriod"; break;
				case 3: m_strValue = L"alphaUcParenBoth"; break;
				case 4: m_strValue = L"alphaUcParenR"; break;
				case 5: m_strValue = L"alphaUcPeriod"; break;
				case 6: m_strValue = L"arabic1Minus"; break;
				case 7: m_strValue = L"arabic2Minus"; break;
				case 8: m_strValue = L"arabicDbPeriod"; break;
				case 9: m_strValue = L"arabicDbPlain"; break;
				case 10: m_strValue = L"arabicParenBoth"; break;
				case 11: m_strValue = L"arabicParenR"; break;
				case 12: m_strValue = L"arabicPeriod"; break;
				case 13: m_strValue = L"arabicPlain"; break;
				case 14: m_strValue = L"circleNumDbPlain"; break;
				case 15: m_strValue = L"circleNumWdBlackPlain"; break;
				case 16: m_strValue = L"circleNumWdWhitePlain"; break;
				case 17: m_strValue = L"ea1ChsPeriod"; break;
				case 18: m_strValue = L"ea1ChsPlain"; break;
				case 19: m_strValue = L"ea1ChtPeriod"; break;
				case 20: m_strValue = L"ea1ChtPlain"; break;
				case 21: m_strValue = L"ea1JpnChsDbPeriod"; break;
				case 22: m_strValue = L"ea1JpnKorPeriod"; break;
				case 23: m_strValue = L"ea1JpnKorPlain"; break;
				case 24: m_strValue = L"hebrew2Minus"; break;
				case 25: m_strValue = L"hindiAlpha1Period"; break;
				case 26: m_strValue = L"hindiAlphaPeriod"; break;
				case 27: m_strValue = L"hindiNumPeriod"; break;
				case 28: m_strValue = L"hindiNumPeriod"; break;
				case 29: m_strValue = L"romanLcParenBoth"; break;
				case 30: m_strValue = L"romanLcParenR"; break;
				case 31: m_strValue = L"romanLcPeriod"; break;
				case 32: m_strValue = L"romanUcParenBoth"; break;
				case 33: m_strValue = L"romanUcParenR"; break;
				case 34: m_strValue = L"romanUcPeriod"; break;
				case 35: m_strValue = L"thaiAlphaParenBoth"; break;
				case 36: m_strValue = L"thaiAlphaParenR"; break;
				case 37: m_strValue = L"thaiAlphaPeriod"; break;
				case 38: m_strValue = L"thaiNumParenBoth"; break;
				case 39: m_strValue = L"thaiNumParenR"; break;
				case 40: m_strValue = L"thaiNumPeriod"; break;
			default:
				m_strValue = L"arabicPlain";
				break;
			}
		}
	} // namespace Limit
} // namespace PPTX
