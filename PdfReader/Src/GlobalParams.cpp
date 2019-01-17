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
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "MemoryUtils.h"
#include "StringExt.h"
#include "Constants.h"
#include "List.h"
#include "Hash.h"
#include "File.h"
#include "NameToCharCode.h"
#include "CharCodeToUnicode.h"
#include "UnicodeMap.h"
#include "CMap.h"
#include "BuiltinFontTables.h"
#include "EncodingTables.h"
#include "GlobalParams.h"
#include "NameToUnicodeTable.h"
#include "UnicodeMapTables.h"
#include "UTF8.h"

#include "../Resources/Fontn022003l.h"
#include "../Resources/Fontn022004l.h"
#include "../Resources/Fontn022024l.h"
#include "../Resources/Fontn022023l.h"
#include "../Resources/Fontn019003l.h"
#include "../Resources/Fontn019004l.h"
#include "../Resources/Fontn019024l.h"
#include "../Resources/Fontn019023l.h"
#include "../Resources/Fonts050000l.h"
#include "../Resources/Fontn021004l.h"
#include "../Resources/Fontn021024l.h"
#include "../Resources/Fontn021023l.h"
#include "../Resources/Fontn021003l.h"
#include "../Resources/Fontd050000l.h"

#define cidToUnicodeCacheSize     4
#define unicodeToUnicodeCacheSize 4

