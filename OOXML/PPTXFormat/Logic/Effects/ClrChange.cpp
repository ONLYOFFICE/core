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

#include "ClrChange.h"

namespace PPTX
{
	namespace Logic
	{
		ClrChange& ClrChange::operator=(const ClrChange& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			ClrFrom	= oSrc.ClrFrom;
			ClrTo	= oSrc.ClrTo;
			useA	= oSrc.useA;
			return *this;
		}
		OOX::EElementType ClrChange::getType() const
		{
			return OOX::et_a_clrChange;
		}
		void ClrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (strName == L"a:clrTo")
					ClrTo.fromXMLParent(oReader);
				else if (strName == L"a:clrFrom")
					ClrFrom.fromXMLParent(oReader);
			}
			FillParentPointersForChilds();
		}
		void ClrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("useA"), useA)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void ClrChange::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode node1 = node.ReadNode(_T("a:clrFrom"));
			ClrFrom.GetColorFrom(node1);

			node1 = node.ReadNode(_T("a:clrTo"));
			ClrTo.GetColorFrom(node1);

			XmlMacroReadAttributeBase(node, L"useA", useA);
			FillParentPointersForChilds();
		}
		std::wstring ClrChange::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("useA"), useA);

			XmlUtils::CNodeValue oValue;
			oValue.Write(_T("a:clrFrom"), ClrFrom);
			oValue.Write(_T("a:clrTo"), ClrTo);

			return XmlUtils::CreateNode(_T("a:clrChange"), oAttr, oValue);
		}
		void ClrChange::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:clrChange");
			pWriter->EndAttributes();
				pWriter->StartNode(L"a:clrFrom");
					pWriter->EndAttributes();
					ClrFrom.toXmlWriter(pWriter);
				pWriter->EndNode(L"a:clrFrom");
				pWriter->StartNode(L"a:clrTo");
					pWriter->EndAttributes();
					ClrTo.toXmlWriter(pWriter);
				pWriter->EndNode(L"a:clrTo");
			pWriter->EndNode(L"a:clrChange");
		}
		void ClrChange::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_CLRCHANGE);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, useA);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, ClrFrom);
			pWriter->WriteRecord1(1, ClrTo);

			pWriter->EndRecord();
		}
		void ClrChange::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar();
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1);

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
						useA = pReader->GetBool(); break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						ClrFrom.fromPPTY(pReader);
					}break;
					case 1:
					{
						ClrTo.fromPPTY(pReader);
					}break;
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void ClrChange::FillParentPointersForChilds()
		{
			ClrFrom.SetParentPointer(this);
			ClrTo.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
