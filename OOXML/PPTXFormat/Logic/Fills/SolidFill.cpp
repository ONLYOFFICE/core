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
				strName = L"w14:solidFill";
			else
				strName = m_namespace.empty() ? L"solidFill" : (m_namespace + L":solidFill");

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
		void SolidFill::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar(); // FILL_TYPE_SOLID
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1);

			Color.fromPPTY(pReader);
			pReader->Seek(_e);
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
