/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "../../../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "TxtFormat/TxtFormat.h"
#include "TxtXmlEvent.h"

namespace Txt2Docx
{
	class Converter_Impl
	{
	public:
		Converter_Impl(int encoding);

		void convert(TxtXml::ITxtXmlEvent& Event);

		Txt::File		m_inputFile;
		OOX::CDocument	m_outputFile;

		static  CString PrepareToXML( const std::wstring & sInput)
		{
			 CString sResult = std_string2string(sInput);
			 //&amp; («&И), &lt; («<И), &gt; («>И), &apos; («'И), и &quot; («"И)
			 sResult.Replace(_T("&"), _T("&amp;"));
			 sResult.Replace(_T("<"), _T("&lt;"));
			 sResult.Replace(_T(">"), _T("&gt;"));
			 sResult.Replace(_T("\""), _T("&quot;"));
			 sResult.Replace(_T("'"), _T("&apos;"));
			 return sResult;
		}
	};

    Converter::Converter(int encoding) : converter_( new Converter_Impl(encoding) )
    {    
    }
    
    Converter::~Converter()
    {
        delete converter_;
    }
    
    void Converter::convert(TxtXml::ITxtXmlEvent& Event)
    {
        return converter_->convert(Event);    
    }

    void Converter::read(const std::wstring& path)
    {
        return converter_->m_inputFile.read(path);
    }

    void Converter::write(/*const std::wstring& path*/XmlUtils::CStringWriter & stringWriter)
    {
		for (long i=0;i < converter_->m_outputFile.m_arrItems.size(); i++)
		{
			if (converter_->m_outputFile.m_arrItems[i] != NULL)
				stringWriter.WriteString(converter_->m_outputFile.m_arrItems[i]->toXML());
		}
		//BOOL res = converter_->m_outputFile.Write(std_string2string(path.string()));
		return;
    }

	Converter_Impl::Converter_Impl(int encoding)
	{
		m_inputFile.m_nEncoding = encoding;
		
	}

	void Converter_Impl::convert(TxtXml::ITxtXmlEvent& Event)
	{
		//smart_ptr<OOX::File> pFile = m_outputFile.Find(OOX::FileTypes::Document);
		
		OOX::CDocument *pDocument = &m_outputFile;//NULL;

		if (!m_inputFile.m_listContent.empty() /*&& pFile.IsInit() && OOX::FileTypes::Document == pFile->type()*/)
		{
			int percent = 100000;
			int step = 800000 / m_inputFile.m_listContentSize; // !!!!!
			bool cancel = Event.Progress(0, 100000);
			if(cancel)
				return;

			ComplexTypes::Word::CSpacing	space;
			ComplexTypes::Word::CFonts		font;
			
			space.m_oAfter.Init();		space.m_oAfter->FromString(L"0");
			space.m_oLine.Init();		space.m_oLine->FromString(L"240");			
			space.m_oLineRule.Init();	space.m_oLineRule->SetValue(SimpleTypes::linespacingruleAuto);			
			
			font.m_sAscii.Init();	*font.m_sAscii	= "Courier New";
			font.m_sHAnsi.Init();	*font.m_sHAnsi	= "Courier New";
			font.m_sCs.Init();		*font.m_sCs		= "Courier New";

			for (std::list<std::wstring>::iterator line = m_inputFile.m_listContent.begin(); line != m_inputFile.m_listContent.end(); line++)
			{
				OOX::Logic::CParagraph			*paragraph	= new OOX::Logic::CParagraph();
				OOX::Logic::CParagraphProperty	*pPr		= new OOX::Logic::CParagraphProperty();
				OOX::Logic::CRunProperty		*rPr		= new OOX::Logic::CRunProperty();

				rPr->m_oRFonts		= font;
				pPr->m_oSpacing		= space;
				pPr->m_oRPr			= *rPr;

				paragraph->m_arrItems.push_back(pPr);
				paragraph->m_oParagraphProperty = pPr; //копия для удобства
				
				while(line->find(_T("\x08")) != line->npos)
				{
					line->erase(line->find(_T("\x08")), 1);//, "");
				}
				while(line->find(_T("\x09")) != line->npos)
                {
					int pos = line->find(_T("\x09"));
					
					if (pos > 0)
					{
						std::wstring s = line->substr(0, pos - 1);
						if (!s.empty())
						{
							OOX::Logic::CRunProperty *rPr_	= new OOX::Logic::CRunProperty();
							rPr_->m_oRFonts		= font;
                            CString s_ = PrepareToXML(s);
                            paragraph->AddText(s_, rPr_);
						}
					}
					paragraph->AddTab();
					line->erase(0, pos + 1);
                }
			
				if (!line->empty())
				{
                    CString s_ = PrepareToXML(*line);
                    paragraph->AddText(s_, rPr);
				}
				pDocument->m_arrItems.push_back(paragraph);

				percent += step;
				cancel = Event.Progress(0, percent);
				if(cancel)
					return;
			}
		}
		Event.Progress(0, 900000);
	}
} // namespace Txt2Docx
