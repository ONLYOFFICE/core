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


#include "CxnSp.h"
#include "SpTree.h"
#include "./../SlideLayout.h"
#include "./../SlideMaster.h"
#include "./../Slide.h"

namespace PPTX
{
	namespace Logic
	{
		CxnSp::CxnSp(std::wstring ns)
		{
			m_namespace = ns;
		}
		CxnSp::~CxnSp()
		{
		}
		CxnSp::CxnSp(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const CxnSp& CxnSp::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		CxnSp::CxnSp(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
		}
		const CxnSp& CxnSp::operator =(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
			return *this;
		}
		void CxnSp::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			ReadAttributes(oReader);

			if ( oReader.IsEmptyNode() )
				return;
					
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("nvCxnSpPr") == strName)
					nvCxnSpPr = oReader;
				else if (_T("spPr") == strName)
					spPr = oReader;
				else if (_T("style") == strName)
					style = oReader;
			}

			FillParentPointersForChilds();
		}

		void CxnSp::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			XmlMacroReadAttributeBase(node, L"macro", macro);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("nvCxnSpPr") == strName)
						nvCxnSpPr = oNode;
					else if (_T("spPr") == strName)
						spPr = oNode;
					else if (_T("style") == strName)
						style = oNode;
				}
			}

			FillParentPointersForChilds();
		}

		std::wstring CxnSp::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"macro", macro);

			XmlUtils::CNodeValue oValue;
			oValue.Write(nvCxnSpPr);
			oValue.Write(spPr);
			oValue.WriteNullable(style);

			return XmlUtils::CreateNode(m_namespace + L":cxnSp", oAttr, oValue);
		}

		void CxnSp::FillParentPointersForChilds()
		{
			nvCxnSpPr.SetParentPointer(this);
			spPr.SetParentPointer(this);
			if(style.is_init())
				style->SetParentPointer(this);
		}
		
		void CxnSp::GetRect(Aggplus::RECT& pRect)const
		{
			pRect.bottom = 0;
			pRect.left = 0;
			pRect.right = 0;
			pRect.top = 0;

			if(spPr.xfrm.IsInit())
			{
				pRect.left		= spPr.xfrm->offX.get();
				pRect.top		= spPr.xfrm->offY.get();
				pRect.right		= pRect.left + spPr.xfrm->extX.get_value_or(0);
				pRect.bottom	= pRect.top + spPr.xfrm->extY.get_value_or(0);
			}
			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(pRect);
		}

		DWORD CxnSp::GetLine(Ln& line)const
		{
			DWORD BGRA = 0;
			line.SetParentFilePointer(parentFile);

			if(style.is_init())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);

				BGRA = style->lnRef.Color.GetBGRA();
			}

			if(spPr.ln.is_init())
				spPr.ln->Merge(line);
			return BGRA;
		}

		DWORD CxnSp::GetFill(UniFill& fill)const
		{
			DWORD BGRA = 0;
			fill.SetParentFilePointer(parentFile);

			if (style.IsInit())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);

				BGRA = style->fillRef.Color.GetBGRA();
			}

			if(spPr.Fill.is_init())
				spPr.Fill.Merge(fill);
			return BGRA;
		}
	} // namespace Logic
} // namespace PPTX