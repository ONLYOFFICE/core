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

#include "BodyPr.h"

namespace PPTX
{
	namespace Logic
	{
		BodyPr::BodyPr(std::wstring ns)
		{
			m_namespace = ns;
		}
		void BodyPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (L"a:prstTxWarp" == strName)
					prstTxWarp = oReader;
				else if (L"a:scene3d" == strName)
					scene3d = oReader;
				else if (L"a:sp3d" == strName)
					sp3d = oReader;
				else if (strName == L"a:noAutofit" || strName == L"a:spAutoFit" || strName == L"a:normAutofit")
					Fit.fromXML(oReader);
				else if (L"a:flatTx" == strName)
				{
					//oNode.ReadAttributeBase(L"z", flatTx);
				}

			}
			FillParentPointersForChilds();
		}
		void BodyPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("anchor"), anchor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("anchorCtr"), anchorCtr )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("bIns"), bIns )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("compatLnSpc"), compatLnSpc )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("forceAA"), forceAA )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("fromWordArt"), fromWordArt )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("horzOverflow"), horzOverflow )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("lIns"), lIns )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("numCol"), numCol )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("rIns"), rIns )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("rot"), rot )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("rtlCol"), rtlCol )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("spcCol"), spcCol )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("spcFirstLastPara"), spcFirstLastPara )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("tIns"), tIns )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("upright"), upright )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("vert"), vert )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("vertOverflow"), vertOverflow )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrap"), wrap )
			WritingElement_ReadAttributes_End	( oReader )

			Normalize();
		}
		void BodyPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"anchor", anchor);
			XmlMacroReadAttributeBase(node, L"anchorCtr", anchorCtr);
			XmlMacroReadAttributeBase(node, L"bIns", bIns);
			XmlMacroReadAttributeBase(node, L"compatLnSpc", compatLnSpc);
			XmlMacroReadAttributeBase(node, L"forceAA", forceAA);
			XmlMacroReadAttributeBase(node, L"fromWordArt", fromWordArt);
			XmlMacroReadAttributeBase(node, L"horzOverflow", horzOverflow);
			XmlMacroReadAttributeBase(node, L"lIns", lIns);
			XmlMacroReadAttributeBase(node, L"numCol", numCol);
			XmlMacroReadAttributeBase(node, L"rIns", rIns);
			XmlMacroReadAttributeBase(node, L"rot", rot);
			XmlMacroReadAttributeBase(node, L"rtlCol", rtlCol);
			XmlMacroReadAttributeBase(node, L"spcCol", spcCol);
			XmlMacroReadAttributeBase(node, L"spcFirstLastPara", spcFirstLastPara);
			XmlMacroReadAttributeBase(node, L"tIns", tIns);
			XmlMacroReadAttributeBase(node, L"upright", upright);
			XmlMacroReadAttributeBase(node, L"vert", vert);
			XmlMacroReadAttributeBase(node, L"vertOverflow", vertOverflow);
			XmlMacroReadAttributeBase(node, L"wrap", wrap);

			Fit.GetTextFitFrom(node);
			prstTxWarp	= node.ReadNode(_T("a:prstTxWarp"));
			scene3d		= node.ReadNode(_T("a:scene3d"));

			XmlUtils::CXmlNode oNode = node.ReadNodeNoNS(_T("flatTx"));
			XmlMacroReadAttributeBase(oNode, L"z", flatTx);

			sp3d		= node.ReadNode(_T("a:sp3d"));

			Normalize();
			FillParentPointersForChilds();
		}
		std::wstring BodyPr::toXML() const
		{
			if (m_namespace.empty()) m_namespace = L"a";

			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("rot"), rot);
			oAttr.Write(_T("spcFirstLastPara"), spcFirstLastPara);
			oAttr.WriteLimitNullable(_T("vertOverflow"), vertOverflow);
			oAttr.WriteLimitNullable(_T("horzOverflow"), horzOverflow);
			oAttr.WriteLimitNullable(_T("vert"), vert);
			oAttr.WriteLimitNullable(_T("wrap"), wrap);
			oAttr.Write(_T("lIns"), lIns);
			oAttr.Write(_T("tIns"), tIns);
			oAttr.Write(_T("rIns"), rIns);
			oAttr.Write(_T("bIns"), bIns);
			oAttr.Write(_T("numCol"), numCol);
			oAttr.Write(_T("spcCol"), spcCol);
			oAttr.Write(_T("rtlCol"), rtlCol);
			oAttr.Write(_T("fromWordArt"), fromWordArt);
			oAttr.WriteLimitNullable(_T("anchor"), anchor);
			oAttr.Write(_T("anchorCtr"), anchorCtr);
			oAttr.Write(_T("forceAA"), forceAA);
			oAttr.Write(_T("upright"), upright);
			oAttr.Write(_T("compatLnSpc"), compatLnSpc);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(prstTxWarp);
			oValue.Write(Fit);
			oValue.WriteNullable(scene3d);
			oValue.WriteNullable(sp3d);
			if (flatTx.IsInit())
			{
				XmlUtils::CAttribute oAttr2;
				oAttr2.Write(_T("z"), flatTx);

				oValue.m_strValue += XmlUtils::CreateNode(m_namespace + _T(":flatTx"), oAttr2);
			}

			return XmlUtils::CreateNode(m_namespace + _T(":bodyPr"), oAttr, oValue);
		}
		void BodyPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_namespace.empty()) m_namespace = L"a";

			pWriter->StartNode(m_namespace + _T(":bodyPr"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("rot"), rot);
			pWriter->WriteAttribute(_T("spcFirstLastPara"), spcFirstLastPara);
			pWriter->WriteAttribute(_T("vertOverflow"), vertOverflow);
			pWriter->WriteAttribute(_T("horzOverflow"), horzOverflow);
			pWriter->WriteAttribute(_T("vert"), vert);
			pWriter->WriteAttribute(_T("wrap"), wrap);
			pWriter->WriteAttribute(_T("lIns"), lIns);
			pWriter->WriteAttribute(_T("tIns"), tIns);
			pWriter->WriteAttribute(_T("rIns"), rIns);
			pWriter->WriteAttribute(_T("bIns"), bIns);
			pWriter->WriteAttribute(_T("numCol"), numCol);
			pWriter->WriteAttribute(_T("spcCol"), spcCol);
			pWriter->WriteAttribute(_T("rtlCol"), rtlCol);
			pWriter->WriteAttribute(_T("fromWordArt"), fromWordArt);
			pWriter->WriteAttribute(_T("anchor"), anchor);
			pWriter->WriteAttribute(_T("anchorCtr"), anchorCtr);
			pWriter->WriteAttribute(_T("forceAA"), forceAA);
			pWriter->WriteAttribute(_T("upright"), upright);
			pWriter->WriteAttribute(_T("compatLnSpc"), compatLnSpc);
			pWriter->EndAttributes();

			pWriter->Write(prstTxWarp);
			Fit.toXmlWriter(pWriter);
			pWriter->Write(scene3d);
			pWriter->Write(sp3d);
			if (flatTx.IsInit())
			{
				pWriter->StartNode(m_namespace + _T(":flatTx"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("z"), flatTx);
				pWriter->EndAttributes();
				pWriter->EndNode(m_namespace + _T(":flatTx"));
			}

			pWriter->EndNode(m_namespace + _T(":bodyPr"));
		}
		void BodyPr::Merge(nullable<BodyPr>& bodyPr)const
		{
			if (!bodyPr.IsInit())
			{
				bodyPr = new PPTX::Logic::BodyPr();
			}

			if(Fit.type != TextFit::FitEmpty)
				Fit.Merge(bodyPr->Fit);
	/*
			nullable_property<PrstTxWarp> prstTxWarp;
			nullable_property<Scene3d> scene3d;
			nullable_property<int> flatTx;
			nullable_property<Sp3d> sp3d;
	*/
			if(anchor.IsInit())
				bodyPr->anchor = *anchor;
			if(anchorCtr.IsInit())
				bodyPr->anchorCtr = *anchorCtr;
			if(bIns.IsInit())
				bodyPr->bIns = *bIns;
			if(compatLnSpc.IsInit())
				bodyPr->compatLnSpc = *compatLnSpc;
			if(forceAA.IsInit())
				bodyPr->forceAA = *forceAA;
			if(fromWordArt.IsInit())
				bodyPr->fromWordArt = *fromWordArt;
			if(horzOverflow.IsInit())
				bodyPr->horzOverflow = *horzOverflow;
			if(lIns.IsInit())
				bodyPr->lIns = *lIns;
			if(numCol.IsInit())
				bodyPr->numCol = *numCol;
			if(rIns.IsInit())
				bodyPr->rIns = *rIns;
			if(rot.IsInit())
				bodyPr->rot = *rot;
			if(rtlCol.IsInit())
				bodyPr->rtlCol = *rtlCol;
			if(spcCol.IsInit())
				bodyPr->spcCol = *spcCol;
			if(spcFirstLastPara.IsInit())
				bodyPr->spcFirstLastPara = *spcFirstLastPara;
			if(tIns.IsInit())
				bodyPr->tIns = *tIns;
			if(upright.IsInit())
				bodyPr->upright = *upright;
			if(vert.IsInit())
				bodyPr->vert = *vert;
			if(vertOverflow.IsInit())
				bodyPr->vertOverflow = *vertOverflow;
			if(wrap.IsInit())
				bodyPr->wrap = *wrap;
		}
		void BodyPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, flatTx);
			pWriter->WriteLimit2(1, anchor);
			pWriter->WriteBool2(2, anchorCtr);
			pWriter->WriteInt2(3, bIns);
			pWriter->WriteBool2(4, compatLnSpc);
			pWriter->WriteBool2(5, forceAA);
			pWriter->WriteBool2(6, fromWordArt);
			pWriter->WriteLimit2(7, horzOverflow);
			pWriter->WriteInt2(8, lIns);
			pWriter->WriteInt2(9, numCol);
			pWriter->WriteInt2(10, rIns);
			pWriter->WriteInt2(11, rot);
			pWriter->WriteBool2(12, rtlCol);
			pWriter->WriteInt2(13, spcCol);
			pWriter->WriteBool2(14, spcFirstLastPara);
			pWriter->WriteInt2(15, tIns);
			pWriter->WriteBool2(16, upright);
			pWriter->WriteLimit2(17, vert);
			pWriter->WriteLimit2(18, vertOverflow);
			pWriter->WriteLimit2(19, wrap);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, prstTxWarp);
			pWriter->WriteRecord1(1, Fit);
			pWriter->WriteRecord2(2, scene3d);
			pWriter->WriteRecord2(3, sp3d);
		}
		void BodyPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						flatTx = pReader->GetLong();
						break;
					}
					case 1:
					{
						anchor = pReader->GetUChar();
						break;
					}
					case 2:
					{
						anchorCtr = pReader->GetBool();
						break;
					}
					case 3:
					{
						bIns = pReader->GetLong();
						break;
					}
					case 4:
					{
						compatLnSpc = pReader->GetBool();
						break;
					}
					case 5:
					{
						forceAA = pReader->GetBool();
						break;
					}
					case 6:
					{
						fromWordArt = pReader->GetBool();
						break;
					}
					case 7:
					{
						horzOverflow = pReader->GetUChar();
						break;
					}
					case 8:
					{
						lIns = pReader->GetLong();
						break;
					}
					case 9:
					{
						numCol = pReader->GetLong();
						break;
					}
					case 10:
					{
						rIns = pReader->GetLong();
						break;
					}
					case 11:
					{
						rot = pReader->GetLong();
						break;
					}
					case 12:
					{
						rtlCol = pReader->GetBool();
						break;
					}
					case 13:
					{
						spcCol = pReader->GetLong();
						break;
					}
					case 14:
					{
						spcFirstLastPara = pReader->GetBool();
						break;
					}
					case 15:
					{
						tIns = pReader->GetLong();
						break;
					}
					case 16:
					{
						upright = pReader->GetBool();
						break;
					}
					case 17:
					{
						vert = pReader->GetUChar();
						break;
					}
					case 18:
					{
						vertOverflow = pReader->GetUChar();
						break;
					}
					case 19:
					{
						wrap = pReader->GetUChar();
						break;
					}
					default:
						break;
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE rec = pReader->GetUChar();

				switch (rec)
				{
				case 0:
				{
					prstTxWarp = new PPTX::Logic::PrstTxWarp();
					prstTxWarp->fromPPTY(pReader);
					break;
				}
				case 1:
				{
					Fit.fromPPTY(pReader);
					break;
				}
				case 2:
				{
					scene3d = new PPTX::Logic::Scene3d();
					scene3d->fromPPTY(pReader);
					break;
				}
				case 3:
				{
					sp3d = new PPTX::Logic::Sp3d();
					sp3d->fromPPTY(pReader);
					break;
				}
				default:
				{
					// пока никаких настроек градиента нет
					pReader->SkipRecord();
				}
				}
			}
			pReader->Seek(_end_rec);
		}
		void BodyPr::FillParentPointersForChilds()
		{
			if(prstTxWarp.IsInit())
				prstTxWarp->SetParentPointer(this);
			Fit.SetParentPointer(this);
			if(scene3d.IsInit())
				scene3d->SetParentPointer(this);
		}
		void BodyPr::Normalize()
		{
			numCol.normalize(1, 16);
			spcCol.normalize_positive();
		}

		LinkedTxbx::LinkedTxbx(std::wstring ns)
		{
			m_namespace = ns;
		}
		void LinkedTxbx::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring strName = oReader.GetName();

				if (L"a:extLst" == strName)
				{
					if (oReader.IsEmptyNode())
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while (oReader.ReadNextSiblingNode(nParentDepth1))
					{
						Ext element;
						element.fromXML(oReader);
						extLst.push_back(element);
					}
				}
			}
			FillParentPointersForChilds();
		}
		void LinkedTxbx::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"id", id)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"seq", seq)
			WritingElement_ReadAttributes_End(oReader)
			Normalize();
		}
		void LinkedTxbx::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"id", id);
			XmlMacroReadAttributeBase(node, L"seq", seq);

			Normalize();
			FillParentPointersForChilds();
		}
		std::wstring LinkedTxbx::toXML() const
		{
			if (m_namespace.empty()) m_namespace = L"a";

			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"id", id);
			oAttr.Write(L"seq", seq);
			return XmlUtils::CreateNode(m_namespace + L":linkedTxbx", oAttr);
		}
		void LinkedTxbx::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_namespace.empty()) m_namespace = L"a";

			pWriter->StartNode(m_namespace + L":linkedTxbx");

			pWriter->StartAttributes();
				pWriter->WriteAttribute2(L"id", id);
				pWriter->WriteAttribute2(L"seq", seq);
			pWriter->EndAttributes();

			pWriter->EndNode(m_namespace + L":linkedTxbx");
		}
		void LinkedTxbx::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteUInt2(0, id);
				pWriter->WriteUInt2(1, seq);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void LinkedTxbx::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					id = pReader->GetULong();
					break;
				}
				case 1:
				{
					seq = pReader->GetULong();
					break;
				}
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void LinkedTxbx::FillParentPointersForChilds()
		{
		}
		void LinkedTxbx::Normalize()
		{
		}
	} // namespace Logic
} // namespace PPTX

