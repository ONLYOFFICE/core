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
#include "ConvertTxt2Docx.h"
#include "TxtFormat/File.h"

#include "../../OOXML/DocxFormat/Document.h"
#include "../../OOXML/DocxFormat/Styles.h"
#include "../../OOXML/DocxFormat/Footnote.h"
#include "../../OOXML/DocxFormat/Endnote.h"
#include "../../OOXML/DocxFormat/HeaderFooter.h"
#include "../../OOXML/DocxFormat/Logic/AlternateContent.h"
#include "../../OOXML/DocxFormat/Logic/Annotations.h"
#include "../../OOXML/DocxFormat/Logic/Sdt.h"
#include "../../OOXML/DocxFormat/Logic/Table.h"
#include "../../OOXML/DocxFormat/Logic/Paragraph.h"
#include "../../OOXML/DocxFormat/Logic/Run.h"

namespace Txt2Docx
{
    namespace
    {
        bool isEmoji(const std::string& utf8Char)
        {
            if (utf8Char.empty()) return false;

            unsigned char c = utf8Char[0];
            uint32_t codePoint = 0;

            if ((c & 0xF8) == 0xF0 && utf8Char.length() >= 4)
            {
                codePoint = ((utf8Char[0] & 0x07) << 18) |
                            ((utf8Char[1] & 0x3F) << 12) |
                            ((utf8Char[2] & 0x3F) << 6)  |
                            (utf8Char[3] & 0x3F);
            }
            else if ((c & 0xF0) == 0xE0 && utf8Char.length() >= 3)
            {
                codePoint = ((utf8Char[0] & 0x0F) << 12) |
                            ((utf8Char[1] & 0x3F) << 6)  |
                            (utf8Char[2] & 0x3F);
            }
            else
            {
                return false;
            }

            return (codePoint >= 0x1F300 && codePoint <= 0x1F9FF) ||
                (codePoint >= 0x2600 && codePoint <= 0x27BF) ||
                (codePoint >= 0x2B00 && codePoint <= 0x2BFF) ||
                (codePoint >= 0xFE00 && codePoint <= 0xFE0F) ||
                (codePoint >= 0x1F900 && codePoint <= 0x1F9FF) ||
                codePoint == 0x00A9 || codePoint == 0x00AE;
        }

        std::string escapeXml(const std::string& text)
        {
            std::string result;
            result.reserve(text.length() * 2);
            for (char ch : text)
            {
                switch (ch)
                {
                case '&': result += "&amp;"; break;
                case '<': result += "&lt;"; break;
                case '>': result += "&gt;"; break;
                case '"': result += "&quot;"; break;
                case '\'': result += "&apos;"; break;
                default: result += ch; break;
                }
            }
            return result;
        }
    }
	class Converter_Impl
	{
	public:
		Converter_Impl(int encoding);

		void convert();

		Txt::File		m_inputFile;
		OOX::CDocument	m_outputFile;

        void AddText(OOX::Logic::CParagraph *pPara , std::wstring& sText)
        {
            if (!pPara)
                return;

            OOX::WritingElement *pR = new OOX::Logic::CRun();
            if ( !pR )
                return;

            OOX::WritingElement *pT = new OOX::Logic::CText();
            if ( !pT )
            {
                delete pR;
                return;
            }

            OOX::Logic::CText *pText = (OOX::Logic::CText*)pT;
            pText->m_sText  = sText;
            pText->m_oSpace = new SimpleTypes::CXmlSpace();
            pText->m_oSpace->SetValue( SimpleTypes::xmlspacePreserve );


            ((OOX::Logic::CRun*)pR)->m_arrItems.push_back( pT );

            pPara->m_arrItems.push_back( pR );
        }
        void AddText(OOX::Logic::CParagraph *pPara , std::wstring& sText, OOX::Logic::CRunProperty * pProperty)
        {
            if (!pPara)
                return;

            OOX::WritingElement *pR = new OOX::Logic::CRun();
            if ( !pR )
                return;

            OOX::WritingElement *pT = new OOX::Logic::CText();
            if ( !pT )
            {
                delete pR;
                return;
            }

            OOX::Logic::CText *pText = (OOX::Logic::CText*)pT;
            pText->m_sText  = sText;
            pText->m_oSpace = new SimpleTypes::CXmlSpace();
            pText->m_oSpace->SetValue( SimpleTypes::xmlspacePreserve );

            if ( pProperty )
            {
                ((OOX::Logic::CRun*)pR)->m_arrItems.push_back( (OOX::WritingElement*)pProperty );
                ((OOX::Logic::CRun*)pR)->m_oRunProperty	= pProperty;//копия для удобства
            }

            ((OOX::Logic::CRun*)pR)->m_arrItems.push_back( pT );

            pPara->m_arrItems.push_back( pR );
        }
        void AddTab(OOX::Logic::CParagraph *pPara)
        {
            if (!pPara)
                return;

            OOX::WritingElement *pR = new OOX::Logic::CRun();
            if ( !pR )
                return;

            OOX::WritingElement *pTab = new OOX::Logic::CTab();
            if ( !pTab )
            {
                delete pR;
                return;
            }

            ((OOX::Logic::CRun*)pR)->m_arrItems.push_back( pTab );

            pPara->m_arrItems.push_back( pR );
        }
    };

