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

        bool loadFromFile   (const std::wstring& sSrcFileName, const std::wstring& sDstPath, const std::wstring& sXMLOptions, const std::wstring& sThemePath, const std::wstring& sMediaPath, const std::wstring& sEmbedPath);
        bool saveToFile     (const std::wstring& sSrcFileName, const std::wstring& sDstPath, const std::wstring& sXMLOptions, const std::wstring& sTempPath);
		
		bool unpackageFile(const std::wstring& sSrcFileName, const std::wstring& sDstPath);
		bool convertFlat(const std::wstring& sSrcFileName, const std::wstring& sDstPath);

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
		void setMacroEnabled	(bool val);
		void setOFormEnabled	(bool val);
	};
}
