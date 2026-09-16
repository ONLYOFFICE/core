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
#include "ConvertTxt2Docx.h"
#include "TxtFormat/File.h"

#include "../../DesktopEditor/common/File.h"

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
		bool isEmojiAt(const std::string& line, size_t pos, size_t len) // utf8 
		{
			unsigned char c = line[pos];
			uint32_t codePoint = 0;

			if ((c & 0xF8) == 0xF0 && len >= 4)
			{
				codePoint = ((line[pos] & 0x07) << 18) |
							((line[pos + 1] & 0x3F) << 12) |
							((line[pos + 2] & 0x3F) << 6)  |
							(line[pos + 3] & 0x3F);
			}
			else if ((c & 0xF0) == 0xE0 && len >= 3)
			{
				codePoint = ((line[pos] & 0x0F) << 12) |
							((line[pos + 1] & 0x3F) << 6)  |
							(line[pos + 2] & 0x3F);
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
		bool isEmoji(const std::string& utf8Char)
		{
			if (utf8Char.empty()) return false;

			return isEmojiAt(utf8Char, 0, utf8Char.length());
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
                ((OOX::Logic::CRun*)pR)->m_oRunProperty	= pProperty;//copy for convenience
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
				paragraph->m_oParagraphProperty = pPr; //copy for convenience
				
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
namespace
{
	const std::wstring c_sFontName		= L"Courier New";
	const std::wstring c_sEmojiFontName	= L"Segoe UI Emoji";

	const size_t c_nContentPortion = 1000;

	void SetRunFonts(OOX::Logic::CRunProperty& oRPr, const std::wstring& sFontName)
	{
		ComplexTypes::Word::CFonts font;

		font.m_sAscii.Init();	*font.m_sAscii	= sFontName;
		font.m_sHAnsi.Init();	*font.m_sHAnsi	= sFontName;
		font.m_sCs.Init();		*font.m_sCs		= sFontName;

		oRPr.m_oRFonts = font;
	}
	size_t emojiLength(const std::string& line, size_t pos)
	{
		unsigned char c = line[pos];

		if ((c & 0xF0) == 0xF0 || (c & 0xE0) == 0xE0)
		{
			size_t len = ((c & 0xF0) == 0xF0) ? 4 : 3;

			if (pos + len <= line.length() && isEmojiAt(line, pos, len))
				return len;
		}
		return 0;
	}
}

	ContentProvider::ContentProvider(const std::wstring& path, int encoding) : path_(path), file_(new Txt::File()), read_(false), pos_(0),
		paraPr_(-1), textRPr_(-1), emojiRPr_(-1)
	{
		file_->m_nEncoding = encoding;
	}

	ContentProvider::~ContentProvider()
	{
		delete file_;
	}

	void ContentProvider::writeText(BinDocxRW::IDocumentContentWriter& oWriter, int nRPr, const std::string& line, size_t start, size_t end)
	{
		if (start >= end)
			return;

		buffer_.clear();
		NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)line.c_str() + start, (LONG)(end - start), buffer_);

		oWriter.WriteRunText(nRPr, buffer_);
	}

	void ContentProvider::writeParagraph(BinDocxRW::IDocumentContentWriter& oWriter, const std::string& lineRaw)
	{
		oWriter.StartParagraph(paraPr_);

		const std::string* pLine = &lineRaw;
		std::string lineClean;

		if (lineRaw.find('\x08') != std::string::npos)
		{
			lineClean = lineRaw;
			lineClean.erase(std::remove(lineClean.begin(), lineClean.end(), '\x08'), lineClean.end());
			pLine = &lineClean;
		}
		const std::string& line = *pLine;

		size_t i = 0;
		while (i < line.length())
		{
			size_t len = emojiLength(line, i);
			if (len > 0)
			{
				writeText(oWriter, emojiRPr_, line, i, i + len);

				i += len;
				continue;
			}

			size_t textStart = i;
			while (i < line.length())
			{
				if (emojiLength(line, i) > 0)
					break;
				i++;
			}

			// text with tabs
			size_t segStart = textStart;
			for (size_t j = textStart; j < i; ++j)
			{
				if (line[j] == '\x09')
				{
					writeText(oWriter, textRPr_, line, segStart, j);
					oWriter.WriteRunTab(-1);

					segStart = j + 1;
				}
			}
			writeText(oWriter, textRPr_, line, segStart, i);
		}

		oWriter.EndParagraph();
	}

	bool ContentProvider::WriteNextContent(BinDocxRW::IDocumentContentWriter& oWriter)
	{
		if (!read_)
		{
			read_ = true;
			try
			{
				file_->read(path_);
			}
			catch(...)
			{
				return false;
			}

			OOX::Logic::CParagraphProperty oPPr;

			ComplexTypes::Word::CSpacing space;
			space.m_oAfter.Init();		space.m_oAfter->FromString(L"0");
			space.m_oLine.Init();		space.m_oLine->FromString(L"240");
			space.m_oLineRule.Init();	space.m_oLineRule->SetValue(SimpleTypes::linespacingruleAuto);
			oPPr.m_oSpacing = space;

			oPPr.m_oRPr.Init();
			SetRunFonts(oPPr.m_oRPr.get2(), c_sFontName);

			OOX::Logic::CRunProperty oTextRPr, oEmojiRPr;
			SetRunFonts(oTextRPr, c_sFontName);
			SetRunFonts(oEmojiRPr, c_sEmojiFontName);

			paraPr_		= oWriter.AddParagraphProperty(oPPr);
			textRPr_	= oWriter.AddRunProperty(oTextRPr);
			emojiRPr_	= oWriter.AddRunProperty(oEmojiRPr);
		}

		std::vector<std::string>& arLines = file_->m_listContentutf8;

		if (pos_ >= arLines.size())
			return false;

		size_t nEnd = (std::min)(pos_ + c_nContentPortion, arLines.size());

		for (; pos_ < nEnd; ++pos_)
		{
			writeParagraph(oWriter, arLines[pos_]);

			std::string().swap(arLines[pos_]);
		}
		return true;
	}

} // namespace Txt2Docx