	Converter::Converter(int encoding) : converter_( new Converter_Impl(encoding) )
	{
	}

	Converter::~Converter()
	{
		delete converter_;
	}

	void Converter::convert()
	{
		return converter_->convert();
	}

	void Converter::read(const std::wstring& path)
	{
		return converter_->m_inputFile.read(path);
	}


    void Converter::write(NSStringUtils::CStringBuilderA &stringWriter)
    {
        const char* fontName = "Courier New";
        const char* emojiFontName = "Segoe UI Emoji";
        const char* defaultSpacing = "<w:spacing w:after=\"0\" w:line=\"240\" w:lineRule=\"auto\"/>";
        for (const std::string &lineRaw : converter_->m_inputFile.m_listContentutf8)
        {
            std::string line = lineRaw;

            line.erase(std::remove(line.begin(), line.end(), '\x08'), line.end());

            stringWriter.WriteString("<w:p><w:pPr>");
            stringWriter.WriteString(defaultSpacing);
            stringWriter.WriteString("<w:rPr><w:rFonts w:ascii=\"");
            stringWriter.WriteString(fontName);
            stringWriter.WriteString("\" w:hAnsi=\"");
            stringWriter.WriteString(fontName);
            stringWriter.WriteString("\" w:cs=\"");
            stringWriter.WriteString(fontName);
            stringWriter.WriteString("\"/></w:rPr></w:pPr>");

            size_t i = 0;
            while (i < line.length())
            {
                unsigned char c = line[i];

                if ((c & 0xF0) == 0xF0 || (c & 0xE0) == 0xE0)
                {
                    int possibleLen = ((c & 0xF0) == 0xF0) ? 4 : 3;

                    if (i + possibleLen <= line.length())
                    {
                        std::string possible = line.substr(i, possibleLen);

                        if (isEmoji(possible))
                        {
                            stringWriter.WriteString("<w:r><w:rPr><w:rFonts w:ascii=\"");
                            stringWriter.WriteString(emojiFontName);
                            stringWriter.WriteString("\" w:hAnsi=\"");
                            stringWriter.WriteString(emojiFontName);
                            stringWriter.WriteString("\" w:cs=\"");
                            stringWriter.WriteString(emojiFontName);
                            stringWriter.WriteString("\"/></w:rPr><w:t xml:space=\"preserve\">");
                            stringWriter.WriteString(possible.c_str());
                            stringWriter.WriteString("</w:t></w:r>");

                            i += possibleLen;
                            continue;
                        }
                    }
                }

                size_t textStart = i;
                while (i < line.length())
                {
                    unsigned char next = line[i];
                    if ((next & 0xF0) == 0xF0 || (next & 0xE0) == 0xE0)
                    {
                        int checkLen = ((next & 0xF0) == 0xF0) ? 4 : 3;
                        if (i + checkLen <= line.length())
                        {
                            std::string check = line.substr(i, checkLen);
                            if (isEmoji(check))
                                break;
                        }
                    }
                    i++;
                }

                std::string textSegment = line.substr(textStart, i - textStart);

                size_t tabStart = 0;
                while (true)
                {
                    size_t tabPos = textSegment.find('\x09', tabStart);
                    std::string seg = (tabPos == std::string::npos) ?
                                          textSegment.substr(tabStart) :
                                          textSegment.substr(tabStart, tabPos - tabStart);

                    if (!seg.empty())
                    {
                        std::string escaped = escapeXml(seg);

                        stringWriter.WriteString("<w:r><w:rPr><w:rFonts w:ascii=\"");
                        stringWriter.WriteString(fontName);
                        stringWriter.WriteString("\" w:hAnsi=\"");
                        stringWriter.WriteString(fontName);
                        stringWriter.WriteString("\" w:cs=\"");
                        stringWriter.WriteString(fontName);
                        stringWriter.WriteString("\"/></w:rPr><w:t xml:space=\"preserve\">");
                        stringWriter.WriteString(escaped.c_str());
                        stringWriter.WriteString("</w:t></w:r>");
                    }

                    if (tabPos == std::string::npos)
                        break;

                    stringWriter.WriteString("<w:r><w:tab/></w:r>");
                    tabStart = tabPos + 1;
                }
            }
            stringWriter.WriteString("</w:p>");
        }
    }



