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
			if (_T("") == m_namespace)
				return _T("<noFill/>");
			return _T("<") + m_namespace + _T(":noFill/>");
		}
		void NoFill::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
				pWriter->WriteString(_T("<w14:noFill/>"));
			else
				pWriter->WriteString(_T("<a:noFill/>"));
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
