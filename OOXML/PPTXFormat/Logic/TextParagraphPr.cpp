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

#include "TextParagraphPr.h"

namespace PPTX
{
	namespace Logic
	{
		TextParagraphPr::TextParagraphPr()
		{
			m_name = L"a:pPr";
		}
		TextParagraphPr& TextParagraphPr::operator=(const TextParagraphPr& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			lnSpc	= oSrc.lnSpc;
			spcAft	= oSrc.spcAft;
			spcBef	= oSrc.spcBef;

			buColor			= oSrc.buColor;
			buSize			= oSrc.buSize;
			buTypeface		= oSrc.buTypeface;
			ParagraphBullet	= oSrc.ParagraphBullet;

			tabLst = oSrc.tabLst;
			defRPr			= oSrc.defRPr;

			algn			= oSrc.algn;
			defTabSz		= oSrc.defTabSz;
			eaLnBrk			= oSrc.eaLnBrk;
			fontAlgn		= oSrc.fontAlgn;
			hangingPunct	= oSrc.hangingPunct;
			indent			= oSrc.indent;
			latinLnBrk		= oSrc.latinLnBrk;
			lvl				= oSrc.lvl;
			marL			= oSrc.marL;
			marR			= oSrc.marR;
			rtl				= oSrc.rtl;

			m_name			= oSrc.m_name;

			return *this;
		}
		void TextParagraphPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = oReader.GetName();
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if ( _T("a:lnSpc") == sName )
					lnSpc = oReader;
				else if ( _T("a:defRPr") == sName )
					defRPr = oReader;
				else if ( _T("a:spcAft") == sName )
					spcAft = oReader;
				else if ( _T("a:spcBef") == sName )
					spcBef = oReader;
				else if ( _T("a:tabLst") == sName )
				{
					if (false == oReader.IsEmptyNode())
					{
						int nParentDepth1 = oReader.GetDepth();
						while (oReader.ReadNextSiblingNode(nParentDepth1))
						{
							std::wstring sName1 = oReader.GetName();
							if (_T("a:tab") == sName1)
							{
								Tab tab(oReader);
								tabLst.push_back(tab);
							}
						}
					}
				}
				//else if ( _T("a:extLst") == sName )
				//	extLst = oReader;

