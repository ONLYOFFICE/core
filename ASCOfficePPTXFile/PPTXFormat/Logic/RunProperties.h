﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#pragma once
#ifndef PPTX_LOGIC_RUNPROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_RUNPROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/TextCaps.h"
#include "./../Limit/TextStrike.h"
#include "./../Limit/TextUnderline.h"

#include "Ln.h"
#include "UniFill.h"
#include "TextFont.h"
#include "EffectProperties.h"
#include "Hyperlink.h"

namespace PPTX
{
	namespace Logic
	{
		class Rtl : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Rtl)
			PPTX_LOGIC_BASE2(Rtl)

			virtual OOX::EElementType getType () const
			{
				return OOX::et_a_rtl;
			}			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{

			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{			
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				pReader->Seek(_end_rec);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(L"<a:rtl/>");				
			}
		protected:
			virtual void FillParentPointersForChilds(){};
		};

		class UFillTx : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(UFillTx)
			
			UFillTx(std::wstring name = L"a:uFillTx")
			{
				m_name = name;
			}
			virtual OOX::EElementType getType () const
			{
				return OOX::et_a_uFillTx;
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();
				
				Fill.GetFillFrom(node);
				FillParentPointersForChilds();
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_name = oReader.GetName();
				
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					
					Fill.fromXML(oReader);
				}	