	Converter_Impl::Converter_Impl(int encoding) : m_outputFile(NULL)
	{
		m_inputFile.m_nEncoding = encoding;
		
	}

	void Converter_Impl::convert()
	{
		//smart_ptr<OOX::File> pFile = m_outputFile.Find(OOX::FileTypes::Document);
		
		OOX::CDocument *pDocument = &m_outputFile;//NULL;

		if (!m_inputFile.m_listContent.empty() /*&& pFile.IsInit() && OOX::FileTypes::Document == pFile->type()*/)
		{
			ComplexTypes::Word::CSpacing	space;
			ComplexTypes::Word::CFonts		font;
			
			space.m_oAfter.Init();		space.m_oAfter->FromString(L"0");
			space.m_oLine.Init();		space.m_oLine->FromString(L"240");
			space.m_oLineRule.Init();	space.m_oLineRule->SetValue(SimpleTypes::linespacingruleAuto);
			
			font.m_sAscii.Init();	*font.m_sAscii	= L"Courier New";
			font.m_sHAnsi.Init();	*font.m_sHAnsi	= L"Courier New";
			font.m_sCs.Init();		*font.m_sCs		= L"Courier New";

			for (size_t i = 0; i < m_inputFile.m_listContent.size(); ++i)
			{
				std::wstring & line = m_inputFile.m_listContent[i];

				OOX::Logic::CParagraph			*paragraph	= new OOX::Logic::CParagraph();
				OOX::Logic::CParagraphProperty	*pPr		= new OOX::Logic::CParagraphProperty();
				OOX::Logic::CRunProperty		*rPr		= new OOX::Logic::CRunProperty();

				rPr->m_oRFonts		= font;
				pPr->m_oSpacing		= space;
				pPr->m_oRPr			= *rPr;

				paragraph->m_arrItems.push_back(pPr);
				paragraph->m_oParagraphProperty = pPr; //копия для удобства
				
				while(line.find(_T("\x08")) != line.npos)
				{
					line.erase(line.find(_T("\x08")), 1);//, "");
				}
				while(line.find(_T("\x09")) != line.npos)
				{
					size_t pos = line.find(_T("\x09"));
					
					if (pos != std::wstring::npos)
					{
						std::wstring s = line.substr(0, pos);
						if (!s.empty())
						{
							OOX::Logic::CRunProperty *rPr_	= new OOX::Logic::CRunProperty();
							rPr_->m_oRFonts	= font;
                            AddText(paragraph, s, rPr_);
						}
					}
                    AddTab(paragraph);
					line.erase(0, pos + 1);
				}

				if (!line.empty())
				{
                    AddText(paragraph, line, rPr);
				}
				pDocument->m_arrItems.push_back(paragraph);
			}
		}
	}
} // namespace Txt2Docx
