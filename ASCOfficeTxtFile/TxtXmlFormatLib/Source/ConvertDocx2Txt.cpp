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

#include "ConvertDocx2Txt.h"

#include "Common/Encoding.h"

#include "TxtXmlEvent.h"
#include "TxtFormat/TxtFormat.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Document.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Numbering.h"
//#include "../../../Common/DocxFormat/Source/DocxFormat/Comments.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Styles.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Footnote.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Endnote.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/HeaderFooter.h"

#include "Common/ToString.h"
#include <map>

namespace Docx2Txt
{
    static bool IsUnicodeSymbol( const wchar_t & symbol )
	{
		bool result = false;

		if ( ( 0x0009 == symbol ) || ( 0x000A == symbol ) || ( 0x000D == symbol ) ||
			( ( 0x0020 <= symbol ) && ( 0xD7FF >= symbol ) ) || ( ( 0xE000 <= symbol ) && ( symbol <= 0xFFFD ) ) ||
			( ( 0x10000 <= symbol ) && symbol ) )
		{
			result = true;
		}

		return result;		  
	}
	class Converter_Impl 
    {
    public:
        Converter_Impl();
        ~Converter_Impl();

		void convert(TxtXml::ITxtXmlEvent& Event);

        void writeUtf8		(const std::wstring& path) const;
        void writeUnicode	(const std::wstring& path) const;
        void writeBigEndian	(const std::wstring& path) const;
        void writeAnsi		(const std::wstring& path) const;

		Txt::File		m_outputFile;
		OOX::CDocx		m_inputFile;

    private:
        void convert(std::vector<OOX::WritingElement *> & items, std::vector<std::wstring>& textOut, TxtXml::ITxtXmlEvent& Event, bool isFirstLevel,
							 OOX::CDocument *pDocument, OOX::CNumbering* pNumbering, OOX::CStyles *pStyles);
       
		std::wstring convert( OOX::Logic::CRun* pRun, TxtXml::ITxtXmlEvent& Event,
			OOX::CDocument *pDocument, OOX::CNumbering* pNumbering, OOX::CStyles* pStyles);
			
		std::vector<bool> m_Field;

		int m_lPercent;
        int m_lAddition;
        bool m_bCancelled;
        int ListCount;
        int* Lists;
		
		int levelPrev;
		int listNumPrev;

		std::wstring convert(OOX::Logic::CParagraph * pParagraph, TxtXml::ITxtXmlEvent& Event,
									OOX::CDocument *pDocument, OOX::CNumbering* pNumbering, OOX::CStyles *pStyles);

        size_t NoteCount;
        std::map<std::wstring, std::vector<std::wstring>> Notes;

        static std::wstring IntToLowerLetter	(int number);
        static std::wstring IntToUpperLetter	(int number);
        static std::wstring IntToLowerRoman		(int number);
        static std::wstring IntToUpperRoman		(int number);

        static const std::wstring m_letters;
    };

    Converter::Converter() : converter_(new Converter_Impl)
    {}

	Converter::~Converter()
    {
        delete converter_;
    }

    void Converter::convert(TxtXml::ITxtXmlEvent& Event)
    {
        return converter_->convert(Event);
    }

    void Converter::read(const std::wstring & path)
    {
        bool res =  converter_->m_inputFile.Read(path);
		return;
    }

    void Converter::write(const std::wstring & path)
    {
        return converter_->m_outputFile.write(path);
    }

    void Converter::writeUtf8(const std::wstring & path) const
    {
        return converter_->writeUtf8(path);
    }

    void Converter::writeUnicode(const std::wstring & path) const
    {
        return converter_->writeUnicode(path);
    }

    void Converter::writeBigEndian(const std::wstring & path) const
    {
        return converter_->writeBigEndian(path);
    }

    void Converter::writeAnsi(const std::wstring & path) const
    {
        return converter_->writeAnsi(path);
    }


	const std::wstring Converter_Impl::m_letters = _T("abcdefghijklmnopqrstuvwxyz");

	Converter_Impl::Converter_Impl()
		:  m_lPercent(0), m_lAddition(0), m_bCancelled(false), ListCount(0), Lists(NULL), NoteCount(0), levelPrev(-1), listNumPrev(1)
	{
	}


	Converter_Impl::~Converter_Impl()
	{
		if(Lists != NULL)
		{
			delete []Lists;
			Lists = NULL;
		}
	}