				else if ( _T("a:buBlip") == sName || _T("a:buChar") == sName || _T("a:buAutoNum") == sName || sName == _T("a:buNone"))
					ParagraphBullet.fromXML(oReader);
				else if ( _T("a:buClr") == sName || sName == _T("a:buClrTx"))
					buColor.fromXML(oReader);
				else if ( _T("a:buFont") == sName ||  _T("a:buFontTx") == sName )
					buTypeface.fromXML( oReader);
				else if ( _T("a:buSzTx") == sName || sName == _T("a:buSzPct")  || sName == _T("a:buSzPts"))
					buSize.fromXML(oReader);
			}
		}
		OOX::EElementType TextParagraphPr::getType() const
		{
			return OOX::et_a_pPr;
		}
		void TextParagraphPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = node.GetName();

			XmlMacroReadAttributeBase(node, L"algn", algn);
			XmlMacroReadAttributeBase(node, L"defTabSz", defTabSz);
			XmlMacroReadAttributeBase(node, L"eaLnBrk", eaLnBrk);
			XmlMacroReadAttributeBase(node, L"fontAlgn", fontAlgn);
			XmlMacroReadAttributeBase(node, L"hangingPunct", hangingPunct);
			XmlMacroReadAttributeBase(node, L"indent", indent);
			XmlMacroReadAttributeBase(node, L"latinLnBrk", latinLnBrk);
			XmlMacroReadAttributeBase(node, L"lvl", lvl);
			XmlMacroReadAttributeBase(node, L"marL", marL);
			XmlMacroReadAttributeBase(node, L"marR", marR);
			XmlMacroReadAttributeBase(node, L"rtl", rtl);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("lnSpc") == strName)
						lnSpc = oNode;
					else if (_T("spcAft") == strName)
						spcAft = oNode;
					else if (_T("spcBef") == strName)
						spcBef = oNode;
					else if (_T("defRPr") == strName)
						defRPr = oNode;
					else if (_T("tabLst") == strName)
						XmlMacroLoadArray(oNode, _T("a:tab"), tabLst, Tab);
				}
			}


			buColor.ReadBulletColorFrom(node);
			buSize.ReadBulletSizeFrom(node);
			buTypeface.ReadBulletTypefaceFrom(node);
			ParagraphBullet.ReadBulletFrom(node);

			Normalize();

			FillParentPointersForChilds();
		}
		void TextParagraphPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if	  ( oReader, _T("rtl"),		rtl)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("lvl"),		lvl)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("algn"),	algn)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("fontAlgn"),fontAlgn)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("marL"),	marL)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("marR"),	marR)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("indent"),	indent)
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("defTabSz"),defTabSz)
			WritingElement_ReadAttributes_End	( oReader )
		}
		void TextParagraphPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(m_name);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("marL"), marL);
			pWriter->WriteAttribute(_T("marR"), marR);
			pWriter->WriteAttribute(_T("lvl"), lvl);
			pWriter->WriteAttribute(_T("indent"), indent);
			pWriter->WriteAttribute(_T("algn"), algn);
			pWriter->WriteAttribute(_T("defTabSz"), defTabSz);
			pWriter->WriteAttribute(_T("rtl"), rtl);
			pWriter->WriteAttribute(_T("eaLnBrk"), eaLnBrk);
			pWriter->WriteAttribute(_T("fontAlgn"), fontAlgn);
			pWriter->WriteAttribute(_T("latinLnBrk"), latinLnBrk);
			pWriter->WriteAttribute(_T("hangingPunct"), hangingPunct);
			pWriter->EndAttributes();

			pWriter->Write(lnSpc);
			pWriter->Write(spcBef);
			pWriter->Write(spcAft);
			buColor.toXmlWriter(pWriter);
			buSize.toXmlWriter(pWriter);
			buTypeface.toXmlWriter(pWriter);
			ParagraphBullet.toXmlWriter(pWriter);

			pWriter->WriteArray(_T("a:tabLst"), tabLst);

			pWriter->Write(defRPr);

			pWriter->EndNode(m_name);
		}
		void TextParagraphPr::Merge(nullable<TextParagraphPr>& props)
		{
			if(!props.is_init())
				props = TextParagraphPr();

			if(lnSpc.is_init())
			{
				props->lnSpc = lnSpc;
			}
			if(spcAft.is_init())
			{
				props->spcAft = spcAft;
			}
			if(spcBef.is_init())
			{
				props->spcBef = spcBef;
			}

			if(buColor.is_init())
				props->buColor = buColor;
			if(buSize.is_init())
				props->buSize = buSize;
			if(buTypeface.is_init())
				props->buTypeface = buTypeface;
			if(ParagraphBullet.is_init())
				props->ParagraphBullet = ParagraphBullet;

			//property<std::list<Tab> > tabLst;// (Tab List)
			if(defRPr.is_init())
				defRPr->Merge(props->defRPr);

			// Attribs
			if(algn.is_init())			props->algn = *algn;
			if(defTabSz.is_init())		props->defTabSz = *defTabSz;
			if(eaLnBrk.is_init())		props->eaLnBrk = *eaLnBrk;
			if(fontAlgn.is_init())		props->fontAlgn = *fontAlgn;
			if(hangingPunct.is_init())	props->hangingPunct = *hangingPunct;
			if(indent.is_init())		props->indent = *indent;
			if(latinLnBrk.is_init())	props->latinLnBrk = *latinLnBrk;
			if(lvl.is_init())			props->lvl = *lvl;
			if(marL.is_init())			props->marL = *marL;
			if(marR.is_init())			props->marR = *marR;
			if(rtl.is_init())			props->rtl = *rtl;
		}
		void TextParagraphPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, algn);
			pWriter->WriteInt2(1, defTabSz);
			pWriter->WriteBool2(2, eaLnBrk);
			pWriter->WriteLimit2(3, fontAlgn);
			pWriter->WriteBool2(4, hangingPunct);
			pWriter->WriteInt2(5, indent);
			pWriter->WriteBool2(6, latinLnBrk);
			pWriter->WriteInt2(7, lvl);
			pWriter->WriteInt2(8, marL);
			pWriter->WriteInt2(9, marR);
			pWriter->WriteBool2(10, rtl);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, lnSpc);
			pWriter->WriteRecord2(1, spcAft);
			pWriter->WriteRecord2(2, spcBef);

			pWriter->WriteRecord1(3, buColor);
			pWriter->WriteRecord1(4, buSize);
			pWriter->WriteRecord1(5, buTypeface);
			pWriter->WriteRecord1(6, ParagraphBullet);

			pWriter->WriteRecordArray(7, 0, tabLst);

			pWriter->WriteRecord2(8, defRPr);
		}
		void TextParagraphPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						algn = pReader->GetUChar();
						break;
					}
					case 1:
					{
						defTabSz = pReader->GetLong();
						break;
					}
					case 2:
					{
						eaLnBrk = pReader->GetBool();
						break;
					}
					case 3:
					{
						fontAlgn = pReader->GetUChar();
						break;
					}
					case 4:
					{
						hangingPunct = pReader->GetBool();
						break;
					}
					case 5:
					{
						indent = pReader->GetLong();
						break;
					}
					case 6:
					{
						latinLnBrk = pReader->GetBool();
						break;
					}
					case 7:
					{
						lvl = pReader->GetLong();
						break;
					}
					case 8:
					{
						marL = pReader->GetLong();
						break;
					}
					case 9:
					{
						marR = pReader->GetLong();
						break;
					}
					case 10:
					{
						rtl = pReader->GetBool();
						break;
					}
					default:
						break;
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						lnSpc = new TextSpacing();
						lnSpc->m_name = _T("a:lnSpc");
						lnSpc->fromPPTY(pReader);
						break;
					}
					case 1:
					{
						spcAft = new TextSpacing();
						spcAft->m_name = _T("a:spcAft");
						spcAft->fromPPTY(pReader);
						break;
					}
					case 2:
					{
						spcBef = new TextSpacing();
						spcBef->m_name = _T("a:spcBef");
						spcBef->fromPPTY(pReader);
						break;
					}
					case 3:
					{
						buColor.fromPPTY(pReader);
						break;
					}
					case 4:
					{
						buSize.fromPPTY(pReader);
						break;
					}
					case 5:
					{
						buTypeface.fromPPTY(pReader);
						break;
					}
					case 6:
					{
						ParagraphBullet.fromPPTY(pReader);
						break;
					}
					case 7:
					{
						pReader->Skip(4);
						ULONG _c = pReader->GetULong();

						if (0 != _c)
						{
							tabLst.clear();
							for (ULONG i = 0; i < _c; i++)
							{
								pReader->Skip(6); // type, len, start attr
								size_t nIndex = tabLst.size();
								tabLst.push_back(Tab());

								while (true)
								{
									BYTE __at = pReader->GetUChar_TypeNode();
									if (__at == NSBinPptxRW::g_nodeAttributeEnd)
										break;

									switch (__at)
									{
										case 0:
										{
											tabLst[nIndex].algn = pReader->GetUChar();
											break;
										}
										case 1:
										{
											tabLst[nIndex].pos = pReader->GetLong();
											break;
										}
										default:
											break;
									}
								}
							}
						}
						break;
					}
					case 8:
					{
						defRPr = new Logic::RunProperties();
						defRPr->m_name = _T("a:defRPr");
						defRPr->fromPPTY(pReader);
						break;
					}
					default:
					{
						pReader->SkipRecord();
					}
				}
			}

			pReader->Seek(_end_rec);
		}
		void TextParagraphPr::FillParentPointersForChilds()
		{
			if(lnSpc.is_init())
				lnSpc->SetParentPointer(this);
			if(spcAft.is_init())
				spcAft->SetParentPointer(this);
			if(spcBef.is_init())
				spcBef->SetParentPointer(this);

			buColor.SetParentPointer(this);
			buSize.SetParentPointer(this);
			buTypeface.SetParentPointer(this);
			ParagraphBullet.SetParentPointer(this);

			if(defRPr.is_init())
				defRPr->SetParentPointer(this);
		}
		void TextParagraphPr::Normalize()
		{
			indent.normalize(-51206400, 51206400);
			lvl.normalize(0, 8);
			marL.normalize(0, 51206400);
			marR.normalize(0, 51206400);
		}
	} // namespace Logic
} // namespace PPTX
