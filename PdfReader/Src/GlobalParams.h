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
#ifndef _PDF_READER_GLOBAL_PARAMS_H
#define _PDF_READER_GLOBAL_PARAMS_H

#include <stdio.h>
#include <string>

#include "CharTypes.h"

#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"

namespace PdfReader
{
	class StringExt;
	class CList;
	class CHash;
	class NameToCharCode;
	class CharCodeToUnicode;
	class CharCodeToUnicodeCache;
	class UnicodeMap;
	class UnicodeMapCache;
	class CMap;
	class CMapCache;
	class GlobalParams;
	//-------------------------------------------------------------------------------------------------------------------------------
	// GlobalParams
	//-------------------------------------------------------------------------------------------------------------------------------
	class GlobalParams
	{
	public:

		GlobalParams();
		~GlobalParams();

		bool               GetMapNumericCharNames();
		bool               GetMapUnknownCharNames();
		void               SetMapNumericCharNames(bool bMap);
		void               SetMapUnknownCharNames(bool bMap);
		CharCode           GetMacRomanCharCode(char* sCharName);
		CharCodeToUnicode* GetUnicodeToUnicode(StringExt* seFontName);
		CharCodeToUnicode* GetCIDToUnicode(StringExt* seCollection);
		Unicode            MapNameToUnicode(char* sCharName);
		CMap*              GetCMap(StringExt* seCollection, StringExt* seCMapName, wchar_t* wsFilePath = NULL);
		FILE*              FindToUnicodeFile(StringExt* seName);
		FILE*              FindCMapFile(StringExt *seCollection, StringExt *seCMapName);
		FILE*              GetUnicodeMapFile(StringExt* seEncodingName);
		std::wstring&      GetTempFolder();
		void               SetTempFolder(const wchar_t* wsTempFolder);
		std::wstring       FindFontFile(StringExt* seFontName, wchar_t** pwsExts);
		void               SetCMapFolder(const wchar_t* wsDir);
        void               SetFontManager(NSFonts::IFontManager* pFontManager);
        NSFonts::IFontManager*      GetFontManager() const;
		std::wstring       GetBuiltinFontPath(StringExt* seFontName) const;

	private:

		bool                    m_bMapNumericCharNames;  // Map numeric char names (from font subsets)?
		bool                    m_bMapUnknownCharNames;  // Map unknown char names? 
						        
		NameToCharCode*         m_pMacRomanReverseMap;   // Char name -> MacRomanEncoding 
		NameToCharCode*         m_pNameToUnicode;        // Char name -> Unicode 

		CMapCache*              m_pCMapCache;
		std::wstring            m_wsCMapDirectory;       // Путь к папке с CMap файлами
		std::wstring            m_wsTempDirectory;

        NSFonts::IFontManager*  m_pFontManager;
		std::wstring            m_arrBuiltinFontsPath[14]; // Пути к стандартным 14 шрифтам
	};
}

#endif // _PDF_READER_GLOBAL_PARAMS_H
