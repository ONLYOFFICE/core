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
#pragma once

#include "SlideLayoutType.h"

namespace PPTX
{
	namespace Limit
	{		
		SlideLayoutType::SlideLayoutType()
		{
			m_strValue = L"blank";
		}
		void SlideLayoutType::set(const std::wstring& strValue)
		{
			if ((strValue == L"blank") ||
				(L"chart" == strValue) ||
				(L"chartAndTx" == strValue) ||
				(L"clipArtAndTx" == strValue) ||
				(L"clipArtAndVertTx" == strValue) ||
				(L"cust" == strValue) ||
				(L"dgm" == strValue) ||
				(L"fourObj" == strValue) ||
				(L"mediaAndTx" == strValue) ||
				(L"obj" == strValue) ||
				(L"objAndTwoObj" == strValue) ||
				(L"objAndTx" == strValue) ||
				(L"objOnly" == strValue) ||
				(L"objOverTx" == strValue) ||
				(L"objTx" == strValue) ||
				(L"picTx" == strValue) ||
				(L"secHead" == strValue) ||
				(L"tbl" == strValue) ||
				(L"title" == strValue) ||
				(L"titleOnly" == strValue) ||
				(L"twoColTx" == strValue) ||
				(L"twoObj" == strValue) ||
				(L"twoObjAndObj" == strValue) ||
				(L"twoObjAndTx" == strValue) ||
				(L"twoObjOverTx" == strValue) ||
				(L"twoTxTwoObj" == strValue) ||
				(L"tx" == strValue) ||
				(L"txAndChart" == strValue) ||
				(L"txAndClipArt" == strValue) ||
				(L"txAndMedia" == strValue) ||
				(L"txAndObj" == strValue) ||
				(L"txAndTwoObj" == strValue) ||
				(L"txOverObj" == strValue) ||
				(L"vertTitleAndTx" == strValue) ||
				(L"vertTitleAndTxOverChart" == strValue) ||
				(L"vertTx" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char SlideLayoutType::GetBYTECode() const
		{
			if (L"blank" == m_strValue) return 0;
			if (L"chart" == m_strValue) return 1;
			if (L"chartAndTx" == m_strValue) return 2;
			if (L"clipArtAndTx" == m_strValue) return 3;
			if (L"clipArtAndVertTx" == m_strValue) return 4;
			if (L"cust" == m_strValue) return 5;
			if (L"dgm" == m_strValue) return 6;
			if (L"fourObj" == m_strValue) return 7;
			if (L"mediaAndTx" == m_strValue) return 8;
			if (L"obj" == m_strValue) return 9;
			if (L"objAndTwoObj" == m_strValue) return 10;
			if (L"objAndTx" == m_strValue) return 11;
			if (L"objOnly" == m_strValue) return 12;
			if (L"objOverTx" == m_strValue) return 13;
			if (L"objTx" == m_strValue) return 14;
			if (L"picTx" == m_strValue) return 15;
			if (L"secHead" == m_strValue) return 16;
			if (L"tbl" == m_strValue) return 17;
			if (L"title" == m_strValue) return 18;
			if (L"titleOnly" == m_strValue) return 19;
			if (L"twoColTx" == m_strValue) return 20;
			if (L"twoObj" == m_strValue) return 21;
			if (L"twoObjAndObj" == m_strValue) return 22;
			if (L"twoObjAndTx" == m_strValue) return 23;
			if (L"twoObjOverTx" == m_strValue) return 24;
			if (L"twoTxTwoObj" == m_strValue) return 25;
			if (L"tx" == m_strValue) return 26;
			if (L"txAndChart" == m_strValue) return 27;
			if (L"txAndClipArt" == m_strValue) return 28;
			if (L"txAndMedia" == m_strValue) return 29;
			if (L"txAndObj" == m_strValue) return 30;
			if (L"txAndTwoObj" == m_strValue) return 31;
			if (L"txOverObj" == m_strValue) return 32;
			if (L"vertTitleAndTx" == m_strValue) return 33;
			if (L"vertTitleAndTxOverChart" == m_strValue) return 34;
			if (L"vertTx" == m_strValue) return 35;
			return 0;
		}
		void SlideLayoutType::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
				case 0: { m_strValue = L"blank"; break; }
				case 1: { m_strValue = L"chart"; break; }
				case 2: { m_strValue = L"chartAndTx"; break; }
				case 3: { m_strValue = L"clipArtAndTx"; break; }
				case 4: { m_strValue = L"clipArtAndVertTx"; break; }
				case 5: { m_strValue = L"cust"; break; }
				case 6: { m_strValue = L"dgm"; break; }
				case 7: { m_strValue = L"fourObj"; break; }
				case 8: { m_strValue = L"mediaAndTx"; break; }
				case 9: { m_strValue = L"obj"; break; }
				case 10: { m_strValue = L"objAndTwoObj"; break; }
				case 11: { m_strValue = L"objAndTx"; break; }
				case 12: { m_strValue = L"objOnly"; break; }
				case 13: { m_strValue = L"objOverTx"; break; }
				case 14: { m_strValue = L"objTx"; break; }
				case 15: { m_strValue = L"picTx"; break; }
				case 16: { m_strValue = L"secHead"; break; }
				case 17: { m_strValue = L"tbl"; break; }
				case 18: { m_strValue = L"title"; break; }
				case 19: { m_strValue = L"titleOnly"; break; }
				case 20: { m_strValue = L"twoColTx"; break; }
				case 21: { m_strValue = L"twoObj"; break; }
				case 22: { m_strValue = L"twoObjAndObj"; break; }
				case 23: { m_strValue = L"twoObjAndTx"; break; }
				case 24: { m_strValue = L"twoObjOverTx"; break; }
				case 25: { m_strValue = L"twoTxTwoObj"; break; }
				case 26: { m_strValue = L"tx"; break; }
				case 27: { m_strValue = L"txAndChart"; break; }
				case 28: { m_strValue = L"txAndClipArt"; break; }
				case 29: { m_strValue = L"txAndMedia"; break; }
				case 30: { m_strValue = L"txAndObj"; break; }
				case 31: { m_strValue = L"txAndTwoObj"; break; }
				case 32: { m_strValue = L"txOverObj"; break; }
				case 33: { m_strValue = L"vertTitleAndTx"; break; }
				case 34: { m_strValue = L"vertTitleAndTxOverChart"; break; }
				case 35: { m_strValue = L"vertTx"; break; }
			default:
				break;
			}
		}
	} // namespace Limit
} // namespace PPTX
