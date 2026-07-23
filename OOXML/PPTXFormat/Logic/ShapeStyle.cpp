/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "ShapeStyle.h"

namespace PPTX
{
	namespace Logic
	{
		ShapeStyle::ShapeStyle(std::wstring ns)
		{
			m_namespace = ns;
		}
		ShapeStyle& ShapeStyle::operator=(const ShapeStyle& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			lnRef		= oSrc.lnRef;
			fillRef		= oSrc.fillRef;
			effectRef	= oSrc.effectRef;
			fontRef		= oSrc.fontRef;

			m_namespace	= oSrc.m_namespace;

			return *this;
		}
		OOX::EElementType ShapeStyle::getType () const
		{
			return OOX::et_p_style;
		}
		void ShapeStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("lnRef") == strName)
					lnRef.fromXML(oReader);
				else if (_T("fillRef") == strName)
					fillRef.fromXML(oReader);
				else if (_T("effectRef") == strName)
					effectRef.fromXML(oReader);
				else if (_T("fontRef") == strName)
					fontRef.fromXML(oReader);
			}
			FillParentPointersForChilds();
		}
		void ShapeStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("lnRef") == strName)
						lnRef = oNode;
					else if (_T("fillRef") == strName)
						fillRef = oNode;
					else if (_T("effectRef") == strName)
						effectRef = oNode;
					else if (_T("fontRef") == strName)
						fontRef = oNode;
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring ShapeStyle::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("xmlns:") + PPTX::g_Namespaces.p.m_strName, PPTX::g_Namespaces.p.m_strLink);
			oAttr.Write(_T("xmlns:") + PPTX::g_Namespaces.a.m_strName, PPTX::g_Namespaces.a.m_strLink);

			XmlUtils::CNodeValue oValue;
			oValue.Write(lnRef);
			oValue.Write(fillRef);
			oValue.Write(effectRef);
			oValue.Write(fontRef);

			return XmlUtils::CreateNode(m_namespace + _T(":style"), oAttr, oValue);
		}
		void ShapeStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring name_ = L"a:style";

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	name_ = L"cdr:style";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)		name_ = L"dgm:style";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)	name_ = L"dsp:style";
			else name_ = m_namespace + L":style";

			pWriter->StartNode(name_);
			pWriter->EndAttributes();

			lnRef.toXmlWriter(pWriter);
			fillRef.toXmlWriter(pWriter);
			effectRef.toXmlWriter(pWriter);
			fontRef.toXmlWriter(pWriter);

			pWriter->EndNode(name_);
		}
		void ShapeStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, lnRef);
			pWriter->WriteRecord1(1, fillRef);
			pWriter->WriteRecord1(2, effectRef);
			pWriter->WriteRecord1(3, fontRef);
		}
		void ShapeStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						lnRef.m_name = _T("a:lnRef");
						lnRef.fromPPTY(pReader);
						break;
					}
					case 1:
					{
						fillRef.m_name = _T("a:fillRef");
						fillRef.fromPPTY(pReader);
						break;
					}
					case 2:
					{
						effectRef.m_name = _T("a:effectRef");
						effectRef.fromPPTY(pReader);
						break;
					}
					case 3:
					{
						fontRef.m_name = _T("a:fontRef");
						fontRef.fromPPTY(pReader);
						break;
					}
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void ShapeStyle::FillParentPointersForChilds()
		{
			lnRef.SetParentPointer(this);
			fillRef.SetParentPointer(this);
			effectRef.SetParentPointer(this);
			fontRef.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
