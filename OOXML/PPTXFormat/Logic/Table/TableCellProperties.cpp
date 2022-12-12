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

#include "TableCellProperties.h"

namespace PPTX
{
	namespace Logic
	{
		TableCellProperties::TableCellProperties()
		{
			is_empty = true;
		}
		TableCellProperties& TableCellProperties::operator=(const TableCellProperties& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			LnL				= oSrc.LnL;
			LnR				= oSrc.LnR;
			LnT				= oSrc.LnT;
			LnB				= oSrc.LnB;
			LnTlToBr		= oSrc.LnTlToBr;
			LnBlToTr		= oSrc.LnBlToTr;
			cell3D			= oSrc.cell3D;
			Fill			= oSrc.Fill;

			MarL			= oSrc.MarL;
			MarR			= oSrc.MarR;
			MarT			= oSrc.MarT;
			MarB			= oSrc.MarB;
			Vert			= oSrc.Vert;
			Anchor			= oSrc.Anchor;
			AnchorCtr		= oSrc.AnchorCtr;
			HorzOverflow	= oSrc.HorzOverflow;

			is_empty		= oSrc.is_empty;

			return *this;
		}
		void TableCellProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (_T("a:lnL") == strName)
					LnL = oReader;
				else if (_T("a:lnR") == strName)
					LnR = oReader;
				else if (_T("a:lnT") == strName)
					LnT = oReader;
				else if (_T("a:lnB") == strName)
					LnB = oReader;
				else if (_T("a:cell3D") == strName)
					cell3D = oReader;
				else if (_T("a:lnBlToBr") == strName)
					LnTlToBr = oReader;
				else if (_T("a:lnBlToTr") == strName)
					LnBlToTr = oReader;
				else
					Fill.fromXML(oReader);
			}