	void Converter_Impl::convert(TxtXml::ITxtXmlEvent& Event)
	{
		m_lPercent = 100000;
		m_bCancelled = Event.Progress(0, m_lPercent);
		if(m_bCancelled)
			return;

		OOX::CDocument	*pDocument	= m_inputFile.m_oMain.document; 
		OOX::CStyles	*pStyles	= m_inputFile.m_oMain.styles;
		OOX::CNumbering *pNumbering = m_inputFile.m_oMain.numbering;
	
		if (pNumbering)
		{
			ListCount = (int)pNumbering->m_arrNum.size();
			Lists = new int[9 * ListCount];
			if(Lists == NULL)
				return;
			for (int i = 0; i < 9 * ListCount; i++)
				Lists[i] = 0;
		}

		if (!pDocument) return;
		if (pDocument->m_arrItems.empty()) return;

		m_lAddition = 800000;
		Notes.clear();
		
		convert(pDocument->m_arrItems, m_outputFile.m_listContent, Event, true, pDocument, pNumbering, pStyles);

		if(NoteCount != 0)
		{
			m_outputFile.m_listContent.push_back(L"");
			m_outputFile.m_listContent.push_back(L"---------------------------");
			
            for(std::map<std::wstring, std::vector<std::wstring>>::const_iterator iter_map = Notes.begin(); iter_map != Notes.end(); iter_map++)
			{
				bool bFirst = true;
				
                for(std::vector<std::wstring>::const_iterator iter = iter_map->second.begin(); iter != iter_map->second.end(); iter++)
				{
					if (bFirst) m_outputFile.m_listContent.push_back(iter_map->first + L" " + *iter);
					else		m_outputFile.m_listContent.push_back(*iter);
					
					bFirst = false;
				}
			}
		}
		if(Lists != NULL)
		{
			delete []Lists;
			Lists = NULL;
		}
		Event.Progress(0, 900000);
	}


    void Converter_Impl::writeUtf8(const std::wstring& path) const
	{
		m_outputFile.writeUtf8(path);
	}


    void Converter_Impl::writeUnicode(const std::wstring& path) const
	{
		m_outputFile.writeUnicode(path);
	}


    void Converter_Impl::writeBigEndian(const std::wstring& path) const
	{
		m_outputFile.writeBigEndian(path);
	}


    void Converter_Impl::writeAnsi(const std::wstring& path) const
	{
		m_outputFile.writeAnsi(path);
	}