namespace PdfReader
{
	static struct
	{
		char*                sName;
		wchar_t*             wsT1FileName;
		wchar_t*             wsTTFileName;
		const unsigned char* pT1Buffer;
		const unsigned int   unSize;
	} c_arrBase14FontTable[] =
	{
		{ "Courier",               L"n022003l.pfb", L"cour.ttf",    c_arrn022003l, c_nSizen022003l},
		{ "Courier-Bold",          L"n022004l.pfb", L"courbd.ttf",  c_arrn022004l, c_nSizen022004l},
		{ "Courier-BoldOblique",   L"n022024l.pfb", L"courbi.ttf",  c_arrn022024l, c_nSizen022024l},
		{ "Courier-Oblique",       L"n022023l.pfb", L"couri.ttf",   c_arrn022023l, c_nSizen022023l},
		{ "Helvetica",             L"n019003l.pfb", L"arial.ttf",   c_arrn019003l, c_nSizen019003l},
		{ "Helvetica-Bold",        L"n019004l.pfb", L"arialbd.ttf", c_arrn019004l, c_nSizen019004l},
		{ "Helvetica-BoldOblique", L"n019024l.pfb", L"arialbi.ttf", c_arrn019024l, c_nSizen019024l},
		{ "Helvetica-Oblique",     L"n019023l.pfb", L"ariali.ttf",  c_arrn019023l, c_nSizen019023l},
		{ "Symbol",                L"s050000l.pfb", NULL,           c_arrs050000l, c_nSizes050000l},
		{ "Times-Bold",            L"n021004l.pfb", L"timesbd.ttf", c_arrn021004l, c_nSizen021004l},
		{ "Times-BoldItalic",      L"n021024l.pfb", L"timesbi.ttf", c_arrn021024l, c_nSizen021024l},
		{ "Times-Italic",          L"n021023l.pfb", L"timesi.ttf",  c_arrn021023l, c_nSizen021023l},
		{ "Times-Roman",           L"n021003l.pfb", L"times.ttf",   c_arrn021003l, c_nSizen021003l},
		{ "ZapfDingbats",          L"d050000l.pfb", NULL,           c_arrd050000l, c_nSized050000l},
		{ NULL, NULL, NULL, NULL, 0 }
	};
	//-------------------------------------------------------------------------------------------------------------------------------
	// GlobalParams
	//-------------------------------------------------------------------------------------------------------------------------------
	GlobalParams::GlobalParams() : m_pFontManager(NULL)
	{
		// Просматриваем кодировку в обратном порядке, чтобы присвоить символу наименьше возможный номер
		// (если у символа их несколько, например, как у 'space')
		m_pMacRomanReverseMap = new NameToCharCode();
		for (int nIndex = 255; nIndex >= 0; --nIndex)
		{
			if (c_arrMacRomanEncoding[nIndex])
			{
				m_pMacRomanReverseMap->Add(c_arrMacRomanEncoding[nIndex], (CharCode)nIndex);
			}
		}

		// Инициализируем талицу m_pNameToUnicode
		m_pNameToUnicode = new NameToCharCode();
		for (int nIndex = 0; c_arrNameToUnicodeTable[nIndex].sName; ++nIndex)
		{
			m_pNameToUnicode->Add(c_arrNameToUnicodeTable[nIndex].sName, c_arrNameToUnicodeTable[nIndex].nUnicode);
		}

		m_bMapNumericCharNames = true;
		m_bMapUnknownCharNames = false;
		m_pCMapCache           = new CMapCache();
		m_wsTempDirectory      = L"";
	}
	GlobalParams::~GlobalParams()
	{
		if (m_pMacRomanReverseMap)
			delete m_pMacRomanReverseMap;

		if (m_pNameToUnicode)
			delete m_pNameToUnicode;

		if (m_pCMapCache)
			delete m_pCMapCache;
	}
	bool               GlobalParams::GetMapNumericCharNames()
	{
		return m_bMapNumericCharNames;
	}
	bool               GlobalParams::GetMapUnknownCharNames()
	{
		return m_bMapUnknownCharNames;
	}
	void               GlobalParams::SetMapNumericCharNames(bool bMap)
	{
		m_bMapNumericCharNames = bMap;
	}
	void               GlobalParams::SetMapUnknownCharNames(bool bMap)
	{
		m_bMapUnknownCharNames = bMap;
	}
	CharCode           GlobalParams::GetMacRomanCharCode(char* sCharName)
	{
		return m_pMacRomanReverseMap->Lookup(sCharName);
	}
	CharCodeToUnicode* GlobalParams::GetUnicodeToUnicode(StringExt* seFontName)
	{
		// TODO: Как только будем хранить отдельную папку с файлами Unicode to Unicode реализовать тут
		return NULL;
	}
	CharCodeToUnicode* GlobalParams::GetCIDToUnicode(StringExt* seCollection)
	{
		// TODO: Как только будем хранить отдельную папку с файлами Cid to Unicode реализовать тут
		return NULL;
	}
	Unicode            GlobalParams::MapNameToUnicode(char* sCharName)
	{
		return m_pNameToUnicode->Lookup(sCharName);
	}
	CMap*              GlobalParams::GetCMap(StringExt* seCollection, StringExt* seCMapName, wchar_t* wsFilePath)
	{
		return m_pCMapCache->GetCMap(seCollection, seCMapName, this, wsFilePath);
	}
	FILE*              GlobalParams::FindToUnicodeFile(StringExt* seName)
	{
		// TODO: Как только будем хранить отдельную папку с файлами ToUnicode реализовать тут
		return NULL;
	}
	FILE*              GlobalParams::FindCMapFile(StringExt* seCollection, StringExt* seCMapName)
	{
		if (m_wsCMapDirectory != L"")
		{
			std::wstring wsFilePath = m_wsCMapDirectory + seCMapName->GetWString();
			return NSFile::CFileBinary::OpenFileNative(wsFilePath, L"rb");
		}

		return NULL;
	}
	FILE*              GlobalParams::GetUnicodeMapFile(StringExt* seEncodingName)
	{
		// Как только будем работать с папкой UnicodeMap реализовать тут
		return NULL;
	}
	std::wstring&      GlobalParams::GetTempFolder()
	{
		return m_wsTempDirectory;
	}
	void               GlobalParams::SetTempFolder(const wchar_t* wsTempFolder)
	{
		if (wsTempFolder)
		{
			m_wsTempDirectory = wsTempFolder;

			// В темповую директорию скидываем 14 стандартных шрифтов
			for (int nIndex = 0; nIndex < 14; nIndex++)
			{
				std::wstring wsFontPath;
				FILE* pFile = NULL;
				if (!NSFile::CFileBinary::OpenTempFile(&wsFontPath, &pFile, L"wb", L".base", (wchar_t*)wsTempFolder, NULL))
					continue;
				fclose(pFile);

				NSFile::CFileBinary oFile;
				oFile.CreateFileW(wsFontPath);
				oFile.WriteFile((BYTE*)c_arrBase14FontTable[nIndex].pT1Buffer, c_arrBase14FontTable[nIndex].unSize);
				oFile.CloseFile();
				m_arrBuiltinFontsPath[nIndex] = wsFontPath;
			}
		}
		else
			m_wsTempDirectory = L"";
	}
	std::wstring       GlobalParams::FindFontFile(StringExt* seFontName, wchar_t** pwsExts)
	{
		// TODO: Возможно стоит перенести сюда подбор шрифтов
		return L"";
	}
	void               GlobalParams::SetCMapFolder(const wchar_t* wsCMapDir)
	{
		if (wsCMapDir)
			m_wsCMapDirectory = wsCMapDir;
		else
			m_wsCMapDirectory = L"";
	}
    void               GlobalParams::SetFontManager(NSFonts::IFontManager* pFontManager)
	{
		m_pFontManager = pFontManager;
	}
    NSFonts::IFontManager*      GlobalParams::GetFontManager() const
	{
		return m_pFontManager;
	}
	std::wstring       GlobalParams::GetBuiltinFontPath(StringExt* seFontName) const
	{
		int nIndex = 0;
		for (nIndex = 0; nIndex < 14; nIndex++)
		{
			if (0 == seFontName->Compare(c_arrBase14FontTable[nIndex].sName))
				break;
		}

		if (nIndex < 14)
			return m_arrBuiltinFontsPath[nIndex];

		return L"";
	}
}
