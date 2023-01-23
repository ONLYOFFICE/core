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

#include "Camera.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType Camera::getType() const
		{
			return OOX::et_a_camera;
		}
		void Camera::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		void Camera::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("prst"), prst)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fov"), fov)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("zoom"), zoom)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void Camera::fromXML(XmlUtils::CXmlNode& node)
		{
			prst	= node.GetAttribute(_T("prst"));
			XmlMacroReadAttributeBase(node, L"fov", fov);
			XmlMacroReadAttributeBase(node, L"zoom", zoom);

			std::wstring sRotNodeName = _T("a:rot");
			rot		= node.ReadNode(sRotNodeName);
			FillParentPointersForChilds();
		}
		std::wstring Camera::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("prst"), prst.get());
			oAttr.Write(_T("fov"), fov);
			oAttr.Write(_T("zoom"), zoom);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(rot);

			return XmlUtils::CreateNode(_T("a:camera"), oAttr, oValue);
		}
		void Camera::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
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

			pWriter->StartNode(sNodeNamespace + L"camera");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + L"prst", prst.get());
			pWriter->WriteAttribute(sAttrNamespace + L"fov", fov);
			pWriter->WriteAttribute(sAttrNamespace + L"zoom", zoom);
			pWriter->EndAttributes();

			pWriter->Write(rot);

			pWriter->EndNode(sNodeNamespace + L"camera");
		}
		void Camera::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit1(0, prst);
			pWriter->WriteInt2(1, fov);
			pWriter->WriteInt2(2, zoom);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, rot);
		}
		void Camera::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)		prst.SetBYTECode(pReader->GetUChar());
				else if (1 == _at)	fov	= pReader->GetLong();
				else if (2 == _at)	zoom	= pReader->GetLong();
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
		void Camera::FillParentPointersForChilds()
		{
			if(rot.IsInit())
				rot->SetParentPointer(this);
		}
		void Camera::Normalize()
		{
			fov.normalize(0, 10800000);
			zoom.normalize_positive();
		}
	} // namespace Logic
} // namespace PPTX
