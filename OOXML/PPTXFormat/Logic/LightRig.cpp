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

#include "LightRig.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType LightRig::getType() const
		{
			return OOX::et_a_lightRig;
		}
		void LightRig::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (strName == L"a:rot")
				{
					rot = oReader;
					break;
				}
			}
			FillParentPointersForChilds();
		}
		void LightRig::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("dir"), dir)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rig"), rig)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void LightRig::fromXML(XmlUtils::CXmlNode& node)
		{
			dir = node.GetAttribute(L"dir");
			rig = node.GetAttribute(L"rig");

			rot = node.ReadNode(L"a:rot");
			FillParentPointersForChilds();
		}
		std::wstring LightRig::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("rig"), rig.get());
			oAttr.Write(_T("dir"), dir.get());

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(rot);

			return XmlUtils::CreateNode(_T("a:lightRig"), oAttr, oValue);
		}
		void LightRig::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sNodeNamespace;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sNodeNamespace = L"w14:";
				sAttrNamespace = sNodeNamespace;
			}
			else
				sNodeNamespace = L"a:";

			pWriter->StartNode(sNodeNamespace + L"lightRig");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + L"rig", rig.get());
			pWriter->WriteAttribute(sAttrNamespace + L"dir", dir.get());
			pWriter->EndAttributes();

			pWriter->Write(rot);

			pWriter->EndNode(sNodeNamespace + L"lightRig");
		}
		void LightRig::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit1(0, dir);
			pWriter->WriteLimit1(1, rig);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, rot);
		}
		void LightRig::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)		dir.SetBYTECode(pReader->GetUChar());
				else if (1 == _at)	rig.SetBYTECode(pReader->GetUChar());
				else
					break;
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						rot = new Logic::Rot();
						rot->fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void LightRig::FillParentPointersForChilds()
		{
			if(rot.IsInit())
				rot->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
