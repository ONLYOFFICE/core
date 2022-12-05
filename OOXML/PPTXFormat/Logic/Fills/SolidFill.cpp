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

#include "SolidFill.h"

namespace PPTX
{
	namespace Logic
	{
		SolidFill::SolidFill(std::wstring ns)
		{
			m_namespace = ns;
		}
		SolidFill& SolidFill::operator=(const SolidFill& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			Color		= oSrc.Color;
			m_namespace	= oSrc.m_namespace;

			return *this;
		}
		void SolidFill::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = oReader.GetName();

				Color.fromXML(oReader);
			}
			FillParentPointersForChilds();
		}
		OOX::EElementType SolidFill::getType () const
		{
			return OOX::et_a_solidFill;
		}
		void SolidFill::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());
			Color.GetColorFrom(node);
			FillParentPointersForChilds();
		}
		std::wstring SolidFill::toXML() const
		{
			std::wstring strName = (_T("") == m_namespace) ? _T("solidFill") : (m_namespace + _T(":solidFill"));

			XmlUtils::CNodeValue oValue;
			oValue.Write(Color);

			return XmlUtils::CreateNode(strName, oValue);
		}
		void SolidFill::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring strName;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				strName = _T("w14:solidFill");
			else
				strName = (_T("") == m_namespace) ? _T("solidFill") : (m_namespace + _T(":solidFill"));

			pWriter->StartNode(strName);
			pWriter->EndAttributes();

			if (Color.is_init())
				Color.toXmlWriter(pWriter);

			pWriter->EndNode(strName);
		}
		void SolidFill::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(FILL_TYPE_SOLID);

			pWriter->WriteRecord1(0, Color);

			pWriter->EndRecord();
		}
		void SolidFill::Merge(SolidFill& fill)const
		{
			if(Color.is_init())
				fill.Color = Color;
		}
		void SolidFill::FillParentPointersForChilds()
		{
			Color.SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
