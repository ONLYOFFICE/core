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

#include "NoFill.h"

namespace PPTX
{
	namespace Logic
	{
		NoFill::NoFill(std::wstring ns)
		{
			m_namespace = ns;
		}
		NoFill& NoFill::operator=(const NoFill& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			m_namespace = oSrc.m_namespace;
			return *this;
		}
		void NoFill::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());
		}
		OOX::EElementType NoFill::getType () const
		{
			return OOX::et_a_noFill;
		}
		void NoFill::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}
		void NoFill::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());
		}
		std::wstring NoFill::toXML() const
		{
			if (m_namespace.empty())
				return _T("<noFill/>");
			return L"<" + m_namespace + L":noFill/>";
		}
		void NoFill::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring strName;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				strName = L"w14:noFill";
			else
				strName = m_namespace.empty() ? L"noFill" : (m_namespace + L":noFill");

			pWriter->WriteString(L"<" + strName  + L"/>");
		}
		void NoFill::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(FILL_TYPE_NOFILL);
			pWriter->EndRecord();
		}
		void NoFill::FillParentPointersForChilds(){}

		GrpFill& GrpFill::operator=(const GrpFill& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			m_namespace = oSrc.m_namespace;
			return *this;
		}
		void GrpFill::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_namespace = XmlUtils::GetNamespace(oReader.GetName());
		}
		OOX::EElementType GrpFill::getType () const
		{
			return OOX::et_a_grpFill;
		}
		void GrpFill::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}
		void GrpFill::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());
		}
		std::wstring GrpFill::toXML() const
		{
			if (_T("") == m_namespace)
				return _T("<grpFill/>");
			return _T("<") + m_namespace + _T(":grpFill/>");
		}
		void GrpFill::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				pWriter->WriteString(_T("<w14:grpFill/>"));
			else
				pWriter->WriteString(_T("<a:grpFill/>"));
		}
		void GrpFill::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(FILL_TYPE_GRP);
			pWriter->EndRecord();
		}
		void GrpFill::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
