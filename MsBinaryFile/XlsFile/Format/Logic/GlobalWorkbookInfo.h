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

#include <boost/unordered_map.hpp> 
#include <boost/smart_ptr/shared_array.hpp>
#include <vector>
#include <map>
#include <unordered_map>

#include "Biff_structures/BorderFillInfo.h"
#include "XlsElementsType.h"

#include "../Crypt/Decryptor.h"

#include "../../../../DesktopEditor/graphics/pro/Fonts.h"

class XlsConverter;

namespace PPTX
{
	class Theme;
	typedef boost::shared_ptr<Theme> ThemePtr;
}

namespace XLS
{

class BaseObject;
typedef boost::shared_ptr<BaseObject> BaseObjectPtr;

static const std::wstring DefaultPalette[] = {
	L"00000000",	L"00FFFFFF",	L"00FF0000",	L"0000FF00",	L"000000FF",	L"00FFFF00",	L"00FF00FF",	L"0000FFFF",	
	L"00000000",	L"00FFFFFF",	L"00FF0000",	L"0000FF00",	L"000000FF",	L"00FFFF00",	L"00FF00FF",	L"0000FFFF",	
	L"00800000",	L"00008000",	L"00000080",	L"00808000",	L"00800080",	L"00008080",	L"00C0C0C0",	L"00808080",	
	L"009999FF",	L"00993366",	L"00FFFFCC",	L"00CCFFFF",	L"00660066",	L"00FF8080",	L"000066CC",	L"00CCCCFF",	
	L"00000080",	L"00FF00FF",	L"00FFFF00",	L"0000FFFF",	L"00800080",	L"00800000",	L"00008080",	L"000000FF",	
	L"0000CCFF",	L"00CCFFFF",	L"00CCFFCC",	L"00FFFF99",	L"0099CCFF",	L"00FF99CC",	L"00CC99FF",	L"00FFCC99",	
	L"003366FF",	L"0033CCCC",	L"0099CC00",	L"00FFCC00",	L"00FF9900",	L"00FF6600",	L"00666699",	L"00969696",	
	L"00003366",	L"00339966",	L"00003300",	L"00333300",	L"00993300",	L"00993366",	L"00333399",	L"00333333"
};


struct _sx_name
{
	BaseObjectPtr				name;
	std::vector<BaseObjectPtr>	pair;
};

class GlobalWorkbookInfo
{
public:
	GlobalWorkbookInfo(const unsigned short code_page, XlsConverter * xls_converter_);
	~GlobalWorkbookInfo();

	const size_t	RegisterBorderId	(const BorderInfo& border);
	const size_t	RegisterFillId		(const FillInfo& fill);	
	const size_t	RegisterFontId		(const FontInfo& font);
	const int		RegistrDxfn			(const std::wstring& dx_style);

	void			RegisterPaletteColor(int id, const std::wstring & argb);
    
	void			GetDigitFontSizePixels();
	void			CalculateAnchor(int colL, int colR, int rowT, int rowB, _UINT32 & x, _UINT32 &y, _UINT32 &cx, _UINT32 & cy);


//-----------------------------------------------------------------------------
	bool									bVbaProjectExist;
	bool									bMacrosExist;
	bool									bWorkbookProtectExist;

	std::string								sTheme;

	int										lcid_user;
	unsigned short							CodePage;
	CRYPT::DecryptorPtr						decryptor;
	std::wstring							password;
	
	boost::unordered_map<BorderInfo, int>	border_x_ids;
	boost::unordered_map<FillInfo, int>		fill_x_ids;
	boost::unordered_map<FontInfo, int>		font_x_ids;

	std::map<int, int>						fonts_charsets;
	std::map<int,  std::wstring>			colors_palette;

	std::vector<BaseObjectPtr>				m_arNumFormats;
	std::vector<BaseObjectPtr>				m_arFonts;
	PPTX::ThemePtr							m_pTheme;
	
	_UINT32									current_sheet;

	_UINT32									last_Axes_id;
	_UINT32									last_Extern_id;

	std::map<std::wstring, BaseObjectPtr>	mapStrConnection;
	std::map<int, BaseObjectPtr>			mapIdConnection;

	short									idPivotCache;
	std::map<int, int>						mapPivotCacheIndex; //streamIdCache, write index order 
	std::map<int, BaseObjectPtr>			mapPivotCacheStream;//streamIdCache, object
	
	std::vector<BaseObjectPtr>				arPIVOTCACHEDEFINITION;

	std::vector<bool>						arPivotCacheFields;
	std::vector<bool>						arPivotCacheFieldShortSize;	

	std::vector<_sx_name>					arPivotSxNames;
	std::vector<std::wstring>				arPivotCacheSxNames;
	std::map<std::wstring, BaseObjectPtr>	mapPivotTableViews;


	std::unordered_map<std::wstring, std::wstring>		mapPivotCacheExternal;

	std::map<std::wstring, std::vector<std::wstring>>	mapDefineNames;
	std::vector<std::wstring>							arDefineNames;
	std::map<std::wstring, int>							mapDefineNamesSerialized;
	
	std::vector<std::pair<boost::shared_array<unsigned char>, size_t> >	bin_data;
	std::pair<boost::shared_array<unsigned char>, size_t>				listdata_data;
	std::pair<boost::shared_array<unsigned char>, size_t>				controls_data;

	struct _xti
	{
		int							iSup;
        _INT32                      itabFirst; //firstSheet in biff12
        _INT32                      itabLast; //lastSheet in biff12
		std::wstring				link;
		std::vector<std::wstring>*	pNames = NULL;
	};
	std::vector<_xti>				arXti;
	std::vector<_xti>				arXti_External;
	
	_UINT32							startAddedSharedStrings;
	std::vector<std::wstring>		arAddedSharedStrings;
	
	struct _row_info
	{
		BaseObjectPtr row_info;
		std::map<int, BaseObjectPtr> mapCells;
	};

	struct _sheet_info
	{
		std::wstring				state = L"visible";
		std::wstring				name;

		std::map<int, double>		customColumnsWidth;
		std::map<int, double>		customRowsHeight;
		
		double						defaultColumnWidth = 8.0;
		double						defaultRowHeight = 14.4;

		std::map<int, _row_info>	mapRows;
	};
	std::vector<_sheet_info>		sheets_info;
	std::vector<std::wstring>		external_sheets_info; //current

	std::pair<float, float>			defaultDigitFontSize;
	NSFonts::IApplicationFonts*     applicationFonts;
	std::wstring					fontsDirectory;

	std::wstring					tempDirectory;

	int								Version;

	int								cmt_rules;
	int								cellXfs_count;
	int								cellStyleXfs_count;
	int								cellStyleDxfs_count;

	std::map<std::wstring, int>		mapDefaultFormatCode;
	std::map<_UINT16, bool>			mapUsedFormatCode;

	std::map<std::wstring, int>		mapUserDxfs;
	std::vector<std::wstring>		arrUserDxfs;

	std::wstringstream				connections_stream;
	
	int								connectionId;
	std::map<std::wstring, int>		connectionNames;

    std::unordered_map<int, std::wstring>		mapTableNames;
    std::unordered_map<int, std::vector<std::wstring>>	mapTableColumnNames;
    std::unordered_map<std::wstring, int>		mapTableGuidsIndex;

    std::unordered_map<int, std::vector<XLS::ElementType>>		pivotCacheRecordType;
    int                     		currentPivotCacheRecord;

	XlsConverter					*xls_converter;
};

typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;


} // namespace XLS
