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
#include "Si.h"
#include "../../XlsbFormat/Biff12_records/SSTItem.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/XLUnicodeRichExtendedString.h"

#include "../../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../../Common/SimpleTypes_Shared.h"
namespace OOX
{
	namespace Spreadsheet
	{
		CSi::CSi()
		{
		}
		CSi::~CSi()
		{
		}
		void CSi::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSi::toXML() const
		{
			return _T("");
		}
		void CSi::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<si>"));
			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(_T("</si>"));
		}
		void CSi::toXML2(NSStringUtils::CStringBuilder& writer) const
		{
			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}
		}
		std::wstring CSi::ToString()
		{
			std::wstring sRes;

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if ( m_arrItems[i] == NULL) continue;

				OOX::Spreadsheet::WritingElement* we = m_arrItems[i];

				if(OOX::et_x_r == we->getType())
				{
					OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(we);

					for ( std::vector<CText*>::iterator it1 = pRun->m_arrItems.begin(); it1 != pRun->m_arrItems.end(); it1++)
					{
						CText* pText = *it1;
						if (pText)
						{
							sRes.append(pText->ToString());
						}
					}
				}
				else if(OOX::et_x_t == we->getType())
				{
					CText* pText = static_cast<CText*>(we);
					sRes.append(pText->ToString());
				}
			}
			return sRes;
		}
		void CSi::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				WritingElement *pItem = NULL;
				if ( _T("phoneticPr") == sName )
					AssignPtrXmlContent(pItem, CPhonetic, oReader)
				else if ( _T("r") == sName )
					AssignPtrXmlContent(pItem, CRun, oReader)
				else if ( _T("rPh") == sName )
					AssignPtrXmlContent(pItem, CRPh, oReader)
				else if ( _T("t") == sName )
					AssignPtrXmlContent(pItem, CText, oReader)

				if ( NULL != pItem )
					m_arrItems.push_back( pItem );
			}
		}
		XLS::BaseObjectPtr CSi::toBin() const
		{
			auto sString(new XLSB::SSTItem);
			XLS::BaseObjectPtr objectPtr(sString);
			auto ptr = &sString->richStr;
			ptr->fExtStr = false;
			ptr->fRichStr = false;
			for(auto i = 0; i < m_arrItems.size(); i++)
			{
				
				
				if(m_arrItems[i]->getType() == OOX::et_x_t)
				{
					auto text = static_cast<CText*>(m_arrItems[i]);
					ptr->str = text->ToString();
					continue;
				}
				
				if(m_arrItems[i]->getType() == OOX::et_x_r)
				{
					auto crunPtr = static_cast<CRun*>(m_arrItems[i]);
					ptr->fRichStr = true;
					USHORT ind = 0;
                    XLSB::StrRun run;
                    run.ich = ptr->str.value().size();
					ptr->str = ptr->str.value() + crunPtr->toBin(ind);
					run.ifnt = ind;
                    if(run.ich != 0 || run.ifnt != 0)
                        ptr->rgsStrRun.push_back(run);
					continue;
				}
				auto phonPtr = static_cast<CPhonetic*>(m_arrItems[i]);
				if(phonPtr)
				{
					ptr->fExtStr = true;
					ptr->phoneticStr = L"";
					XLSB::PhRun phRun;
					phonPtr->toBin(&phRun);
					if(i < m_arrItems.size() - 1)
					{
						auto ph = static_cast<CRPh*>(m_arrItems[i+1]);
						if(ph)
						{
							auto phoneticStr  = ph->toBin(&phRun);
							if(!phoneticStr.empty())
								ptr->phoneticStr = phoneticStr;
						}
						i++;
					}

					ptr->rgsPhRun.push_back(phRun);
				}
			}

			return objectPtr;
		}
		XLS::BiffStructurePtr CSi::toXLS() const
		{
			XLS::XLUnicodeRichExtendedString* StringPtr;
			{
				std::list<XLS::CFRecordPtr> cfRecordPtr;// just for construction
				StringPtr = new XLS::XLUnicodeRichExtendedString(cfRecordPtr);
			}
			for(auto i = 0; i < m_arrItems.size(); i++)
			{
				if(m_arrItems[i]->getType() == OOX::et_x_t)
				{
					auto text = static_cast<CText*>(m_arrItems[i]);
					StringPtr->str_ = text->ToString();
					continue;
				}
				if(m_arrItems[i]->getType() == OOX::et_x_r)
				{
					auto crunPtr = static_cast<CRun*>(m_arrItems[i]);
					StringPtr->fRichSt = true;
					USHORT ind = 0;
					XLS::FormatRun run;
					run.ich = StringPtr->str_.size();
					StringPtr->str_ = StringPtr->str_ + crunPtr->toBin(ind);
					if(ind != 0)
						run.ifnt.value() = ind + 5;
					if(run.ich != 0 || run.ifnt != 0)
						StringPtr->rgRun.push_back(run);
					continue;
				}
				auto phonPtr = static_cast<CPhonetic*>(m_arrItems[i]);
				if(phonPtr)
				{
					StringPtr->fExtSt  = true;
					StringPtr->extRst.rphssub.st = L"";
					if(phonPtr->m_oFontId.IsInit())
						StringPtr->extRst.phs.ifnt = phonPtr->m_oFontId->GetValue();
					if(phonPtr->m_oAlignment.IsInit())
						StringPtr->extRst.phs.data.alcH = phonPtr->m_oAlignment->GetValue();
					if(phonPtr->m_oType.IsInit())
						StringPtr->extRst.phs.data.phType = phonPtr->m_oType->GetValue();

					if(i < m_arrItems.size() - 1)
					{
						auto ph = static_cast<CRPh*>(m_arrItems[i+1]);
						if(ph)
						{
							XLS::PhRuns runs;
							if(ph->m_oEb.IsInit())
								runs.ichMom = ph->m_oEb->GetValue();
							if(ph->m_oSb.IsInit())
								runs.ichFirst = ph->m_oSb->GetValue();
							if(!ph->m_arrItems.empty())
							{
								StringPtr->extRst.rphssub.st.value() +=  ph->m_arrItems.back()->ToString();
								runs.ichMom = ph->m_arrItems.back()->ToString().size();
							}
							StringPtr->extRst.rgphruns.push_back(runs);
						}
						i++;
					}
				}
			}
			auto StructPtr = XLS::BiffStructurePtr(StringPtr);
			return StructPtr;
		}
		void CSi::fromBin(XLS::BiffStructure& obj, bool flagIsComment)
		{
			auto ptr = static_cast<XLSB::RichStr*>(&obj);
			CText* text             = nullptr;
			CPhonetic* phoneticPr   = nullptr;
			CRPh* rPh               = nullptr;
			CRun* r                 = nullptr;

			if(ptr != nullptr)
			{
				if(ptr->rgsStrRun.empty() || flagIsComment)
				{
					text = new CText();
					text->fromBin(ptr->str.value());
					m_arrItems.push_back(text);
				}
				else
				{
					int index = 0;
					std::wstring str;
					for(auto &strRun : ptr->rgsStrRun)
					{
						++index;
						//if there are leading spaces (possibly not only for spaces)
						if(index == 1 && strRun.ich != 0)
						{
							auto r0 = new CRun();
							str = ptr->str.value().substr(0, strRun.ich);

							auto text = new CText();
							text->fromBin(str);
							r0->m_arrItems.push_back(text);

							m_arrItems.push_back(r0);
						}
						r = new CRun();
						if(strRun.ich < ptr->str.value().size())
						{
							str = ptr->str.value().substr(strRun.ich, index == ptr->rgsStrRun.size()?ptr->str.value().size() - strRun.ich:ptr->rgsStrRun[index].ich - strRun.ich);
						}
						r->fromBin(str, strRun.ifnt);
						m_arrItems.push_back(r);
					}
				}

				for(auto &phRun : ptr->rgsPhRun)
				{
					phoneticPr = new CPhonetic();
					phoneticPr->fromBin(phRun);
					m_arrItems.push_back(phoneticPr);

					rPh = new CRPh();
					rPh->fromBin(phRun, ptr->phoneticStr.value());
					m_arrItems.push_back(rPh);
				}
			}
		}
		EElementType CSi::getType () const
		{
			return et_x_Si;
		}
		void CSi::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}
		void CSi::fromXLSBExt (NSBinPptxRW::CBinaryFileReader& oStream)
		{
			_UINT32 nCount = oStream.GetULong();
			while(nCount-- > 0)
			{
				BYTE nTypeRun = oStream.GetUChar();
				if(0x1 == nTypeRun)
				{
					OOX::Spreadsheet::CRun* pRun = new OOX::Spreadsheet::CRun();
					if(oStream.GetBool())
					{
						pRun->m_oRPr.Init();
                        pRun->m_oRPr->fromXLSB(oStream, XLSB::rt_Font);
					}
					_UINT32 nTextCount = oStream.GetULong();
					while(nTextCount-- > 0)
					{
						OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
						pText->m_sText.append(oStream.GetString2());
						pRun->m_arrItems.push_back(pText);
					}
					m_arrItems.push_back(pRun);
				}
				else if(0x2 == nTypeRun)
				{
					OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
					pText->m_sText.append(oStream.GetString2());
					m_arrItems.push_back(pText);
				}
			}
		}
		void CSi::toXLSBExt (NSBinPptxRW::CXlsbBinaryWriter& oStream)
		{
			//it's not by XLSB format
			oStream.WriteULONG(m_arrItems.size());
			for(size_t i = 0; i < m_arrItems.size(); ++i)
			{
				OOX::Spreadsheet::WritingElement* we = m_arrItems[i];
				if(OOX::et_x_r == we->getType())
				{
					oStream.WriteBYTE(0x1);
					OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(we);
					oStream.WriteBOOL(pRun->m_oRPr.IsInit());
					if(pRun->m_oRPr.IsInit())
					{
						pRun->m_oRPr->toXLSB(oStream);
					}
					oStream.WriteULONG(pRun->m_arrItems.size());
					for(size_t j = 0, length = pRun->m_arrItems.size(); j < length; ++j)
					{
						OOX::Spreadsheet::WritingElement* we = pRun->m_arrItems[j];
						if(OOX::et_x_t == we->getType())
						{
							OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(we);
							oStream.WriteString(pText->m_sText);
						}
						else
						{
							oStream.WriteString(L"");
						}
					}
				}
				else if(OOX::et_x_t == we->getType())
				{
					OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(we);
					oStream.WriteBYTE(0x2);
					oStream.WriteString(pText->m_sText);
				}
				else
				{
					oStream.WriteBYTE(0);
				}
			}
		}
		_UINT32 CSi::getXLSBSize() const
		{
			_UINT32 nLen = 4;
			for(size_t i = 0; i < m_arrItems.size(); ++i)
			{
				nLen += 1;
				OOX::Spreadsheet::WritingElement* we = m_arrItems[i];
				if(OOX::et_x_r == we->getType())
				{
					nLen += 1;
					OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(we);
					if(pRun->m_oRPr.IsInit())
					{
						nLen += pRun->m_oRPr->getXLSBSize();
					}
					nLen += 4;
					for(size_t j = 0, length = pRun->m_arrItems.size(); j < length; ++j)
					{
						OOX::Spreadsheet::WritingElement* we = pRun->m_arrItems[j];
						if(OOX::et_x_t == we->getType())
						{
							OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(we);
							nLen += 4 + NSFile::CUtf8Converter::GetUtf16SizeFromUnicode(pText->m_sText.c_str(), pText->m_sText.length());
						}
						else
						{
							nLen += 4;
						}
					}
				}
				else if(OOX::et_x_t == we->getType())
				{
					OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(we);
					nLen += 4 + NSFile::CUtf8Converter::GetUtf16SizeFromUnicode(pText->m_sText.c_str(), pText->m_sText.length());
				}
			}
			return nLen;
		}

	} //Spreadsheet
} // OOX
