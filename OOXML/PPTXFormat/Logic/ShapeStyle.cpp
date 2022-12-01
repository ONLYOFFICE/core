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

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

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
