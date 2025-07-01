﻿/*
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

#include "TxBody.h"
#include "ClrMap.h"
#include "../Theme.h"
#include "../../Common/SimpleTypes_Word.h"

#include "boost/format.hpp"

namespace PPTX
{
	namespace Logic
	{		
		TxBody::TxBody(std::wstring name)
		{
			m_name = name;
		}
		TxBody::~TxBody() {}
		TxBody::TxBody(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const TxBody& TxBody::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		TxBody::TxBody(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
		}
		const TxBody& TxBody::operator =(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
			return *this;
		}
		TxBody::TxBody(const TxBody& oSrc) { *this = oSrc; }
		TxBody& TxBody::operator=(const TxBody& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			sp3d		= oSrc.sp3d;
			bodyPr		= oSrc.bodyPr;
			lstStyle	= oSrc.lstStyle;
			Paragrs		= oSrc.Paragrs;

			m_name		= oSrc.m_name;

			return *this;
		}
		void TxBody::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = oReader.GetName();

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();
				if (L"a:bodyPr" == strName)
				{
					bodyPr = oReader;
				}
				else if (L"a:lstStyle" == strName)
				{
					lstStyle = oReader;
				}
				else if (L"a:sp3d" == strName)
				{
					sp3d = oReader;
				}
				else if (L"a:p" == strName)
				{
					Paragraph p;
					Paragrs.push_back(p);
					Paragrs.back().fromXML(oReader);
				}
			}
			FillParentPointersForChilds();
		}
		void TxBody::fromXML(XmlUtils::CXmlNode& node)
		{
			Paragrs.clear();

			m_name		= node.GetName();

			bodyPr		= node.ReadNode(L"a:bodyPr");
			lstStyle	= node.ReadNode(L"a:lstStyle");
			sp3d		= node.ReadNode(L"a:sp3d");

			XmlMacroLoadArray(node, L"a:p", Paragrs, Paragraph);

			FillParentPointersForChilds();
		}
		std::wstring TxBody::toXML() const
		{
			XmlUtils::CNodeValue oValue;

			oValue.WriteNullable(bodyPr);
			oValue.WriteNullable(lstStyle);
			oValue.WriteArray(Paragrs);

			return XmlUtils::CreateNode(m_name, oValue);
		}
		std::wstring TxBody::toVML()
		{
			std::wstring sResult = L"<v:textbox>";
			for (auto p : Paragrs)
			{
				sResult += L"<div";
				if (p.pPr.IsInit())
				{
					//todooo
				}
				sResult += L">";

				for (auto elm : p.RunElems)
				{
					smart_ptr<Run> run = elm.GetElem().smart_dynamic_cast<Run>();
					if (run.IsInit())
					{
						if (run->rPr.IsInit())
						{
							sResult += L"<font";
							if (run->rPr->latin.is_init())
							{
								sResult += L" face=\"" + run->rPr->latin->typeface + L"\"";
							}							
							if (run->rPr->sz.is_init())
							{
								sResult += L" size=\"" + std::to_wstring(*run->rPr->sz / 5) + L"\""; //kf = 20
							}

							smart_ptr<SolidFill> solidFill = run->rPr->Fill.Fill.smart_dynamic_cast<SolidFill>();
							if (solidFill.IsInit())
							{
								SimpleTypes::CHexColor color;
								color.SetValue(SimpleTypes::hexcolorARGB);

								color.Set_A(solidFill->Color.Color->alpha);
								color.Set_R(solidFill->Color.Color->red);
								color.Set_G(solidFill->Color.Color->green);
								color.Set_B(solidFill->Color.Color->blue);

								sResult += L" color=\"#" + color.ToStringNoAlpha() + L"\"";
							}
							sResult += L">";
							if (run->rPr->b.is_init())
							{
								sResult += L"<b>";
							}
							if (run->rPr->i.is_init())
							{
								sResult += L"<i>";
							}
							sResult += run->GetText();
							if (run->rPr->i.is_init())
							{
								sResult += L"</i>";
							}
							if (run->rPr->b.is_init())
							{
								sResult += L"</b>";
							}
							sResult += L"</font>";
						}
						else 
							sResult += run->GetText();
					}
				}

				sResult += L"</div>";
			}
			sResult += L"</v:textbox>";
			return sResult;
		}
		void TxBody::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(m_name);
			pWriter->EndAttributes();

			if (bodyPr.IsInit())
			{
				bodyPr->m_namespace = L"a";
				bodyPr->toXmlWriter(pWriter);
			}
			if (sp3d.IsInit())
			{
				sp3d->toXmlWriter(pWriter);
			}
			if (lstStyle.IsInit())
				lstStyle->m_name = L"a:lstStyle";
			pWriter->Write(lstStyle);

			size_t nCount = Paragrs.size();
			for (size_t i = 0; i < nCount; ++i)
				Paragrs[i].toXmlWriter(pWriter);

			pWriter->EndNode(m_name);
		}
		void TxBody::toXmlWriterExcel(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (bodyPr.IsInit())
			{
				bodyPr->m_namespace = L"a";
				bodyPr->toXmlWriter(pWriter);
			}
			if (sp3d.IsInit())
			{
				sp3d->toXmlWriter(pWriter);
			}
			if (lstStyle.is_init())
				lstStyle->m_name = L"a:lstStyle";
			pWriter->Write(lstStyle);

			size_t nCount = Paragrs.size();
			for (size_t i = 0; i < nCount; ++i)
				Paragrs[i].toXmlWriter(pWriter);

			/*
			pWriter->EndNode(L"c:rich"));
			*/
		}
		std::wstring TxBody::GetText(bool bParagraphSeparator)const
		{
			std::wstring result;
			size_t count = Paragrs.size();

			for (size_t i = 0; i < count; ++i)
				result += Paragrs[i].GetText(bParagraphSeparator);
			return result;
		}
		void TxBody::Merge(nullable<TxBody>& txBody)
		{
			if (!bodyPr.IsInit())
				bodyPr = new Logic::BodyPr();

			bodyPr->Merge(txBody->bodyPr);

			if(lstStyle.IsInit())
				lstStyle->Merge(txBody->lstStyle);
		}
		void TxBody::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, bodyPr);
			pWriter->WriteRecord2(1, lstStyle);
			pWriter->WriteRecordArray(2, 0, Paragrs);
			pWriter->WriteRecord2(3, sp3d);
		}
		void TxBody::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						bodyPr = new Logic::BodyPr();
						bodyPr->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						lstStyle = new Logic::TextListStyle();
						lstStyle->fromPPTY(pReader);
						break;
					}
					case 2:
					{
						pReader->Skip(4);
						ULONG _c = pReader->GetULong();
						for (ULONG i = 0; i < _c; ++i)
						{
							pReader->Skip(1); // type
							Paragrs.push_back(Paragraph());
							Paragrs.back().fromPPTY(pReader);
						}
						break;
					}
					case 3:
					{
						sp3d = new Logic::Sp3d();
						sp3d->fromPPTY(pReader);
						break;
					}
					default:
					{
						break;
					}
				}
			}

			pReader->Seek(_end_rec);

			if (!bodyPr.IsInit())
				bodyPr = new Logic::BodyPr();
		}
		void TxBody::FillParentPointersForChilds()
		{
			if(bodyPr.is_init())
				bodyPr->SetParentPointer(this);

			if(lstStyle.is_init())
				lstStyle->SetParentPointer(this);

			size_t count = Paragrs.size();
			for (size_t i = 0; i < count; ++i)
				Paragrs[i].SetParentPointer(this);
		}
		std::wstring TxBody::GetDocxTxBoxContent(NSBinPptxRW::CBinaryFileWriter* pWriter, const nullable<ShapeStyle>& shape_style)
		{
			std::wstring strXml = _T("<w:txbxContent ");

			strXml += _T("\
	xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
	xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
	xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
	xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
	xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
	xmlns:v=\"urn:schemas-microsoft-com:vml\" \
	xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
	xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
	xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
	xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
	xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
	xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
	xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
	xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
	xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
	xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
	xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
	xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
	xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" \
	xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"");

			strXml += _T(">");
			
			std::wstring sThemeFont = _T("");
			
			DWORD dwColor = 0;
			if (shape_style.is_init() && shape_style->fontRef.idx.is_init())
			{
				if (shape_style->fontRef.idx->get() == _T("major"))
					sThemeFont = _T("+mj-lt");
				else
					sThemeFont = _T("+mn-lt");
			}

			NSBinPptxRW::CXmlWriter oWriter;

			oWriter.WriteString(strXml);

			double dKoef = 635; // emu -> 20pt

			size_t nCount = Paragrs.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				PPTX::Logic::Paragraph& oPar = Paragrs[i];

				oWriter.StartNode(_T("w:p"));
				oWriter.StartAttributes();
				oWriter.EndAttributes();

				nullable<PPTX::Logic::RunProperties> pRunProps;
				pRunProps = new PPTX::Logic::RunProperties();

				if (oPar.pPr.is_init())
				{
					nullable<PPTX::Logic::TextParagraphPr> pPr;
					pPr = new PPTX::Logic::TextParagraphPr();
					
					if (oPar.pPr->lvl.is_init() && lstStyle.is_init())
					{
						int nLvl = *oPar.pPr->lvl;
						if (nLvl >= 0 && nLvl < 10)
						{
							if (lstStyle->levels[nLvl].is_init())
							{
								lstStyle->levels[nLvl]->Merge(pPr);

								if (lstStyle->levels[nLvl]->defRPr.is_init())
									lstStyle->levels[nLvl]->defRPr->Merge(pRunProps);
							}
						}
					}
					oPar.pPr->Merge(pPr);

					if (pPr->indent.is_init() || pPr->marL.is_init() || pPr->marR.is_init())
					{
						oWriter.StartNode(_T("w:ind"));
						oWriter.StartAttributes();
						if (oPar.pPr->marL.is_init())
							oWriter.WriteAttribute(_T("w:left"), (int)((double)(*pPr->marL) / 635.) + 0.5);
						if (oPar.pPr->marR.is_init())
							oWriter.WriteAttribute(_T("w:right"), (int)((double)(*pPr->marR) / 635.) + 0.5);
						if (oPar.pPr->indent.is_init())
							oWriter.WriteAttribute(_T("w:firstLine"), (int)((double)(*pPr->indent) / 635.) + 0.5);
						oWriter.EndAttributes();
						oWriter.EndNode(_T("w:ind"));
					}

                    if (true)
					{
						oWriter.WriteString(_T("<w:spacing w:before=\"0\" w:after=\"0\"/>"));
					}

					if (pPr->algn.is_init())
					{
						LONG _code = pPr->algn->GetBYTECode();

						oWriter.StartNode(_T("w:jc"));
						switch (_code)
						{
						case 0:
							// center
							oWriter.WriteAttribute(_T("w:val"), (std::wstring)_T("center"));
							break;
						case 2:
						case 3:
							// both
							oWriter.WriteAttribute(_T("w:val"), (std::wstring)_T("both"));
							break;
						case 5:
							// right
							oWriter.WriteAttribute(_T("w:val"), (std::wstring)_T("right"));
							break;
						default:
							oWriter.WriteAttribute(_T("w:val"), (std::wstring)_T("left"));
							break;
						}

						oWriter.EndAttributes();
						oWriter.EndNode(_T("w:jc"));
					}

					if (pPr->fontAlgn.is_init())
					{
						BYTE nFA = pPr->fontAlgn->GetBYTECode();

						oWriter.StartNode(_T("w:textAlignment"));
						switch (nFA)
						{
						case 0: 
							oWriter.WriteAttribute(_T("w:val"), (std::wstring)_T("auto")); 
							break;
						case 1: 
							oWriter.WriteAttribute(_T("w:val"), (std::wstring)_T("bottom")); 
							break;
						case 2: 
							oWriter.WriteAttribute(_T("w:val"), (std::wstring)_T("baseline")); 
							break;
						case 3: 
							oWriter.WriteAttribute(_T("w:val"), (std::wstring)_T("center")); 
							break;
						case 4: 
							oWriter.WriteAttribute(_T("w:val"), (std::wstring)_T("top")); 
							break;
						default:
							oWriter.WriteAttribute(_T("w:val"), (std::wstring)_T("baseline")); 
							break;
						}

						oWriter.EndAttributes();
						oWriter.EndNode(_T("w:textAlignment"));
					}
				}

				size_t nCountRuns = oPar.RunElems.size();
				for (size_t j = 0; j < nCountRuns; ++j)
				{
					if (oPar.RunElems[j].is<PPTX::Logic::Run>())
					{
						PPTX::Logic::Run& oRun = oPar.RunElems[j].as<PPTX::Logic::Run>();

						nullable<PPTX::Logic::RunProperties> pRPr;
						pRPr = new PPTX::Logic::RunProperties();
						
						if (_T("") != sThemeFont)
						{
							pRPr->latin = new PPTX::Logic::TextFont();
							pRPr->latin->typeface = sThemeFont;

							pRPr->ea = new PPTX::Logic::TextFont();
							pRPr->ea->typeface = sThemeFont;

							pRPr->cs = new PPTX::Logic::TextFont();
							pRPr->cs->typeface = sThemeFont;

							pRPr->sym = new PPTX::Logic::TextFont();
							pRPr->sym->typeface = sThemeFont;
						}

						pRunProps->Merge(pRPr);

						if (oRun.rPr.is_init())
							oRun.rPr->Merge(pRPr);

						bool bIsWriteColor = false;
						DWORD dwColor = 0;

						if (pRPr->Fill.is_init() && pRPr->Fill.is<PPTX::Logic::SolidFill>())
						{
							PPTX::Logic::SolidFill& oFill = pRPr->Fill.as<PPTX::Logic::SolidFill>();

							if (oFill.Color.is_init())
							{
								bIsWriteColor = true;
								dwColor = oFill.Color.GetRGBColor(*pWriter->m_pTheme, *pWriter->m_pClrMap, 0);
							}
						}
						else if (shape_style.is_init() && shape_style->fontRef.Color.is_init())
						{
							bIsWriteColor = true;
							dwColor = shape_style->fontRef.Color.GetRGBColor(*pWriter->m_pTheme, *pWriter->m_pClrMap, 0);
						}

						oWriter.StartNode(_T("w:r"));
						oWriter.EndAttributes();

						// run props
						oWriter.StartNode(_T("w:rPr"));
						oWriter.EndAttributes();

						if (pRPr->latin.is_init() || pRPr->ea.is_init() || pRPr->cs.is_init())
						{
							oWriter.StartNode(_T("w:rFonts"));

							if (pRPr->latin.is_init())
							{
								std::wstring sPick = pWriter->m_pCommon->m_pNativePicker->GetTypefacePick(pRPr->latin.get2());

								if (sPick == _T("minor") || sPick == _T("+mn-lt"))
								{
									oWriter.WriteAttribute(_T("w:asciiTheme"), (std::wstring)_T("minorHAnsi"));
									oWriter.WriteAttribute(_T("w:hAnsiTheme"), (std::wstring)_T("minorHAnsi"));
								}
								else if (sPick == _T("major") || sPick == _T("+mj-lt"))
								{
									oWriter.WriteAttribute(_T("w:asciiTheme"), (std::wstring)_T("majorHAnsi"));
									oWriter.WriteAttribute(_T("w:hAnsiTheme"), (std::wstring)_T("majorHAnsi"));
								}
								else
								{
									oWriter.WriteAttribute(_T("w:ascii"), sPick);
									oWriter.WriteAttribute(_T("w:hAnsi"), sPick);
								}
							}
							if (pRPr->ea.is_init())
							{
								std::wstring sPick = pWriter->m_pCommon->m_pNativePicker->GetTypefacePick(pRPr->ea.get2());

								if (sPick == _T("minor") || sPick == _T("+mn-lt"))
								{
									oWriter.WriteAttribute(_T("w:eastAsiaTheme"), (std::wstring)_T("minorEastAsia"));
								}
								else if (sPick == _T("major") || sPick == _T("+mj-lt"))
								{
									oWriter.WriteAttribute(_T("w:eastAsiaTheme"), (std::wstring)_T("majorEastAsia"));
								}
								else
								{
									oWriter.WriteAttribute(_T("w:eastAsia"), sPick);
								}
							}
							if (pRPr->cs.is_init())
							{
								std::wstring sPick = pWriter->m_pCommon->m_pNativePicker->GetTypefacePick(pRPr->cs.get2());

								if (sPick == _T("minor") || sPick == _T("+mn-lt"))
								{
									oWriter.WriteAttribute(_T("w:cstheme"), (std::wstring)_T("minorBidi"));
								}
								else if (sPick == _T("major") || sPick == _T("+mj-lt"))
								{
									oWriter.WriteAttribute(_T("w:cstheme"), (std::wstring)_T("majorBidi"));
								}
								else
								{
									oWriter.WriteAttribute(_T("w:cs"), sPick);
								}
							}
							
                            oWriter.WriteNodeEnd(_T("w:rFonts"), true, true);
						}

						if (bIsWriteColor)
						{
							BYTE _r = (BYTE)((dwColor >> 16) & 0xFF);
							BYTE _g = (BYTE)((dwColor >> 8) & 0xFF);
							BYTE _b = (BYTE)((dwColor) & 0xFF);

                            std::wstringstream sstream;
                            sstream << boost::wformat( L"%02X%02X%02X" ) % _r % _g % _b;

                            std::wstring sHex = L"<w:color w:val=\"" + sstream.str() + L"\"/>";
							oWriter.WriteString(sHex);
						}

						if (pRPr->b.get_value_or(false))
							oWriter.WriteString(_T("<w:b/>"));

						if (pRPr->i.get_value_or(false))
							oWriter.WriteString(_T("<w:i/>"));

						if (pRPr->strike.is_init())
						{
							BYTE lType = pRPr->strike->GetBYTECode();
							if (0 == lType)
								oWriter.WriteString(_T("<w:dstrike/>"));
							else if (2 == lType)
								oWriter.WriteString(_T("<w:strike/>"));
						}

						if (pRPr->sz.is_init())
						{
							double dSize = (double)pRPr->sz.get();
							dSize /= 100;
							int nSize = (int)(dSize * 2);
							
                            std::wstring strFS =L"<w:sz w:val=\"" + std::to_wstring(nSize) + L"\"/><w:szCs w:val=\"" + std::to_wstring(nSize) + L"\"/>";
							oWriter.WriteString(strFS);
						}
						
						if (pRPr->u.is_init())
						{
							BYTE lType = pRPr->u->GetBYTECode();
							if (12 != lType)
								oWriter.WriteString(_T("<w:u w:val=\"single\"/>"));							
						}

						oWriter.EndNode(_T("w:rPr"));

						oWriter.StartNode(_T("w:t"));
						oWriter.StartAttributes();
						oWriter.WriteAttribute(_T("xml:space"), (std::wstring)_T("preserve"));
						oWriter.EndAttributes();
						oWriter.WriteString(oRun.GetText());
						oWriter.EndNode(_T("w:t"));

						oWriter.EndNode(_T("w:r"));
					}
				}

				oWriter.EndNode(_T("w:p"));
			}

			oWriter.WriteString(_T("</w:txbxContent>"));

			return oWriter.GetXmlString();
		}
	} // namespace Logic
} // namespace PPTX

