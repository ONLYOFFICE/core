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

#include "Path.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType Path::getType () const
		{
			return OOX::et_a_path;
		}
		void Path::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("fillToRect") == strName)
					rect = oReader;
			}
			FillParentPointersForChilds();
		}
		void Path::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_ReadSingle    ( oReader, _T("path"), path )
			WritingElement_ReadAttributes_End	( oReader )
		}
		void Path::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"path", path);
			rect = node.ReadNodeNoNS(_T("fillToRect"));

			FillParentPointersForChilds();
		}
		std::wstring Path::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("path"), path);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(rect);

			return XmlUtils::CreateNode(_T("a:path"), oAttr, oValue);
		}
		void Path::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteLimit2(0, path);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, rect);
		}
		void Path::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
				{
					path = pReader->GetUChar();
				}
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
						rect = new Logic::Rect();
						rect->fromPPTY(pReader);
						rect->m_name = _T("a:fillToRect");
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void Path::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sNodeNamespace;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sNodeNamespace = _T("w14:");
				sAttrNamespace = sNodeNamespace;
			}
			else
				sNodeNamespace = _T("a:");
			pWriter->StartNode(sNodeNamespace + _T("path"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + _T("path"), path);
			pWriter->EndAttributes();

			pWriter->Write(rect);

			pWriter->EndNode(sNodeNamespace + _T("path"));
		}
		void Path::FillParentPointersForChilds()
		{
			if(rect.IsInit())
				rect->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