				FillParentPointersForChilds();
			}

			void Merge(nullable<UFillTx>& uFillTx)const
			{
				if(!uFillTx.is_init())
					uFillTx = UFillTx();

				uFillTx->m_name = m_name;
				
				if(Fill.is_init())	
					uFillTx->Fill = Fill;
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{			
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
				
				pWriter->WriteRecord1(0, Fill);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
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
							Fill.fromPPTY(pReader);
						}break;
						default:
						{
						}break;
					}
				}
				pReader->Seek(_end_rec);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);
				pWriter->EndAttributes();
				Fill.toXmlWriter(pWriter);
				pWriter->EndNode(m_name);
			}

			UniFill				Fill;
			std::wstring		m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Fill.SetParentPointer(this);
			}
		};

		class Highlight : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Highlight)
			
			Highlight()
			{
			}
			virtual OOX::EElementType getType () const
			{
				return OOX::et_a_highlight;
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Color.GetColorFrom(node);
				FillParentPointersForChilds();
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					
					Color.fromXML(oReader);
				}	

				FillParentPointersForChilds();
			}

			void Merge(nullable<Highlight>& highlight)const
			{
				if(!highlight.is_init())
					highlight = Highlight();

				if(Color.is_init())	
					highlight->Color = Color;
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{			
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
				
				pWriter->WriteRecord1(0, Color);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
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
							Color.fromPPTY(pReader);
						}break;
						default:
						{
						}break;
					}
				}
				pReader->Seek(_end_rec);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(L"a:highlight");
				pWriter->EndAttributes();
					Color.toXmlWriter(pWriter);
				pWriter->EndNode(L"a:highlight");
			}

			UniColor Color;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Color.SetParentPointer(this);
			}
		};

		class RunProperties : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(RunProperties)
			
			RunProperties()
			{
				m_name = L"a:rPr";
			}
			virtual OOX::EElementType getType () const
			{
				return OOX::et_a_rPr;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_name = oReader.GetName();

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					
					if (L"blipFill"	== sName	||
						L"gradFill"	== sName	||
						L"grpFill"	== sName	||
						L"noFill"	== sName	||
						L"pattFill"	== sName	||
						L"solidFill" == sName )
					{
						Fill.fromXML(oReader);
					}
					else if ( L"ln" == sName )
						ln = oReader;
					else if ( L"cs" == sName )
						cs = oReader;					
					else if ( L"ea" == sName )
						ea = oReader;				
					else if ( L"latin" == sName )
						latin = oReader;			
					else if ( L"sym" == sName )
						sym = oReader;		
					else if ( L"uFill" == sName )
						uFill = oReader;		
					else if ( L"uFillTx" == sName )
						uFillTx = oReader;		
					else if ( L"hlinkClick" == sName )
						hlinkClick = oReader;			
					else if (L"hlinkMouseOver" == sName)
						hlinkMouseOver = oReader;
					else if ( L"rtl" == sName )
						rtl = oReader;
					else if ( L"rtl" == sName )
						rtl = oReader;
					else if (	L"effectDag"	== sName	||
								L"effectLst"	== sName	||
								L"extLst"		== sName )
					{
						EffectList.fromXML(oReader);		
					}
				}			
				FillParentPointersForChilds();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();

				node.ReadAttributeBase(L"altLang", altLang);
				node.ReadAttributeBase(L"b", b);
				node.ReadAttributeBase(L"bmk", bmk);
				node.ReadAttributeBase(L"cap", cap);
				node.ReadAttributeBase(L"dirty", dirty);
				node.ReadAttributeBase(L"err", err);
				node.ReadAttributeBase(L"i", i);
				node.ReadAttributeBase(L"kern", kern);
				node.ReadAttributeBase(L"kumimoji", kumimoji);
				node.ReadAttributeBase(L"lang", lang);
				node.ReadAttributeBase(L"noProof", noProof);
				node.ReadAttributeBase(L"normalizeH", normalizeH);
				node.ReadAttributeBase(L"smtClean", smtClean);
				node.ReadAttributeBase(L"smtId", altLang);
				node.ReadAttributeBase(L"strike", strike);
				node.ReadAttributeBase(L"sz", sz);
				node.ReadAttributeBase(L"u", u);
				node.ReadAttributeBase(L"baseline", baseline);
				node.ReadAttributeBase(L"spc", spc);

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(L"*", oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (L"ln" == strName)
							ln = oNode;
						else if (L"latin" == strName)
							latin = oNode;
						else if (L"ea" == strName)
							ea = oNode;
						else if (L"cs" == strName)
							cs = oNode;
						else if (L"sym" == strName)
							sym = oNode;
						else if (L"hlinkClick" == strName)
							hlinkClick = oNode;
						else if (L"hlinkMouseOver" == strName)
							hlinkMouseOver = oNode;
						else if (L"rtl" == strName)
							rtl = oNode;
						else if (L"uFill" == strName)
							uFill = oNode;
						else if (L"uFillTx" == strName)
							uFillTx = oNode;
						else if (L"highlight" == strName)
							highlight = oNode;
					}
				}

				Fill.GetFillFrom(node);
				EffectList.GetEffectListFrom(node);

				Normalize();
				
				FillParentPointersForChilds();
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
					WritingElement_ReadAttributes_Read_if	  ( oReader, L"altLang",	altLang)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"b",			b)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"bmk",		bmk)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"cap",		cap)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"dirty",		dirty)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"err",		err)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"i",			i)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"kern",		kern)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"kumimoji",	kumimoji)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"lang",		lang)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"noProof",	noProof)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"normalizeH",	normalizeH)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"smtClean",	smtClean)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"smtId",		smtId)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"strike",		strike)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"sz",			sz)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"u",			u)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"baseline",	baseline)
					WritingElement_ReadAttributes_Read_else_if( oReader, L"spc",		spc)
				WritingElement_ReadAttributes_End	( oReader )		
				
				Normalize();
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);

				pWriter->StartAttributes();

				pWriter->WriteAttribute(L"kumimoji", kumimoji);
				pWriter->WriteAttribute(L"lang", lang);
				pWriter->WriteAttribute(L"altLang", altLang);
				pWriter->WriteAttribute(L"sz", sz);
				pWriter->WriteAttribute(L"b", b);
				pWriter->WriteAttribute(L"i", i);
				pWriter->WriteAttribute(L"u", u);
				pWriter->WriteAttribute(L"strike", strike);
				pWriter->WriteAttribute(L"kern", kern);
				pWriter->WriteAttribute(L"cap", cap);
				pWriter->WriteAttribute(L"spc", spc);
				pWriter->WriteAttribute(L"normalizeH", normalizeH);
				pWriter->WriteAttribute(L"baseline", baseline);
				pWriter->WriteAttribute(L"noProof", noProof);
				pWriter->WriteAttribute(L"dirty", dirty);
				pWriter->WriteAttribute(L"err", err);
				pWriter->WriteAttribute(L"smtClean", smtClean);
				pWriter->WriteAttribute(L"smtId", smtId);
				pWriter->WriteAttribute(L"bmk", bmk);

				pWriter->EndAttributes();

				pWriter->Write(ln);
				Fill.toXmlWriter(pWriter);
				EffectList.toXmlWriter(pWriter);
				pWriter->Write(latin);
				pWriter->Write(ea);
				pWriter->Write(cs);
				pWriter->Write(sym);
				pWriter->Write(hlinkClick);
				pWriter->Write(hlinkMouseOver);
				pWriter->Write(rtl);
				pWriter->Write(uFill);
				pWriter->Write(uFillTx);
				pWriter->Write(highlight);

				pWriter->EndNode(m_name);
			}


			void Merge(nullable<RunProperties>& props)const
			{
				if(!props.is_init())
					props = new RunProperties();

				if(ln.is_init())
					ln->Merge(props->ln);
				if(Fill.is_init())
					props->Fill = Fill;
				if(uFill.is_init())
					uFill->Merge(props->uFill);
				if(uFillTx.is_init())
					uFillTx->Merge(props->uFillTx);
				if (highlight.is_init())
					highlight->Merge(props->highlight);

	//			EffectProperties EffectList;
				//highlight (Highlight Color)  §21.1.2.3.4 
				//uLn (Underline Stroke)  §21.1.2.3.14 
				//uLnTx (Underline Follows Text)  §21.1.2.3.15 

				if(latin.is_init())	latin->Merge(props->latin);
				if(ea.is_init())	ea->Merge(props->ea);
				if(cs.is_init())	cs->Merge(props->cs);
				if(sym.is_init())	sym->Merge(props->sym);
	/*
				nullable_property<Hyperlink> hlinkClick;
				nullable_property<Hyperlink> hlinkMouseOver;
	*/
				if(rtl.is_init())		props->rtl		= new Logic::Rtl();
				if(altLang.is_init())	props->altLang	= *altLang;
				if(b.is_init())			props->b		= *b;
				if(baseline.is_init())	props->baseline = *baseline;
				if(bmk.is_init())		props->bmk		= *bmk;
				if(cap.is_init())		props->cap		= *cap;
				if(dirty.is_init())		props->dirty	= *dirty;
				if(err.is_init())		props->err		= *err;
				if(i.is_init())			props->i		= *i;
				if(kern.is_init())		props->kern		= *kern;
				if(kumimoji.is_init())	props->kumimoji = *kumimoji;
				if(lang.is_init())		props->lang		= *lang;
				if(noProof.is_init())	props->noProof	= *noProof;
				if(normalizeH.is_init())props->normalizeH = *normalizeH;
				if(smtClean.is_init())	props->smtClean = *smtClean;
				if(smtId.is_init())		props->smtId	= *smtId;
				if(spc.is_init())		props->spc		= *spc;
				if(strike.is_init())	props->strike	= *strike;
				if(sz.is_init())		props->sz		= *sz;
				if(u.is_init())			props->u		= *u;
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

				pWriter->WriteString2(0,	altLang);
				pWriter->WriteBool2	(1,		b);
				pWriter->WriteInt2	(2,		baseline);
				pWriter->WriteString2(3,	bmk);
				pWriter->WriteLimit2(4,		cap);
				pWriter->WriteBool2	(5,		dirty);
				pWriter->WriteBool2	(6,		err);
				pWriter->WriteBool2	(7,		i);
				pWriter->WriteInt2	(8,		kern);
				pWriter->WriteBool2	(9,		kumimoji);
				pWriter->WriteString2(10,	lang);
				pWriter->WriteBool2	(11,	noProof);
				pWriter->WriteBool2	(12,	normalizeH);
				pWriter->WriteBool2	(13,	smtClean);
				pWriter->WriteInt2	(14,	smtId);
				pWriter->WriteInt2	(15,	spc);
				pWriter->WriteLimit2(16,	strike);
				pWriter->WriteInt2	(17,	sz);
				pWriter->WriteLimit2(18,	u);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, ln);
				pWriter->WriteRecord1(1, Fill);
				pWriter->WriteRecord1(2, EffectList);

				pWriter->WriteRecord2(3, latin);
				pWriter->WriteRecord2(4, ea);
				pWriter->WriteRecord2(5, cs);
				pWriter->WriteRecord2(6, sym);

				pWriter->WriteRecord2(7, hlinkClick);
				pWriter->WriteRecord2(8, hlinkMouseOver);
				pWriter->WriteRecord2(9, rtl);
				
				pWriter->WriteRecord2(10, uFill);
				pWriter->WriteRecord2(11, uFillTx);

				pWriter->WriteRecord2(12, highlight);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

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
							altLang = pReader->GetString2();
						}break;
						case 1:
						{
							b = pReader->GetBool();
						}break;
						case 2:
						{
							baseline = pReader->GetLong();
						}break;
						case 3:
						{
							bmk = pReader->GetString2();
						}break;
						case 4:
						{
							cap = new Limit::TextCaps();
							cap->SetBYTECode(pReader->GetUChar());
						}break;
						case 5:
						{
							dirty = pReader->GetBool();
						}break;
						case 6:
						{
							err = pReader->GetBool();
						}break;
						case 7:
						{
							i = pReader->GetBool();
						}break;
						case 8:
						{
							kern = pReader->GetLong();
						}break;
						case 9:
						{
							kumimoji = pReader->GetBool();
						}break;
						case 10:
						{
							lang = pReader->GetString2();
						}break;
						case 11:
						{
							noProof = pReader->GetBool(); // noproof
						}break;
						case 12:
						{
							normalizeH = pReader->GetBool();
						}break;
						case 13:
						{
							smtClean = pReader->GetBool();
						}break;
						case 14:
						{
							smtId = pReader->GetLong();
						}break;
						case 15:
						{
							spc = pReader->GetLong();
						}break;
						case 16:
						{
							strike = new Limit::TextStrike();
							strike->SetBYTECode(pReader->GetUChar());
						}break;
						case 17:
						{
							sz = pReader->GetLong();
						}break;
						case 18:
						{
							u = new Limit::TextUnderline();
							u->SetBYTECode(pReader->GetUChar());
						}break;
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
							ln = new Logic::Ln();
							ln->fromPPTY(pReader);
						}break;
						case 1:
						{
							Fill.fromPPTY(pReader);
						}break;
						case 2:
						{
							EffectList.fromPPTY(pReader);
						}break;
						case 3:
						{
							latin = new Logic::TextFont();
							latin->m_name = L"a:latin";
							latin->fromPPTY(pReader);
						}break;
						case 4:
						{
							ea = new Logic::TextFont();
							ea->m_name = L"a:ea";
							ea->fromPPTY(pReader);
						}break;
						case 5:
						{
							cs = new Logic::TextFont();
							cs->m_name = L"a:cs";
							cs->fromPPTY(pReader);
						}break;
						case 6:
						{
							sym = new Logic::TextFont();
							sym->m_name = L"a:sym";
							sym->fromPPTY(pReader);
						}break;
						case 7:
						{
							hlinkClick = new Logic::Hyperlink(L"hlinkClick");
							hlinkClick->fromPPTY(pReader);							
						}break;
						case 8:
						{
							hlinkMouseOver = new Logic::Hyperlink(L"hlinkMouseOver");
							hlinkMouseOver->fromPPTY(pReader);
						}break;
						case 9:
						{
							rtl = new Logic::Rtl();
							rtl->fromPPTY(pReader);
						}break;
						case 10:
						{
							uFill = new Logic::UFillTx(L"a:uFill");
							uFill->fromPPTY(pReader);
						}break;
						case 11:
						{
							uFillTx = new Logic::UFillTx(L"a:uFillTx");
							uFillTx->fromPPTY(pReader);
						}break;
						case 12:
						{
							highlight = new Logic::Highlight();
							highlight->fromPPTY(pReader);
						}break;					
						default:
						{
							pReader->SkipRecord();
						}
					}
				}

				pReader->Seek(_end_rec);
			}

	//Childs
			//uLn (Underline Stroke)  §21.1.2.3.14 
			//uLnTx (Underline Follows Text)  §21.1.2.3.15 
			nullable<Ln>						ln;
			UniFill								Fill;
			EffectProperties					EffectList;
			nullable<Highlight>					highlight;
			nullable<UFillTx>					uFill;
			nullable<UFillTx>					uFillTx;
			nullable<TextFont>					latin;
			nullable<TextFont>					ea;
			nullable<TextFont>					cs;
			nullable<TextFont>					sym;
			nullable<Hyperlink>					hlinkClick;
			nullable<Hyperlink>					hlinkMouseOver;
			nullable<Rtl>						rtl;

	// Attributes
			nullable_string						altLang;
			nullable_bool						b;
			nullable_int						baseline;
			nullable_string						bmk;
			nullable_limit<Limit::TextCaps>		cap;
			nullable_bool						dirty;
			nullable_bool						err;
			nullable_bool						i;
			nullable_int						kern;
			nullable_bool						kumimoji;
			nullable_string						lang;
			nullable_bool						noProof;
			nullable_bool						normalizeH;
			nullable_bool						smtClean;
			nullable_int						smtId;
			nullable_int						spc;
			nullable_limit<Limit::TextStrike>	strike;
			nullable_int						sz;
			nullable_limit<Limit::TextUnderline> u;

			std::wstring						m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(ln.is_init())
					ln->SetParentPointer(this);
				Fill.SetParentPointer(this);
				EffectList.SetParentPointer(this);
				if(latin.is_init())
					latin->SetParentPointer(this);
				if(ea.is_init())
					ea->SetParentPointer(this);
				if(cs.is_init())
					cs->SetParentPointer(this);
				if(sym.is_init())
					sym->SetParentPointer(this);
				if(hlinkClick.is_init())
					hlinkClick->SetParentPointer(this);
				if(hlinkMouseOver.is_init())
					hlinkMouseOver->SetParentPointer(this);
				if(rtl.is_init())
					rtl->SetParentPointer(this);
				if(uFill.is_init())
					uFill->SetParentPointer(this);
				if(uFillTx.is_init())
					uFillTx->SetParentPointer(this);
				if (highlight.is_init())
					highlight->SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				kern.normalize(0, 400000);
				smtId.normalize_positive();
				spc.normalize(-400000, 400000);
				sz.normalize(10, 400000);
			}

		public:
			PPTX::Logic::UniColor GetColor()const
			{
				if (Fill.is<SolidFill>())
					return Fill.as<SolidFill>().Color;
				if (Fill.is<GradFill>())
					return Fill.as<GradFill>().GetFrontColor();

				UniColor oUniColor;
				return oUniColor;
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RUNPROPERTIES_INCLUDE_H