    void Converter_Impl::convert(std::vector<OOX::WritingElement*> & items, std::vector<std::wstring>& textOut, TxtXml::ITxtXmlEvent& Event,
										bool isFirstLevel, OOX::CDocument *pDocument,  OOX::CNumbering* pNumbering, OOX::CStyles *pStyles)
	{
		if (items.empty()) return;

		if(isFirstLevel)
			m_lAddition = m_lAddition / items.size();
			
        for (std::vector<OOX::WritingElement*>::iterator it = items.begin(); it != items.end(); ++it)
		{
			OOX::WritingElement* item = *it;

			if (!item)continue;

			if (item->getType() == OOX::et_w_p)
			{
				textOut.push_back(convert((dynamic_cast<OOX::Logic::CParagraph*>(item)), Event, pDocument, pNumbering, pStyles));
			}
			else if (item->getType() == OOX::et_w_r)
			{
				textOut.push_back(convert((dynamic_cast<OOX::Logic::CRun*>(item)), Event, pDocument, pNumbering, pStyles));
			}
			/*else if (item.is<OOX::Logic::List>())
			{
				BOOST_FOREACH(const OOX::Logic::ListItem& listItem, *item.as<OOX::Logic::List>().m_arrItems)
				{
					convert(listItem.m_arrItems);
				}
			}*/
			//else if (item->getType() == OOX::et_w_tbl)
			//{
			//	OOX::Logic::CTbl* tbl = dynamic_cast<OOX::Logic::CTbl*>(item);
			//	for (int r = 0 ; r < tbl->m_arrItems.size(); r++)
			//	{
			//		OOX::WritingElement * item = tbl->m_arrItems[r];
			//		if (item->getType() == OOX::et_w_tr)
			//		{
			//			OOX::Logic::CTr* tr = dynamic_cast<OOX::Logic::CTr*>(item);
			//			for (int c = 0 ; c < tr->m_arrItems.size(); c++)
			//			{
			//				OOX::WritingElement * item = tr->m_arrItems[c];
			//				if (item->getType() == OOX::et_w_tc)
			//				{
			//					OOX::Logic::CTc* tc = dynamic_cast<OOX::Logic::CTc*>(item);
			//					convert(tc->m_arrItems, Event, false, pDocument, pStyles);
			//				}
			//			}
			//		}
		
			//	}
			//}
			//else if (item->getType() == OOX::et_w_sdt)
			//{
			//	convert((dynamic_cast<OOX::Logic::CSdt*>(item))->m_arrItems, text, Event, false, pDocument, pNumbering, pStyles);
			//}
			else
			{
				//todoooo  проверить - это общий случай - вместо CSdt ... да и Tbl тож
				OOX::WritingElementWithChilds<OOX::WritingElement> *item_with_items = dynamic_cast<OOX::WritingElementWithChilds<OOX::WritingElement>*>(item);
					
				if (item_with_items)
				{
					convert(item_with_items->m_arrItems, textOut, Event, false, pDocument, pNumbering, pStyles);
				}
			}

			if(isFirstLevel)
			{
				m_lPercent += m_lAddition;
				m_bCancelled = Event.Progress(0, m_lPercent);
				if(m_bCancelled)
					return;
			}
		}
	}
	std::wstring Converter_Impl::convert(OOX::Logic::CRun* pRun, TxtXml::ITxtXmlEvent& Event,
		OOX::CDocument *pDocument, OOX::CNumbering* pNumbering, OOX::CStyles* pStyles)
	{
		if (pRun == NULL) return L"";
		
		std::wstring line;

		for (size_t j = 0; pRun && j < pRun->m_arrItems.size(); ++j)
		{
			if (pRun->m_arrItems[j] == NULL) continue;

			if (pRun->m_arrItems[j]->getType() == OOX::et_w_fldChar)
			{
				OOX::Logic::CFldChar *fldChar = dynamic_cast<OOX::Logic::CFldChar*>(pRun->m_arrItems[j]);

				if ((fldChar) && (fldChar->m_oFldCharType.IsInit()))
				{
					if (fldChar->m_oFldCharType->GetValue() == SimpleTypes::fldchartypeBegin)	m_Field.push_back(true);
					else if (fldChar->m_oFldCharType->GetValue() == SimpleTypes::fldchartypeEnd)m_Field.pop_back();
					else if (false == m_Field.empty()) m_Field[m_Field.size() - 1] = false;
				}
			}
			else if ((m_Field.empty()) || (m_Field.back() == false))
			{
				bool caps = false;
				//
				//if ((pRun->m_oRunProperty) && (pRun->m_oRunProperty->m_oCaps.Init()) && (pRun->m_oRunProperty->m_oCaps->m_oVal.ToBool()))	
				//	caps = true;

				std::wstring wstr;

				switch (pRun->m_arrItems[j]->getType())
				{
					case OOX::et_w_tab:
					{
						wstr = L"\x09";
					}break;
					case  OOX::et_w_br:
					{
						wstr = L"\x0A";
					}break;
					case OOX::et_w_t:
					{
						OOX::Logic::CText* text = dynamic_cast<OOX::Logic::CText*>(pRun->m_arrItems[j]);
						wstr = text->m_sText;
						if (caps)
						{
							wstr = XmlUtils::GetUpper(wstr);
						}
					}break;
					case OOX::et_w_footnoteReference:
					case OOX::et_w_endnoteReference:
					{
						OOX::Logic::CFootnoteReference* footnote_ref = dynamic_cast<OOX::Logic::CFootnoteReference*>(pRun->m_arrItems[j]);
						OOX::Logic::CEndnoteReference* endnote_ref = dynamic_cast<OOX::Logic::CEndnoteReference*>(pRun->m_arrItems[j]);

						std::vector<std::wstring> notes_content;

						if (footnote_ref && m_inputFile.m_oMain.footnotes)
						{
							for (size_t r = 0; r < m_inputFile.m_oMain.footnotes->m_arrFootnote.size(); r++)
							{
								OOX::CFtnEdn* note = dynamic_cast<OOX::CFtnEdn*>(m_inputFile.m_oMain.footnotes->m_arrFootnote[r]);

								if (note && note->m_oId == footnote_ref->m_oId)
								{
									convert(m_inputFile.m_oMain.footnotes->m_arrFootnote[r]->m_arrItems, notes_content, Event, false, pDocument, pNumbering, pStyles);
								}
							}
							Notes.insert(std::make_pair(ToWString(++NoteCount), notes_content));
							wstr += L"[" + ToWString(NoteCount) + L"]";
						}
						if (endnote_ref && m_inputFile.m_oMain.endnotes)
						{
							for (size_t r = 0; r < m_inputFile.m_oMain.endnotes->m_arrEndnote.size(); r++)
							{
								OOX::CFtnEdn* note = dynamic_cast<OOX::CFtnEdn*>(m_inputFile.m_oMain.endnotes->m_arrEndnote[r]);

								if (note && note->m_oId == endnote_ref->m_oId)
								{
									convert(m_inputFile.m_oMain.endnotes->m_arrEndnote[r]->m_arrItems, notes_content, Event, false, pDocument, pNumbering, pStyles);
								}
							}
							Notes.insert(std::make_pair(ToWString(++NoteCount), notes_content));
							wstr += L"[" + ToWString(NoteCount) + L"]";
						}break;
					}
				}
				line += wstr;
			}
		}
		return line;
	}

