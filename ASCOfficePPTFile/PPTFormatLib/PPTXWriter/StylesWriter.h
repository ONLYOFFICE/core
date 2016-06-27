/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

class CStylesWriter
{
public:
	AVSINLINE static CString GetTextAnchor(const WORD& value)
	{
		if (0 == value)
			return _T("t");
		if (1 == value)
			return _T("ctr");
		if (2 == value)
			return _T("b");
		return _T("t");
	}

	AVSINLINE static CString GetTextAlign(const WORD& value)
	{
		if (0 == value)
			return _T("l");
		if (1 == value)
			return _T("ctr");
		if (2 == value)
			return _T("r");
		if (3 == value)
			return _T("just");
		if (4 == value)
			return _T("dist");
		if (5 == value)
			return _T("thaiDist");
		if (6 == value)
			return _T("justLow");
		return _T("l");
	}
		//oArrayMem.push_back(oScheme[0]);//0
		//oArrayMem.push_back(oScheme[1]);//1
		//oArrayMem.push_back(oScheme[2]);//2
		//oArrayMem.push_back(oScheme[3]);//3
		//oArrayMem.push_back(oScheme[0]);//4
		//oArrayMem.push_back(oScheme[4]);//5 //accent1
		//oArrayMem.push_back(oScheme[5]);//6 //accent2
		//oArrayMem.push_back(oScheme[0]);//7 //accent3
		//oArrayMem.push_back(oScheme[5]);//8 //accent4
		//oArrayMem.push_back(oScheme[4]);//9 //accent5
		//oArrayMem.push_back(oScheme[7]);//10 //accent6
		//oArrayMem.push_back(oScheme[6]);//11 //hlink
		//oArrayMem.push_back(oScheme[7]);//12 //folHlink
		//oArrayMem.push_back(oScheme[0]);//13 //lt1
		//oArrayMem.push_back(oScheme[1]);//14 //dk1
		//oArrayMem.push_back(oScheme[2]);//15 //lt2
		//oArrayMem.push_back(oScheme[3]);//16 //dk2
	AVSINLINE static CString GetColorInScheme(const LONG& lIndex)
	{
		switch (lIndex)
		{
		case 0:	 return _T("phClr");
		case 1:	 return _T("bg1");
		case 2:	 return _T("tx1");
		case 3:	 return _T("bg2");
		case 4:	 return _T("tx2");
		case 5:	 return _T("accent1");
		case 6:	 return _T("accent2");
		case 7:	 return _T("accent3");
		case 8:	 return _T("accent4");
		case 9:	 return _T("accent5");
		case 10: return _T("accent6");
		case 11: return _T("hlink");
		case 12: return _T("folHlink");
		case 13: return _T("lt1");
		case 14: return _T("dk1");
		case 15: return _T("lt2");
		case 16: return _T("dk2");
		break;
		};
		return _T("none");
	}

	AVSINLINE static CString GetFontAlign(const WORD& value)
	{
		if (0 == value)
			return _T("base");
		if (1 == value)
			return _T("t");
		if (2 == value)
			return _T("ctr");
		if (3 == value)
			return _T("b");
		return _T("auto");
	}

	static CString ConvertStyles(NSPresentationEditor::CTextStyles& oStyles, NSPresentationEditor::CMetricInfo& oMetricInfo, int nCount = 10)
	{
		NSPresentationEditor::CStringWriter oWriter;
		ConvertStyles(oStyles, oMetricInfo, oWriter, nCount);
		return oWriter.GetData();
	}
	static void ConvertStyles(NSPresentationEditor::CTextStyles& oStyles, NSPresentationEditor::CMetricInfo& oMetricInfo, 
		NSPresentationEditor::CStringWriter& oWriter, int nCount = 10)
	{
		for (int i = 0; i < nCount; ++i)
		{
			if (oStyles.m_pLevels[i].is_init())
				ConvertStyleLevel(oStyles.m_pLevels[i].get(), oMetricInfo, oWriter, i);
		}
	}
	static CString ConvertStyleLevel(NSPresentationEditor::CTextStyleLevel& oLevel, NSPresentationEditor::CMetricInfo& oMetricInfo, const int& nLevel)
	{
		NSPresentationEditor::CStringWriter oWriter;
		ConvertStyleLevel(oLevel, oMetricInfo, oWriter, nLevel);
		return oWriter.GetData();
	}

	static void ConvertStyleLevel(NSPresentationEditor::CTextStyleLevel& oLevel, NSPresentationEditor::CMetricInfo& oMetricInfo,
		NSPresentationEditor::CStringWriter& oWriter, const int& nLevel);
};
