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
#ifndef PPTX_LOGIC_LN_INCLUDE_H_
#define PPTX_LOGIC_LN_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/PenAlign.h"
#include "./../Limit/LineCap.h"
#include "./../Limit/CompoundLine.h"

#include "EffectProperties.h"
#include "UniFill.h"
#include "PrstDash.h"
#include "LineEnd.h"
#include "LineJoin.h"
#include "ExtP.h"

namespace PPTX
{
	namespace Logic
	{
		class Ln : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Ln)
			Ln()
			{
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				//m_eDashType   = OOX::Drawing::linedashtypeUnknown;

				m_name = oReader.GetName();

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while ( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (_T("bevel") == sName	||
						_T("miter") == sName  ||
						_T("round") == sName )
					{
						Join.fromXML(oReader);
					}
					else if ( _T("tailEnd") == sName )
						tailEnd = oReader;
					else if ( _T("headEnd") == sName )
						headEnd = oReader;

					else if (	_T("gradFill")	== sName ||
								_T("noFill")	== sName ||
								_T("pattFill")	== sName ||
								_T("solidFill")	== sName )
					{
						Fill.fromXML(oReader);
					}
					else if ( _T("custDash") == sName )
					{
						//custDash = oReader;
						//m_eDashType = OOX::Drawing::linedashtypeCustom;
					}
					else if ( _T("prstDash") == sName )
					{
						prstDash = oReader;
						//m_eDashType = OOX::Drawing::linedashtypePreset;
					}
					else if ( L"effectDag"	== sName	||
							  L"effectLst"	== sName)
					{
						Effects.fromXML(oReader);		
					}			
					else if ( L"extLst"		== sName )
					{
						if ( oReader.IsEmptyNode() )
							continue;

						int nParentDepth1 = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
						{
							Ext element;
							element.fromXML(oReader);
							extLst.push_back (element);
						}
					}
				}
				FillParentPointersForChilds();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();

				XmlMacroReadAttributeBase(node, L"algn", algn);
				XmlMacroReadAttributeBase(node, L"cap", cap);
				XmlMacroReadAttributeBase(node, L"cmpd", cmpd);
				XmlMacroReadAttributeBase(node, L"w", w);

				Fill.GetFillFrom(node);
				prstDash = node.ReadNodeNoNS(_T("prstDash"));
				Join.GetJoinFrom(node);
				headEnd = node.ReadNodeNoNS(_T("headEnd"));
				tailEnd = node.ReadNodeNoNS(_T("tailEnd"));

				FillParentPointersForChilds();
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("algn"), algn )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("cap"),  cap )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("cmpd"), cmpd )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w"),    w )
				WritingElement_ReadAttributes_End_No_NS( oReader )
				
				Normalize();				
			}
			virtual std::wstring toXML() const
			{
				std::wstring _name = m_name;
				if (_name.empty())
					_name = _T("a:ln");

				std::wstring sAttrNamespace;

				if (_name == L"w14:textOutline")
					sAttrNamespace = _T("w14:");

				XmlUtils::CAttribute oAttr;
				oAttr.Write				(sAttrNamespace + _T("w"),		w);
				oAttr.WriteLimitNullable(sAttrNamespace + _T("cap"),	cap);
				oAttr.WriteLimitNullable(sAttrNamespace + _T("cmpd"),	cmpd);
				oAttr.WriteLimitNullable(sAttrNamespace + _T("algn"),	algn);

				XmlUtils::CNodeValue oValue;
				oValue.Write(Fill);
				oValue.WriteNullable(prstDash);
				oValue.Write(Join);
				oValue.WriteNullable(headEnd);
				oValue.WriteNullable(tailEnd);

				return XmlUtils::CreateNode(_name, oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				std::wstring _name = m_name;
				if (_name.empty())
					_name = _T("a:ln");
				
				std::wstring sAttrNamespace;
				if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				{
					_name = _T("w14:textOutline");
					sAttrNamespace = _T("w14:");
				}

				pWriter->StartNode(_name);			

				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + _T("w"), w);
				pWriter->WriteAttribute(sAttrNamespace + _T("cap"), cap);
				pWriter->WriteAttribute(sAttrNamespace + _T("cmpd"), cmpd);
				pWriter->WriteAttribute(sAttrNamespace + _T("algn"), algn);
				pWriter->EndAttributes();

				Fill.toXmlWriter(pWriter);
				pWriter->Write(prstDash);
				Join.toXmlWriter(pWriter);
				pWriter->Write(headEnd);
				pWriter->Write(tailEnd);

				pWriter->EndNode(_name);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, algn);
				pWriter->WriteLimit2(1, cap);
				pWriter->WriteLimit2(2, cmpd);
				pWriter->WriteInt2(3, w);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, Fill);
				pWriter->WriteRecord2(1, prstDash);
				pWriter->WriteRecord1(2, Join);
				pWriter->WriteRecord2(3, headEnd);
				pWriter->WriteRecord2(4, tailEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				m_name = _T("a:ln");

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
							algn = new Limit::PenAlign();
							algn->SetBYTECode(pReader->GetUChar());
							break;
						}
						case 1:
						{
							cap = new Limit::LineCap();
							cap->SetBYTECode(pReader->GetUChar());
							break;
						}
						case 2:
						{
							cmpd = new Limit::CompoundLine();
							cmpd->SetBYTECode(pReader->GetUChar());
							break;
						}
						case 3:
						{
							w = pReader->GetLong();
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
							Fill.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							prstDash = new Logic::PrstDash();
							prstDash->fromPPTY(pReader);
							break;
						}
						case 2:
						{
							Join.fromPPTY(pReader);
							break;
						}
						case 3:
						{
							headEnd = new Logic::LineEnd();
							headEnd->m_name = _T("a:headEnd");
							headEnd->fromPPTY(pReader);
							break;
						}
						case 4:
						{
							tailEnd = new Logic::LineEnd();
							tailEnd->m_name = _T("a:tailEnd");
							tailEnd->fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

			// merge
			virtual void Merge(nullable<Ln>& line)const
			{
				if(!line.IsInit())
					line = Ln();

				if(Fill.is_init())
					line->Fill = Fill;//.fromXML(Fill.toXML());
				if(prstDash.is_init())
					line->prstDash = *prstDash;
				if(Join.is_init())
					line->Join = Join;
				if(headEnd.is_init())
					headEnd->Merge(line->headEnd);
				if(tailEnd.is_init())
					tailEnd->Merge(line->tailEnd);

				if(algn.is_init())
					line->algn = *algn;
				if(cap.is_init())
					line->cap = *cap;
				if(cmpd.is_init())
					line->cmpd = *cmpd;
				if(w.is_init())
					line->w = *w;
			}

			virtual void Merge(Ln& line)const
			{
				if(Fill.is_init())
					line.Fill = Fill;//.fromXML(Fill.toXML());
				if(prstDash.is_init())
					line.prstDash = *prstDash;
				if(Join.is_init())
					line.Join = Join;
				if(headEnd.is_init())
					headEnd->Merge(line.headEnd);
				if(tailEnd.is_init())
					tailEnd->Merge(line.tailEnd);

				if(algn.is_init())
					line.algn = *algn;
				if(cap.is_init())
					line.cap = *cap;
				if(cmpd.is_init())
					line.cmpd = *cmpd;
				if(w.is_init())
					line.w = *w;
			}

		public:
//			OOX::Drawing::ELineDashType	m_eDashType;   // Тип штриха

			UniFill						Fill;
			EffectProperties			Effects;

			std::vector<Ext>			extLst;

			nullable<PrstDash>			prstDash;
			//custDash (Custom Dash)  ยง20.1.8.21 
			LineJoin					Join;
			nullable<LineEnd>			headEnd;
			nullable<LineEnd>			tailEnd;

			nullable_limit<Limit::PenAlign>		algn;
			nullable_limit<Limit::LineCap>		cap;
			nullable_limit<Limit::CompoundLine> cmpd;
			nullable_int						w;

			std::wstring m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Fill.SetParentPointer(this);
				Join.SetParentPointer(this);
				if(prstDash.IsInit())
					prstDash->SetParentPointer(this);
				if(headEnd.IsInit())
					headEnd->SetParentPointer(this);
				if(tailEnd.IsInit())
					tailEnd->SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				w.normalize(0, 20116800);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LN_INCLUDE_H_
