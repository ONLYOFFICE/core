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
#include "../../SystemUtility/File.h"
#include "BinReaderWriterDefines.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../../Common/Base64.h"
#include "../../../DesktopEditor/common/Array.h"
#include "../../Base/Nullable.h"

#include <map>

namespace NSFontCutter
{
	class CEmbeddedFontsManager
	{		
	private:
		class CEmbeddedFontInfo
		{
		public:
			std::wstring	Name;

			std::wstring PathRegular;
			LONG	FaceRegular;

			std::wstring PathBold;
			LONG	FaceBold;

			std::wstring PathItalic;
			LONG	FaceItalic;

			std::wstring PathBoldItalic;
			LONG	FaceBoldItalic;

		public:
			CEmbeddedFontInfo();
			CEmbeddedFontInfo(const CEmbeddedFontInfo& oSrc);
			CEmbeddedFontInfo& operator=(const CEmbeddedFontInfo& oSrc);
		};

		std::map<std::wstring, CEmbeddedFontInfo> m_mapFontsEmbeddded;
        std::map<WCHAR, bool> m_CharMap;
		
	public:
		std::wstring m_strEmbeddedFontsFolder;

	public:
		CEmbeddedFontsManager();

		void CheckFont(const std::wstring& strName, NSFonts::IFontManager* pManager);
		void CheckString(const nullable_string& val);
		void CheckString(const std::wstring& val);

		template<typename T>
		void WriteEmbeddedFonts(T* pWriter)
		{
#if !defined(DONT_WRITE_EMBEDDED_FONTS)
			ULONG nCount = 0;

			for(std::map<std::wstring, CEmbeddedFontInfo>::iterator pPair = m_mapFontsEmbeddded.begin(); pPair != m_mapFontsEmbeddded.end(); ++pPair)
			{
				CEmbeddedFontInfo& oInfo = pPair->second;

				if (_T("") != oInfo.PathRegular)
					++nCount;

				if (_T("") != oInfo.PathBold)
					++nCount;

				if (_T("") != oInfo.PathItalic)
					++nCount;

				if (_T("") != oInfo.PathBoldItalic)
					++nCount;
			}

			pWriter->WriteULONG(nCount);

			std::wstring strFileWrite = m_strEmbeddedFontsFolder + _T("\\fonts.js");
			CFile oFile;
			oFile.CreateFile(strFileWrite);

			std::string strWrite = "var embedded_fonts = [\n";
			oFile.WriteFile(strWrite.GetBuffer(), strWrite.length());

			strWrite = ",\n";
			bool bIsFirst = true;
			ULONG lIndexF = 0;


            USHORT* pArrayUnicodes = NULL;
            size_t pArrayUnicodesLength = 0;
            bool bRes = GenerateSafearray(&pArrayUnicodes, pArrayUnicodesLength);
            if (!bRes)  // what about error handling?
                return;

			Fonts::IFontConverter* pFontConverter = NULL;
			CoCreateInstance(__uuidof(Fonts::CFontConverter), NULL, CLSCTX_ALL, __uuidof(Fonts::IFontConverter), (void**)&pFontConverter);

			for(std::map<std::wstring, CEmbeddedFontInfo>::iterator pPair = m_mapFontsEmbeddded.begin(); pPair != m_mapFontsEmbeddded.end(); ++pPair)
			{
				CEmbeddedFontInfo& oInfo = pPair->second;

				if (_T("") != oInfo.PathRegular)
				{
					if (!bIsFirst)
						oFile.WriteFile(strWrite.GetBuffer(), strWrite.length());
					
					bIsFirst = false;
                    WriteFont(oInfo.Name, oInfo.FaceRegular, oInfo.PathRegular, &oFile, pArrayUnicodes, pArrayUnicodesLength, pFontConverter);

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					
					pWriter->WriteBYTE(0);
					pWriter->WriteString(oInfo.Name);
					
					pWriter->WriteBYTE(1);
					pWriter->WriteULONG(0); // regular

					pWriter->WriteBYTE(2);
					pWriter->WriteBYTE(1); // is cutting

					pWriter->WriteBYTE(3);
					pWriter->WriteULONG(lIndexF); // index in cutting

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

					lIndexF++;
				}

				if (_T("") != oInfo.PathBold)
				{
					if (!bIsFirst)
						oFile.WriteFile(strWrite.GetBuffer(), strWrite.length());

					bIsFirst = false;
                    WriteFont(oInfo.Name, oInfo.FaceBold, oInfo.PathBold, &oFile, pArrayUnicodes, pArrayUnicodesLength, pFontConverter);

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					
					pWriter->WriteBYTE(0);
					pWriter->WriteString(oInfo.Name);
					
					pWriter->WriteBYTE(1);
					pWriter->WriteULONG(1); // bold

					pWriter->WriteBYTE(2);
					pWriter->WriteBYTE(1); // is cutting

					pWriter->WriteBYTE(3);
					pWriter->WriteULONG(lIndexF); // index in cutting

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

					lIndexF++;
				}

				if (_T("") != oInfo.PathItalic)
				{
					if (!bIsFirst)
						oFile.WriteFile(strWrite.GetBuffer(), strWrite.length());

					bIsFirst = false;
                    WriteFont(oInfo.Name, oInfo.FaceItalic, oInfo.PathItalic, &oFile, pArrayUnicodes, pArrayUnicodesLength, pFontConverter);

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					
					pWriter->WriteBYTE(0);
					pWriter->WriteString(oInfo.Name);
					
					pWriter->WriteBYTE(1);
					pWriter->WriteULONG(2); // italic

					pWriter->WriteBYTE(2);
					pWriter->WriteBYTE(1); // is cutting

					pWriter->WriteBYTE(3);
					pWriter->WriteULONG(lIndexF); // index in cutting

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

					lIndexF++;
				}

				if (_T("") != oInfo.PathBoldItalic)
				{
					if (!bIsFirst)
						oFile.WriteFile(strWrite.GetBuffer(), strWrite.length());

					bIsFirst = false;
                    WriteFont(oInfo.Name, oInfo.FaceBoldItalic, oInfo.PathBoldItalic, &oFile, pArrayUnicodes, pArrayUnicodesLength, pFontConverter);

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					
					pWriter->WriteBYTE(0);
					pWriter->WriteString(oInfo.Name);
					
					pWriter->WriteBYTE(1);
					pWriter->WriteULONG(3); // bold italic

					pWriter->WriteBYTE(2);
					pWriter->WriteBYTE(1); // is cutting

					pWriter->WriteBYTE(3);
					pWriter->WriteULONG(lIndexF); // index in cutting

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

					lIndexF++;
				}
			}

			strWrite = "\n];";
			oFile.WriteFile(strWrite.GetBuffer(), strWrite.length());

			oFile.CloseFile();

            delete [] pArrayUnicodes;
			RELEASEINTERFACE(pFontConverter);
#endif // #if !defined(DONT_WRITE_EMBEDDED_FONTS)
		}

		bool GenerateSafearray(USHORT **ppArray, size_t& nCount);

#if !defined(DONT_WRITE_EMBEDDED_FONTS)
		void WriteFont(std::wstring& strName, LONG& lFaceIndex, std::wstring& strFontPath, CFile* pFile, USHORT* pArrayUnicodes, size_t pArrayUnicodesLength, Fonts::IFontConverter* pFontConverter);
#endif // #if !defined(DONT_WRITE_EMBEDDED_FONTS)
	};
}

#define FONT_PICKER_BINARYDATA_SAVEEMBEDDED	0
#define FONT_PICKER_BINARYDATA_SAVEMAP		1
