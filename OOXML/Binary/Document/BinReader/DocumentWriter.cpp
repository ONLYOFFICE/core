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

#include "DocumentWriter.h"

namespace Writers
{
	DocumentWriter::DocumentWriter(std::wstring sDir, HeaderFooterWriter& oHeaderFooterWriter) : m_sDir(sDir), m_oHeaderFooterWriter(oHeaderFooterWriter)
	{
	}

	void DocumentWriter::Write(bool bGlossary)
	{
		if (bGlossary)
		{
			OOX::CPath filePath = m_sDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"glossary" + FILE_SEPARATOR_STR + L"document.xml";

			NSFile::CFileBinary oFile;
			oFile.CreateFileW(filePath.GetPath());
			oFile.WriteStringUTF8(std::wstring(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
			oFile.WriteStringUTF8(std::wstring(L"<w:glossaryDocument \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\" \
xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\" \
xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\" \
xmlns:w16sdtdh=\"http://schemas.microsoft.com/office/word/2020/wordml/sdtdatahash\" \
xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" \
mc:Ignorable=\"w14 w15 w16se w16cid w16 w16cex w16sdtdh wp14\">"));

			oFile.WriteStringUTF8(m_oBackground.GetData());

			oFile.WriteStringUTF8(m_oContent.GetData());

			oFile.WriteStringUTF8(std::wstring(L"</w:glossaryDocument>"));
			oFile.CloseFile();
		}
		else
		{
			OOX::CPath filePath = m_sDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"document.xml";

			NSFile::CFileBinary oFile;
			oFile.CreateFileW(filePath.GetPath());
			oFile.WriteStringUTF8(std::wstring(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
			oFile.WriteStringUTF8(std::wstring(L"<w:document \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\" \
xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\" \
xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\" \
xmlns:w16sdtdh=\"http://schemas.microsoft.com/office/word/2020/wordml/sdtdatahash\" \
xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" \
mc:Ignorable=\"w14 w15 w16se w16cid w16 w16cex w16sdtdh wp14\">"));

			oFile.WriteStringUTF8(m_oBackground.GetData());

			oFile.WriteStringUTF8(std::wstring(L"<w:body>"));
            //oFile.WriteStringUTF8(m_oContent.GetData());
            std::wstring content = m_oContent.GetData();
            const wchar_t* data = content.c_str();
            size_t totalLength = content.length();
            const size_t CHUNK_SIZE = 65536;

            for (size_t i = 0; i < totalLength; i += CHUNK_SIZE)
            {
                size_t end = std::min(i + CHUNK_SIZE, totalLength);
                size_t chunk_length = end - i;
                std::wstring chunk(data + i, chunk_length);
                oFile.WriteStringUTF8(chunk);
            }

            m_oContent.Clear();
			//oFile.WriteStringUTF8(WriteSectPrHdrFtr());
			oFile.WriteStringUTF8(m_oSecPr.GetData());

			oFile.WriteStringUTF8(std::wstring(L"</w:body>"));

			oFile.WriteStringUTF8(std::wstring(L"</w:document>"));
			oFile.CloseFile();
		}
	}

	std::wstring DocumentWriter::WriteSectPrHdrFtr()
	{
		 std::wstring sResult;
		if(BinDocxRW::g_nCurFormatVersion < 5)
		{
			bool bTitlePage = false;
			for(size_t i = 0, length = m_oHeaderFooterWriter.m_aHeaders.size(); i < length; ++i)
			{
				HdrFtrItem* pHeader = m_oHeaderFooterWriter.m_aHeaders[i];
				if(false == pHeader->rId.empty())
				{
					if(SimpleTypes::hdrftrFirst == pHeader->eType)
					{
						sResult += L"<w:headerReference w:type=\"first\" r:id=\"" + pHeader->rId + L"\"/>";
						bTitlePage = true;
					}
					else if(SimpleTypes::hdrftrEven == pHeader->eType)
					{
						sResult += L"<w:headerReference w:type=\"even\" r:id=\"" + pHeader->rId + L"\"/>";
					}
					else
					{
						sResult += L"<w:headerReference w:type=\"default\" r:id=\"" + pHeader->rId + L"\"/>";
					}
				}
			}
			for(size_t i = 0, length = m_oHeaderFooterWriter.m_aFooters.size(); i < length; ++i)
			{
				HdrFtrItem* pFooter = m_oHeaderFooterWriter.m_aFooters[i];
				if(false == pFooter->rId.empty())
				{
					if(SimpleTypes::hdrftrFirst == pFooter->eType)
					{
						sResult += L"<w:footerReference w:type=\"first\" r:id=\"" + pFooter->rId + L"\"/>";
						bTitlePage = true;
					}
					else if(SimpleTypes::hdrftrEven == pFooter->eType)
					{
						sResult += L"<w:footerReference w:type=\"even\" r:id=\"" + pFooter->rId + L"\"/>";
					}
					else
					{
						sResult += L"<w:footerReference w:type=\"default\" r:id=\"" + pFooter->rId + L"\"/>";
					}
				}
			}
			if(true == bTitlePage)
				sResult += L"<w:titlePg/>";
		}
		return sResult;
	}
}
