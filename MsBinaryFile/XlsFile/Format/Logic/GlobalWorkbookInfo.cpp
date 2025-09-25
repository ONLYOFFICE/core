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
#include "GlobalWorkbookInfo.h"

#include "Biff_records/Font.h"
#include "Biff_records/Format.h"

#include "../../../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../../../OOXML/Base/Unit.h"

namespace XLS
{

std::pair<float, float> GetMaxDigitSizePixelsImpl(const std::wstring & fontName, double fontSize, double dpi, long fontStyle, NSFonts::IFontManager *pFontManager)
{
	if (pFontManager == NULL) return std::pair<float, float>(7,8);

	int hr = FALSE;

    if (FALSE == (hr = pFontManager->LoadFontByName(fontName, fontSize, fontStyle, dpi, dpi )))
	{
        if (FALSE == (hr = pFontManager->LoadFontByName(L"Arial", fontSize, fontStyle, dpi, dpi )))
		{
            return std::pair<float, float>(7,8);
		}
	}

    float maxWidth = 0;
    float maxHeight = 0;

	float minWidth = 0xffff;
	float minHeight = 0xffff;

    //for (int i = 0; i <= 9; ++i)
    {
		//if (FALSE == (hr = pFontManager->LoadString2( std::to_wstring(i), 0, 0)))
		//	return std::pair<float, float>(7,8);

		if (FALSE == (hr = pFontManager->LoadString2( L"xxxxx", 0, 0)))
			return std::pair<float, float>(7,8);      
		 
		TBBox box;
		try
		{
           box = pFontManager->MeasureString();
		}catch(...)
		{
            return std::pair<float, float>(7,8);
		}

		if (box.fMaxX < -0xffff+1 || box.fMaxY < -0xffff+1 ||
			box.fMinX > 0xffff-1 || box.fMinY > 0xffff-1)		
				return std::pair<float, float>(7,8);
          
        if (box.fMaxX - box.fMinX > maxWidth)   maxWidth = box.fMaxX - box.fMinX;
        if (box.fMaxY - box.fMinY > maxHeight)  maxHeight = box.fMaxY - box.fMinY;
       
		if (box.fMaxX - box.fMinX < minWidth)   minWidth = box.fMaxX - box.fMinX;
        if (box.fMaxY - box.fMinY < minHeight)  minHeight = box.fMaxY - box.fMinY;
    }

	double width = (minWidth + 2 * maxWidth) /3. /5.;
    return std::pair<float, float>(width, maxHeight);
}
std::vector<GlobalWorkbookInfo::_xti>				GlobalWorkbookInfo::arXti_External_static;
std::unordered_map<int, std::wstring>				GlobalWorkbookInfo::mapTableNames_static;
std::unordered_map<int, std::vector<std::wstring>>	GlobalWorkbookInfo::mapTableColumnNames_static;
std::vector<std::wstring>							GlobalWorkbookInfo::arDefineNames_static;
std::unordered_map<int, std::vector<int>>			GlobalWorkbookInfo::mapXtiTables_static;

GlobalWorkbookInfo::GlobalWorkbookInfo(const unsigned short code_page, XlsConverter * converter) :	CodePage(code_page), xls_converter(converter)
{
	fill_x_ids[FillInfo(0, 0, 0)]		= 0;
	fill_x_ids[FillInfo(17, 64, 65)]	= 1;
	
	last_Axes_id			= 0x2000000;
	last_Extern_id			= 1;
	last_User_NumFmt		= 165;
	lcid_user				= -1;

	Version					= 0x0600; 

	startAddedSharedStrings = 0;
	current_sheet			= 0;
	cmt_rules				= 0;

	cellXfs_count			= 0;
	cellStyleXfs_count		= 0;
	cellStyleDxfs_count		= 0;

	connectionId			= 0;

	defaultDigitFontSize = std::pair<float, float>(0, 0);
	applicationFonts		= NULL;

	bVbaProjectExist		= false;
	bMacrosExist			= false;
	bWorkbookProtectExist	= false;

	idPivotCache = 0;	
	currentPivotCacheRecord = 0;

// common for all lcid
	mapDefaultFormatCode.insert(std::make_pair(L"0", 1));
	mapDefaultFormatCode.insert(std::make_pair(L"0.00", 2));
	mapDefaultFormatCode.insert(std::make_pair(L"#,##0", 3));
	mapDefaultFormatCode.insert(std::make_pair(L"#,##0.00", 4));
	mapDefaultFormatCode.insert(std::make_pair(L"0%", 9));
	mapDefaultFormatCode.insert(std::make_pair(L"0.00%", 10));
	mapDefaultFormatCode.insert(std::make_pair(L"0.00E+00", 11));
	mapDefaultFormatCode.insert(std::make_pair(L"# ?/?", 12));
	mapDefaultFormatCode.insert(std::make_pair(L"# ??/??", 13));
	mapDefaultFormatCode.insert(std::make_pair(L"mm-dd-yy", 14));
	mapDefaultFormatCode.insert(std::make_pair(L"d-mmm-yy", 15));
	mapDefaultFormatCode.insert(std::make_pair(L"d-mmm", 16));
	mapDefaultFormatCode.insert(std::make_pair(L"mmm-yy", 17));
	mapDefaultFormatCode.insert(std::make_pair(L"h:mm AM/PM", 18));
	mapDefaultFormatCode.insert(std::make_pair(L"h:mm:ss AM/PM", 19));
	mapDefaultFormatCode.insert(std::make_pair(L"h:mm", 20));
	mapDefaultFormatCode.insert(std::make_pair(L"h:mm:ss", 21));
	mapDefaultFormatCode.insert(std::make_pair(L"m/d/yy h:mm", 22));
	mapDefaultFormatCode.insert(std::make_pair(L"#,##0 ;(#,##0)", 37));
	mapDefaultFormatCode.insert(std::make_pair(L"#,##0 ;[Red](#,##0)", 38));
	mapDefaultFormatCode.insert(std::make_pair(L"#,##0.00;(#,##0.00)", 39));
	mapDefaultFormatCode.insert(std::make_pair(L"#,##0.00;[Red](#,##0.00)", 40));
	mapDefaultFormatCode.insert(std::make_pair(L"mm:ss", 45));
	mapDefaultFormatCode.insert(std::make_pair(L"[h]:mm:ss", 46));
	mapDefaultFormatCode.insert(std::make_pair(L"mmss.0", 47));
	mapDefaultFormatCode.insert(std::make_pair(L"##0.0E+0", 48));
	mapDefaultFormatCode.insert(std::make_pair(L"@", 49));
	mapDefaultFormatCode.insert(std::make_pair(L"General", 0));

	mapDefaultFormatCodeNum.insert(std::make_pair(0, L"General"));
	mapDefaultFormatCodeNum.insert(std::make_pair(1, L"0"));
	mapDefaultFormatCodeNum.insert(std::make_pair(2, L"0.00"));
	mapDefaultFormatCodeNum.insert(std::make_pair(3, L"#,##0"));
	mapDefaultFormatCodeNum.insert(std::make_pair(4, L"#,##0.00"));
	mapDefaultFormatCodeNum.insert(std::make_pair(9, L"0%"));
	mapDefaultFormatCodeNum.insert(std::make_pair(10,L"0.00%"));
	mapDefaultFormatCodeNum.insert(std::make_pair(11, L"0.00E+00"));
	mapDefaultFormatCodeNum.insert(std::make_pair(12, L"# ?/?"));
	mapDefaultFormatCodeNum.insert(std::make_pair(13, L"# ??/??"));
	mapDefaultFormatCodeNum.insert(std::make_pair(14, L"mm-dd-yy"));
	mapDefaultFormatCodeNum.insert(std::make_pair(15, L"d-mmm-yy"));
	mapDefaultFormatCodeNum.insert(std::make_pair(16, L"d-mmm"));
	mapDefaultFormatCodeNum.insert(std::make_pair(17, L"mmm-yy"));
	mapDefaultFormatCodeNum.insert(std::make_pair(18, L"h:mm AM/PM"));
	mapDefaultFormatCodeNum.insert(std::make_pair(19, L"h:mm:ss AM/PM"));
	mapDefaultFormatCodeNum.insert(std::make_pair(20, L"h:mm"));
	mapDefaultFormatCodeNum.insert(std::make_pair(21, L"h:mm:ss"));
	mapDefaultFormatCodeNum.insert(std::make_pair(22, L"m/d/yy h:mm"));
	mapDefaultFormatCodeNum.insert(std::make_pair(37, L"#,##0 ;(#,##0)"));
	mapDefaultFormatCodeNum.insert(std::make_pair(38, L"#,##0 ;[Red](#,##0)"));
	mapDefaultFormatCodeNum.insert(std::make_pair(39, L"#,##0.00;(#,##0.00)"));
	mapDefaultFormatCodeNum.insert(std::make_pair(40, L"#,##0.00;[Red](#,##0.00)"));
	mapDefaultFormatCodeNum.insert(std::make_pair(45, L"mm:ss"));
	mapDefaultFormatCodeNum.insert(std::make_pair(46, L"[h]:mm:ss"));
	mapDefaultFormatCodeNum.insert(std::make_pair(47, L"mmss.0"));
	mapDefaultFormatCodeNum.insert(std::make_pair(48, L"##0.0E+0"));
	mapDefaultFormatCodeNum.insert(std::make_pair(49, L"@"));
}

GlobalWorkbookInfo::~GlobalWorkbookInfo()
{
    if (applicationFonts)
        delete applicationFonts;

	arXti_External_static.clear();
	mapTableNames_static.clear();
	mapTableColumnNames_static.clear();
	arDefineNames_static.clear();
}

const size_t GlobalWorkbookInfo::RegisterBorderId(const BorderInfo& border)
{
	boost::unordered_map<BorderInfo, int>::const_iterator it_find = border_x_ids.find(border);
	if(border_x_ids.end() == it_find)
	{
		int id = border_x_ids.size();
		border_x_ids[border] = id;
		return id;
	}
	else
	{
		return it_find->second;
	}
}
const size_t GlobalWorkbookInfo::RegisterFontId(const FontInfo& font)
{
	boost::unordered_map<FontInfo, int>::const_iterator it_find = font_x_ids.find(font);
	
	if(font_x_ids.end() == it_find)
	{
		int id = font_x_ids.size();
		font_x_ids[font] = id;
		return id;
	}
	else
	{
		return it_find->second;
	}
}
const size_t GlobalWorkbookInfo::RegisterFillId(const FillInfo& fill)
{
	if(0 == fill.fls)
	{
		return 0;
	}

	boost::unordered_map<FillInfo, int>::const_iterator it_find = fill_x_ids.find(fill);
	if(fill_x_ids.end() == it_find)
	{
		int id = fill_x_ids.size();
		fill_x_ids[fill] = id;
		return id;
	}
	else
	{
		return it_find->second;
	}
}

void GlobalWorkbookInfo::RegisterPaletteColor(int id, const std::wstring & rgb)
{
	colors_palette.insert(std::make_pair(id, rgb));
}
void GlobalWorkbookInfo::RegisterNumFormat(BaseObjectPtr element)
{
	Format* fmt = dynamic_cast<Format*>(element.get());
	if (!fmt) return;

	if (fmt->ifmt == 0xffff)
	{
		std::map<std::wstring, _UINT16>::iterator pFind = mapDefaultFormatCode.find(fmt->stFormat);
		if (pFind != mapDefaultFormatCode.end())
		{
			fmt->ifmt_used = fmt->ifmt = pFind->second;
		}
		else
		{
			fmt->ifmt_used = fmt->ifmt = last_User_NumFmt++;
		}
	}
	else
	{
		std::map<_UINT16, _UINT16>::iterator pFindCode = mapUsedFormatCode.find(fmt->ifmt);
		if (pFindCode != mapUsedFormatCode.end())
		{
			fmt->ifmt_used = pFindCode->second;
		}
		else
		{
			if (fmt->ifmt > 49)
			{
				fmt->ifmt_used = last_User_NumFmt++;
			}
			else
			{
				fmt->ifmt_used = fmt->ifmt;
			}
			mapUsedFormatCode.insert(std::make_pair(fmt->ifmt, fmt->ifmt_used));
		}
	}
	std::map<_UINT16, BaseObjectPtr>::iterator pFindFormat = m_mapNumFormats.find(fmt->ifmt_used);
	if (pFindFormat == m_mapNumFormats.end())
	{
		m_mapNumFormats.insert(std::make_pair(fmt->ifmt_used, element));
	}
	else
	{
		//меняем
		pFindFormat->second = element;
	}
}
_UINT16 GlobalWorkbookInfo::RegisterNumFormat(_UINT16 ifmt, const std::wstring & format_code_)
{
	std::wstring format_code = format_code_;

	std::map<_UINT16, _UINT16>::iterator pFind = mapUsedFormatCode.find(ifmt);
	if (pFind != mapUsedFormatCode.end())
	{
		return pFind->second;
	}
	else
	{
		if (format_code.empty())
		{
			if (59 <= ifmt && ifmt <= 78)
			{
				if (69 <= ifmt && ifmt <= 71)
				{
					ifmt += 1;
				}
				ifmt -= 58;
			}
			else if (79 <= ifmt && ifmt <= 81)
			{
				ifmt -= 34;
			}
			switch (ifmt)
			{
			case 23:
			case 24:
			case 25:
			case 26:
				ifmt = 0;
				break;
			case 27:
			case 28:
			case 29:
			case 30:
			case 31:
			case 36:
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:
			case 55:
			case 56:
			case 57:
			case 58:
				ifmt = 14;
				break;
			case 32:
			case 33:
			case 34:
			case 35:
				ifmt = 21;
				break;
			}

			std::map<_UINT16, std::wstring>::iterator pFindCode = mapDefaultFormatCodeNum.find(ifmt);
			if (pFindCode != mapDefaultFormatCodeNum.end())
			{
				format_code = pFindCode->second;
			}
			else
			{
				// ???? todooo 
			}
		}
		
		_UINT16 ifmt_used = ifmt;
		if (ifmt > 49)
		{
			ifmt_used = last_User_NumFmt++; 
		}

		std::map<_UINT16, BaseObjectPtr>::iterator pFindFormat = m_mapNumFormats.find(ifmt_used);
		if (pFindFormat == m_mapNumFormats.end())
		{
			// генерим хоть что то
			Format* fmt = new Format();
			fmt->ifmt = ifmt;
			fmt->ifmt_used = ifmt_used;

			fmt->stFormat = XmlUtils::EncodeXmlString(format_code, true);

			m_mapNumFormats.insert(std::make_pair(fmt->ifmt_used, BaseObjectPtr(fmt)));
		}
		mapUsedFormatCode.insert(std::make_pair(ifmt, ifmt_used));
	
		return ifmt_used;
	}
}
const int GlobalWorkbookInfo::RegistrDxfn(const std::wstring & dxfn)
{
	if (dxfn.empty() == true) return -1;

	std::map<std::wstring, int>::iterator pFind = mapUserDxfs.find(dxfn);

	if (pFind == mapUserDxfs.end())
	{
		int id = cellStyleDxfs_count++;
		mapUserDxfs.insert(std::make_pair(dxfn, id));
		arrUserDxfs.push_back(dxfn);
		return id;
	}
	else
	{
		return pFind->second;
	}
}

void GlobalWorkbookInfo::GetDigitFontSizePixels()
{
	if (defaultDigitFontSize.first > 0.01) return;

	if (applicationFonts == NULL)
	{
        applicationFonts = NSFonts::NSApplication::Create();
		applicationFonts->InitializeFromFolder(fontsDirectory);
	}

	defaultDigitFontSize = std::pair<float, float>(7,8);
	if (m_arFonts.empty()) return;

	Font * font = dynamic_cast<Font*>(m_arFonts[0].get());
	if (!font) return;

    std::wstring	fontName = font->fontName;
	double			fontSize = font->dyHeight /20.;

    try 
    {
        if (applicationFonts)
        {
            NSFonts::IFontManager *pFontManager = applicationFonts->GenerateFontManager();

            std::pair<float, float> val = GetMaxDigitSizePixelsImpl(fontName, fontSize, 96., 0, pFontManager);

            if (pFontManager)
            {
                delete pFontManager;
            }

            defaultDigitFontSize = val;
        }
    }
    catch(...)
    {
    }    
}

void GlobalWorkbookInfo::CalculateAnchor(int colL, int colR, int rwT, int rwB, _UINT32 & x, _UINT32 &y, _UINT32 &cx, _UINT32 & cy)
{
	_sheet_info zero;
	_sheet_info & sheet_info = current_sheet >= 0 ? sheets_info[current_sheet - 1] : zero;

	GetDigitFontSizePixels();

//----------------------------------------------------------------------------------------------------
	x = y = cx = cy = 0;
	//1 inch	=	72 point
	//1 emu		=	360000 * 2.54 inch

	double kfCol	= 1;// 1250.;//360000 / 72. / 4.;
	double kfRow	= ( 360000 * 2.54 / 72) / 256. ;

	double Digit_Width	= defaultDigitFontSize.first;
	double Digit_Height = defaultDigitFontSize.second;

	double width = 0, column_width = 0;

	for (int i = 0 ; i < colL; i++)
	{
		if (sheet_info.customColumnsWidth.find(i) != sheet_info.customColumnsWidth.end())
			x +=  256 * kfCol * sheet_info.customColumnsWidth[i];	
		else 
			x +=  256 * kfCol * sheet_info.defaultColumnWidth;
	}

	for (int i = colL ; i < colR; i++)
	{
		if (sheet_info.customColumnsWidth.find(i) != sheet_info.customColumnsWidth.end())
			cx += 256 * kfCol * sheet_info.customColumnsWidth[i];	
		else 
			cx += 256 * kfCol * sheet_info.defaultColumnWidth;
	}

	for (int i = 0 ; i < rwT; i++)
	{
		if (sheet_info.customRowsHeight.find(i) != sheet_info.customRowsHeight.end())
		{
			y += 256 * kfRow * sheet_info.customRowsHeight[i];	
		}
		else 
			y += 256 * kfRow * sheet_info.defaultRowHeight;	
	}

	for (int i = rwT ; i < rwB; i++)
	{
		if (sheet_info.customRowsHeight.find(i) != sheet_info.customRowsHeight.end())
		{
			cy += 256 * kfRow * sheet_info.customRowsHeight[i];	
		}
		else 
			cy += 256 * kfRow * sheet_info.defaultRowHeight;	
	}
	cx = ((int)((cx * Digit_Width + 5) / Digit_Width * 256 )) / 256.;
	cx = (int)(((256. * cx + ((int)(128. / Digit_Width ))) / 256. ) * Digit_Width ); //in pixels
	
	x = ((int)((x * Digit_Width + 5) / Digit_Width * 256 )) / 256.;
	x = (int)(((256. * x + ((int)(128. / Digit_Width ))) / 256. ) * Digit_Width ); //in pixels

}


} // namespace XLS