	std::wstring Converter_Impl::convert(OOX::Logic::CParagraph* pParagraph, TxtXml::ITxtXmlEvent& Event, 
												OOX::CDocument *pDocument, OOX::CNumbering* pNumbering, OOX::CStyles* pStyles)
	{
		if (pParagraph == NULL) return L"";

		std::wstring line;

		int level = 0;
		int listNum = 0;

		std::wstring styleName;
		if (pParagraph->m_oParagraphProperty)
		{
			if (pParagraph->m_oParagraphProperty->m_oPStyle.IsInit())
			{
				styleName = pParagraph->m_oParagraphProperty->m_oPStyle->m_sVal.IsInit() ? pParagraph->m_oParagraphProperty->m_oPStyle->m_sVal.get() : L"";
			}
			if (false == styleName.empty() && pStyles)
			{
                std::map<std::wstring, size_t>::iterator pFind = pStyles->m_mapStyleNames.find(styleName);

				if (pFind != pStyles->m_mapStyleNames.end())
				{
					OOX::CStyle* style = pStyles->m_arrStyle[pFind->second];
			
					if ((style) && (style->m_oParPr.IsInit()) && (style->m_oParPr->m_oNumPr.IsInit()))
					{
						if (false == pParagraph->m_oParagraphProperty->m_oNumPr.IsInit())
						{
							pParagraph->m_oParagraphProperty->m_oNumPr = style->m_oParPr->m_oNumPr;
						}
						if (true == style->m_oParPr->m_oNumPr->m_oIlvl.IsInit())
						{
							level = style->m_oParPr->m_oNumPr->m_oIlvl->m_oVal.IsInit() ? *style->m_oParPr->m_oNumPr->m_oIlvl->m_oVal + 1 : 0;
						}
					}
				}
			}
			if (true == pParagraph->m_oParagraphProperty->m_oNumPr.IsInit())
			{
				if (true == pParagraph->m_oParagraphProperty->m_oNumPr->m_oIlvl.IsInit())
				{
					level = pParagraph->m_oParagraphProperty->m_oNumPr->m_oIlvl->m_oVal.IsInit() ? 
						*pParagraph->m_oParagraphProperty->m_oNumPr->m_oIlvl->m_oVal + 1 : 0;
				}
				if (true == pParagraph->m_oParagraphProperty->m_oNumPr->m_oNumID.IsInit())
				{
					listNum = pParagraph->m_oParagraphProperty->m_oNumPr->m_oNumID->m_oVal.IsInit() ? 
						*pParagraph->m_oParagraphProperty->m_oNumPr->m_oNumID->m_oVal : 1;

					listNumPrev = listNum;
				}
				else listNum = listNumPrev;
			}
		}
	
		if (listNum > 0 && pNumbering)
		{
			int start = (listNum - 1) * 9;
			if (level > 0)
			{
				level--;
				for(int i = level + 1; i < 9; i++)
				{
					Lists[start + i] = 0;
				}
			}

			listNum--;
			if ((pNumbering->m_arrNum[listNum]) && (pNumbering->m_arrNum[listNum]->m_oNumId.IsInit()))
			{
				size_t abstractNumId = 0;
				nullable_int startLvl;
				nullable_int restartLvl;

				if ((pNumbering->m_arrNum[listNum]) && (pNumbering->m_arrNum[listNum]->m_oAbstractNumId.IsInit()) &&
					(pNumbering->m_arrNum[listNum]->m_oAbstractNumId->m_oVal.IsInit()))
				{
					abstractNumId = *pNumbering->m_arrNum[listNum]->m_oAbstractNumId->m_oVal;

					std::map<int, size_t>::iterator pFind = pNumbering->m_arrNum[listNum]->m_mapLvlOverride.find(level);
					if (pFind != pNumbering->m_arrNum[listNum]->m_mapLvlOverride.end())
					{
						OOX::Numbering::CNumLvl* numLvl = pNumbering->m_arrNum[listNum]->m_arrLvlOverride[pFind->second];
						if (numLvl)
						{
							if (numLvl->m_oStartOverride.IsInit())
								startLvl = numLvl->m_oStartOverride->m_oVal;
							if (numLvl->m_oLvl.IsInit())
							{
								if (numLvl->m_oLvl->m_oLvlRestart.IsInit())
									restartLvl = numLvl->m_oLvl->m_oLvlRestart->m_oVal;
							}
						}
					}
				}
				
				OOX::Numbering::CAbstractNum* abstractNum = NULL;
				if (abstractNumId < pNumbering->m_arrAbstractNum.size())
					abstractNum = pNumbering->m_arrAbstractNum[abstractNumId];

				if (abstractNum)
				{
					int ind_level = - 1;
					std::map<int, size_t>::iterator pFind = abstractNum->m_mapLvl.find(level);
					if (pFind != abstractNum->m_mapLvl.end())
					{
						OOX::Numbering::CLvl* lvl = abstractNum->m_arrLvl[pFind->second];
						if (lvl)
						{
							ind_level = pFind->second;
							if (false == startLvl.IsInit() && lvl->m_oStart.IsInit())
								startLvl = lvl->m_oStart->m_oVal;
							if (false == restartLvl.IsInit() && lvl->m_oLvlRestart.IsInit())
								restartLvl = lvl->m_oLvlRestart->m_oVal;
						}
					}
					bool reset = true;
					if (restartLvl.IsInit())
					{
						if (*restartLvl == 0) reset = false;
						else if (*restartLvl >= Lists[start + level])
							reset = true;
					}
					else if (levelPrev >= level) reset = false;

					levelPrev = level;

					if (reset) Lists[start + level] = startLvl.IsInit() ? *startLvl : 1;
					else Lists[start + level]++;

					if (( ind_level >= 0 ) && ( abstractNum->m_arrLvl[ind_level] ))
					{
						for (int tab = 0; tab < ind_level; tab++)
							line += L"\t";

						if ((abstractNum->m_arrLvl[ind_level]->m_oNumFmt.IsInit()) && (abstractNum->m_arrLvl[ind_level]->m_oNumFmt->m_oVal.IsInit()))
						{
							std::wstring strLevelText;
							if ((abstractNum->m_arrLvl[ind_level]->m_oLvlText.IsInit()) &&  (abstractNum->m_arrLvl[ind_level]->m_oLvlText->m_sVal.IsInit()))
							{
								strLevelText =  abstractNum->m_arrLvl[ind_level]->m_oLvlText->m_sVal.get();
							}	

							if (abstractNum->m_arrLvl[ind_level]->m_oNumFmt->m_oVal->GetValue() == SimpleTypes::numberformatBullet)
							{
								////всли символ более-менее корректный 
								//if ((strLevelText.length() > 0) && (IsUnicodeSymbol(strLevelText[0]))) line += strLevelText + L" ";
								//else 
								line += L"* ";
							}
							else
							{
								std::wstring example = strLevelText;
								for (int i = 0; i < 9; i++)
								{
									std::wstring num = L"%" + ToWString(i + 1);
									while(example.find(num) != example.npos)
									{
										switch(abstractNum->m_arrLvl[ind_level]->m_oNumFmt->m_oVal->GetValue())
										{
										case SimpleTypes::numberformatLowerLetter:
											example.replace(example.find(num), 2, IntToLowerLetter(Lists[start + i]));
											break;
										case SimpleTypes::numberformatUpperLetter:
											example.replace(example.find(num), 2, IntToUpperLetter(Lists[start + i]));
											break;
										case SimpleTypes::numberformatLowerRoman:
											example.replace(example.find(num), 2, IntToLowerRoman(Lists[start + i]));
											break;
										case SimpleTypes::numberformatUpperRoman:
											example.replace(example.find(num), 2, IntToUpperRoman(Lists[start + i]));
											break;
										default:
											example.replace(example.find(num), 2, ToWString(Lists[start + i]));
											break;
										}
									}
								}
								line += example + L" ";
							}
						}
					}
				}
			}
		}

        for (size_t	i = 0; i < pParagraph->m_arrItems.size(); ++i)
		{
			if (pParagraph->m_arrItems[i] == NULL) continue;

			switch (pParagraph->m_arrItems[i]->getType())
			{
				case OOX::et_w_r:
				{
					OOX::Logic::CRun *pRun = dynamic_cast<OOX::Logic::CRun*>(pParagraph->m_arrItems[i]);
					line += convert(pRun, Event, pDocument, pNumbering, pStyles);
				}break;
				case OOX::et_w_hyperlink:
				{
					OOX::Logic::CHyperlink *pHyperlink = dynamic_cast<OOX::Logic::CHyperlink*>(pParagraph->m_arrItems[i]);
					if (pHyperlink)
					{
						std::wstring sTarget;
						std::vector<std::wstring> arDisplay;

						if ((pHyperlink) && (pHyperlink->m_oId.IsInit()))
						{
							if (pDocument) //todooo - >+headers/footers ->container
							{
								smart_ptr<OOX::File> oFile = pDocument->Find(pHyperlink->m_oId->GetValue());
								if ((oFile.IsInit()) && (OOX::FileTypes::HyperLink == oFile->type()))
								{
									OOX::HyperLink* pH = (OOX::HyperLink*)oFile.GetPointer();
									sTarget = pH->Uri().GetPath();
								}
							}
						}
						if (pHyperlink->m_sAnchor.IsInit())
						{
							sTarget += L"#" + *pHyperlink->m_sAnchor;
						}

						for (size_t j = 0; j < pHyperlink->m_arrItems.size(); ++j)
						{
							convert(pHyperlink->m_arrItems, arDisplay, Event, false, pDocument, pNumbering, pStyles);
						}
						for (size_t j = 0; j < arDisplay.size(); ++j)
							line += arDisplay[j];

						if (false == sTarget.empty())
						{
							line += L"(" + sTarget + L")";
						}				
					}
				}break;
			}
		}
		return line;
	}

