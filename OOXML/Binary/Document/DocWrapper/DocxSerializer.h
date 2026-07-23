/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "../../../DocxFormat/WritingElement.h"

namespace Writers
{
	class FileWriter;
}
namespace NSBinPptxRW
{
	class CBinaryFileReader;
	class CBinaryFileWriter;
}
namespace BinDocxRW
{
	class BinaryFileWriter;
	class ParamsWriter;

	class CDocxSerializer
	{
	private:
        std::wstring			m_sFontDir;
        std::wstring			m_sEmbeddedFontsDir;
		bool					m_bIsMacro;
		bool					m_bIsNoBase64Save;
		bool					m_bIsNoBase64;
		bool					m_bIsOForm;
	public:
		Writers::FileWriter*	m_pCurFileWriter;
		ParamsWriter*			m_pParamsWriter;
		
		CDocxSerializer();
		virtual ~CDocxSerializer();

        bool loadFromFile   (const std::wstring& sSrcFileName, const std::wstring& sDstPath, const std::wstring& sThemePath, const std::wstring& sMediaPath, const std::wstring& sEmbedPath);
        bool saveToFile     (const std::wstring& sSrcFileName, const std::wstring& sDstPath, const std::wstring& sTempPath);
		
		bool unpackageFile(const std::wstring& sSrcFileName, const std::wstring& sDstPath);
		bool convertFlat(const std::wstring& sSrcFileName, const std::wstring& sDstPath, bool& bMacro, const std::wstring& sTempPath);

        bool CreateDocxFolders(std::wstring strDirectory, std::wstring& sThemePath, std::wstring& sMediaPath, std::wstring& sEmbedPath);
		
        bool getXmlContent      (NSBinPptxRW::CBinaryFileReader& oBufferedStream, long lLength, std::wstring& sOutputXml);
        bool getBinaryContent   (const std::wstring& bsTxContent, NSBinPptxRW::CBinaryFileWriter& oBufferedStream, long& lDataSize);
		
        bool getBinaryContentElem   (OOX::EElementType eElemType, void* pElem, NSBinPptxRW::CBinaryFileWriter& oBufferedStream, long& lDataSize);
        bool getXmlContentElem      (OOX::EElementType eType, NSBinPptxRW::CBinaryFileReader& oBufferedStream, std::wstring& sOutputXml);

        void setFontDir         (const std::wstring& sFontDir);
        void setEmbeddedFontsDir(const std::wstring& sEmbeddedFontsDir);
        void setIsNoBase64Save  (bool val);
        void setIsNoBase64      (bool val);
        void setSaveChartAsImg  (bool val);
		void setOFormEnabled	(bool val);
		
		void setMacroEnabled	(bool val);
		bool getMacroEnabled();
	};
}
