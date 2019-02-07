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
#pragma once

namespace PPT_FORMAT
{
	class CTheme;
}
class CStylesWriter
{
public:
	CStylesWriter(PPT_FORMAT::CTheme* m_pTheme);
	CStylesWriter();

	PPT_FORMAT::CTheme* m_pTheme;
	
	void ConvertStyles(PPT_FORMAT::CTextStyles& oStyles, PPT_FORMAT::CStringWriter& oWriter, int nCount = 10)
	{
		for (int i = 0; i < nCount; ++i)
		{
			if (oStyles.m_pLevels[i].is_init())
				ConvertStyleLevel(oStyles.m_pLevels[i].get(), oWriter, i);
		}
	}
	std::wstring ConvertStyleLevel(PPT_FORMAT::CTextStyleLevel& oLevel, const int& nLevel)
	{
		PPT_FORMAT::CStringWriter oWriter;
		ConvertStyleLevel(oLevel, oWriter, nLevel);
		return oWriter.GetData();
	}

	void ConvertStyleLevel(PPT_FORMAT::CTextStyleLevel& oLevel,
		PPT_FORMAT::CStringWriter& oWriter, const int& nLevel);

    std::wstring ConvertStyles(PPT_FORMAT::CTextStyles& oStyles, int nCount = 10)
	{
		PPT_FORMAT::CStringWriter oWriter;
		ConvertStyles(oStyles, oWriter, nCount);
		return oWriter.GetData();
	}
	AVSINLINE static std::wstring GetTextAnchor(const WORD& value)
	{
		if (0 == value)			return L"t";
		if (1 == value)			return L"ctr";
		if (2 == value)			return L"b";
		return L"t";
	}

	AVSINLINE static std::wstring GetTextAlign(const WORD& value)
	{
		if (0 == value)			return L"l";
		if (1 == value)			return L"ctr";
		if (2 == value)			return L"r";
		if (3 == value)			return L"just";
		if (4 == value)			return L"dist";
		if (5 == value)			return L"thaiDist";
		if (6 == value)			return L"justLow";
		return L"l";
	}
	AVSINLINE static std::wstring GetColorInScheme(const LONG& lIndex)
	{
		switch (lIndex)
		{
		case 0:	 return L"phClr";
		case 1:	 return L"bg1";
		case 2:	 return L"tx1";
		case 3:	 return L"bg2";
		case 4:	 return L"tx2";
		case 5:	 return L"accent1";
		case 6:	 return L"accent2";
		case 7:	 return L"accent3";
		case 8:	 return L"accent4";
		case 9:	 return L"accent5";
		case 10: return L"accent6";
		case 11: return L"hlink";
		case 12: return L"folHlink";
		case 13: return L"lt1";
		case 14: return L"dk1";
		case 15: return L"lt2";
		case 16: return L"dk2";
		break;
		};
		return L"none";
	}

	AVSINLINE static std::wstring GetFontAlign(const WORD& value)
	{
		if (0 == value)			return L"base";
		if (1 == value)			return L"t";
		if (2 == value)			return L"ctr";
		if (3 == value)			return L"b";
		return L"auto";
	}


};