	std::wstring Converter_Impl::IntToLowerLetter(int number)
	{
		number--;
		static const size_t r = (L'z' - L'a' + 1);
		std::wstring res;
		size_t r0 = number / r;

		if (r0 > 0)
		{
			std::wstring rest = IntToLowerLetter(number - r * r0);
			std::wstring res	= IntToLowerLetter(r0-1) + rest;
			return res;
		}
		else
			return std::wstring(1, (wchar_t)(L'a' + number));

		return res;
	}

	std::wstring Converter_Impl::IntToUpperLetter(int number)
	{
		number--;
		static const size_t r = (L'Z' - L'A' + 1);
		std::wstring res;
		size_t r0 = number / r;

		if (r0 > 0)
		{
			std::wstring rest = IntToUpperLetter(number - r * r0);
			std::wstring res = IntToUpperLetter(r0-1) + rest;
			return res;
		}
		else
			return std::wstring(1, (wchar_t)(L'A' + number));
		return res;
	}

	std::wstring Converter_Impl::IntToLowerRoman(int number)
	{
		std::wstring result;

		const std::wstring ixcm		= _T("ixcm");
		const std::wstring vxlcdm	= _T("vxlcdm");
		const std::wstring vld		= _T("vld");

		std::wstring str_num = ToWString(number);
		size_t len = str_num.size();
		int digit;
		for(size_t i = 0; i < len; i++)
		{
			digit = str_num[len - i - 1] - '0';
			if(i > 2)
			{
				std::wstring m(digit + 10 * (i - 3), 'm');
				result = m + result;
			}
			else
			{
				if(((digit + 1) % 5) == 0)
				{
					result.insert(0, 1, vxlcdm[i * 2 + (digit + 1) / 5 - 1]);
					result.insert(0, 1, ixcm[i]);
					//result = ixcm[i] + vxlcdm[i * 2 + (digit + 1) / 5 - 1] + result;
				}
				else
				{
					std::wstring m(digit % 5, ixcm[i]);
					if(digit >= 5)
						result = vld[i] + m + result;
					else
						result = m + result;
				}
			}
		}
		return result;
	}

	std::wstring Converter_Impl::IntToUpperRoman(int number)
	{
		std::wstring result = IntToLowerRoman(number);
		result = XmlUtils::GetUpper(result);
		return result;
	}

} // namespace Docx2Txt