			FillParentPointersForChilds();
		}
		void TableCellProperties::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			is_empty = (oReader.GetAttributesCount() == 0);

			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("marL"), MarL )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("marR"), MarR )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("marT"), MarT )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("marB"), MarB )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("vert"), Vert )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("anchor"), Anchor )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("anchorCtr"), AnchorCtr )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("horzOverflow"), HorzOverflow )
			WritingElement_ReadAttributes_End	( oReader )
		}
		void TableCellProperties::fromXML(XmlUtils::CXmlNode& node)
		{
			is_empty = (node.GetAttributesCount() == 0);

			LnL				= node.ReadNode(_T("a:lnL"));
			LnR				= node.ReadNode(_T("a:lnR"));
			LnT				= node.ReadNode(_T("a:lnT"));
			LnB				= node.ReadNode(_T("a:lnB"));
			LnTlToBr		= node.ReadNode(_T("a:lnTlToBr"));
			LnBlToTr		= node.ReadNode(_T("a:lnBlToTr"));
			cell3D			= node.ReadNode(_T("a:cell3D"));
			Fill.GetFillFrom(node);
	//		<xsd:element name="headers" type="CT_Headers" minOccurs="0"/>

			XmlMacroReadAttributeBase(node, L"marL", MarL);
			XmlMacroReadAttributeBase(node, L"marR", MarR);
			XmlMacroReadAttributeBase(node, L"marT", MarT);
			XmlMacroReadAttributeBase(node, L"marB", MarB);
			XmlMacroReadAttributeBase(node, L"vert", Vert);
			XmlMacroReadAttributeBase(node, L"anchor", Anchor);
			XmlMacroReadAttributeBase(node, L"anchorCtr", AnchorCtr);
			XmlMacroReadAttributeBase(node, L"horzOverflow", HorzOverflow);

			FillParentPointersForChilds();
		}
		void TableCellProperties::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:tcPr"));

			pWriter->StartAttributes();

			pWriter->WriteAttribute(_T("marL"), MarL);
			pWriter->WriteAttribute(_T("marR"), MarR);
			pWriter->WriteAttribute(_T("marT"), MarT);
			pWriter->WriteAttribute(_T("marB"), MarB);
			pWriter->WriteAttribute(_T("vert"), Vert);
			pWriter->WriteAttribute(_T("anchor"), Anchor);
			pWriter->WriteAttribute(_T("anchorCtr"), AnchorCtr);
			pWriter->WriteAttribute(_T("horzOverflow"), HorzOverflow);

			pWriter->EndAttributes();

			pWriter->Write(LnL);
			pWriter->Write(LnR);
			pWriter->Write(LnT);
			pWriter->Write(LnB);
			pWriter->Write(LnTlToBr);
			pWriter->Write(LnBlToTr);
			Fill.toXmlWriter(pWriter);
			pWriter->Write(cell3D);

			pWriter->EndNode(_T("a:tcPr"));
		}
		void TableCellProperties::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, MarL);
			pWriter->WriteInt2(1, MarT);
			pWriter->WriteInt2(2, MarR);
			pWriter->WriteInt2(3, MarB);
			pWriter->WriteBool2(4, AnchorCtr);
			pWriter->WriteLimit2(5, Vert);
			pWriter->WriteLimit2(6, Anchor);
			pWriter->WriteLimit2(7, HorzOverflow);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, LnL);
			pWriter->WriteRecord2(1, LnT);
			pWriter->WriteRecord2(2, LnR);
			pWriter->WriteRecord2(3, LnB);
			pWriter->WriteRecord2(4, LnTlToBr);
			pWriter->WriteRecord2(5, LnBlToTr);
			pWriter->WriteRecord1(6, Fill);
			pWriter->WriteRecord2(7, cell3D);
		}
		void TableCellProperties::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						MarL = pReader->GetLong();
						break;
					}
					case 1:
					{
						MarT = pReader->GetLong();
						break;
					}
					case 2:
					{
						MarR = pReader->GetLong();
						break;
					}
					case 3:
					{
						MarB = pReader->GetLong();
						break;
					}
					case 4:
					{
						AnchorCtr = pReader->GetBool();
						break;
					}
					case 5:
					{
						Vert = pReader->GetUChar();
						break;
					}
					case 6:
					{
						Anchor = pReader->GetUChar();
						break;
					}
					case 7:
					{
						HorzOverflow = pReader->GetUChar();
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
						LnL = new Ln();
						LnL->fromPPTY(pReader);
						LnL->m_name = _T("a:lnL");
						break;
					}
					case 1:
					{
						LnT = new Ln();
						LnT->fromPPTY(pReader);
						LnT->m_name = _T("a:lnT");
						break;
					}
					case 2:
					{
						LnR = new Ln();
						LnR->fromPPTY(pReader);
						LnR->m_name = _T("a:lnR");
						break;
					}
					case 3:
					{
						LnB = new Ln();
						LnB->fromPPTY(pReader);
						LnB->m_name = _T("a:lnB");
						break;
					}
					case 4:
					{
						LnTlToBr = new Ln();
						LnTlToBr->fromPPTY(pReader);
						LnTlToBr->m_name = _T("a:lnTlToBr");
						break;
					}
					case 5:
					{
						LnBlToTr = new Ln();
						LnBlToTr->fromPPTY(pReader);
						LnBlToTr->m_name = _T("a:lnBlToTr");
						break;
					}
					case 6:
					{
						Fill.fromPPTY(pReader);
						break;
					}
					case 7:
					{
						cell3D = new Logic::Cell3D();
						cell3D->fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void TableCellProperties::FillParentPointersForChilds()
		{
			is_empty = is_empty & !(LnL.IsInit() || LnR.IsInit() || LnT.IsInit() || LnB.IsInit() || LnTlToBr.IsInit() || LnBlToTr.IsInit() ||
						cell3D.IsInit() || Fill.is_init());

			if(LnL.IsInit())
				LnL->SetParentPointer(this);
			if(LnR.IsInit())
				LnR->SetParentPointer(this);
			if(LnT.IsInit())
				LnT->SetParentPointer(this);
			if(LnB.IsInit())
				LnB->SetParentPointer(this);
			if(LnTlToBr.IsInit())
				LnTlToBr->SetParentPointer(this);
			if(LnBlToTr.IsInit())
				LnBlToTr->SetParentPointer(this);

			if(cell3D.IsInit())
				cell3D->SetParentPointer(this);
			if(Fill.is_init())
				Fill.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
