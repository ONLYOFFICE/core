#ifndef _PDF_READER_GLOBAL_PARAMS_H
#define _PDF_READER_GLOBAL_PARAMS_H

#include <stdio.h>
#include <string>

#include "CharTypes.h"

#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../DesktopEditor/fontengine/FontManager.h"


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
		void               SetFontManager(CFontManager* pFontManager);
		CFontManager*      GetFontManager() const;
		std::wstring       GetBuiltinFontPath(StringExt* seFontName) const;

	private:

		bool                    m_bMapNumericCharNames;  // Map numeric char names (from font subsets)?
		bool                    m_bMapUnknownCharNames;  // Map unknown char names? 
						        
		NameToCharCode*         m_pMacRomanReverseMap;   // Char name -> MacRomanEncoding 
		NameToCharCode*         m_pNameToUnicode;        // Char name -> Unicode 

		CMapCache*              m_pCMapCache;
		std::wstring            m_wsCMapDirectory;       // Путь к папке с CMap файлами
		std::wstring            m_wsTempDirectory;

		CFontManager*           m_pFontManager;
		std::wstring            m_arrBuiltinFontsPath[14]; // Пути к стандартным 14 шрифтам
	};
}

#endif // _PDF_READER_GLOBAL_PARAMS_H
