/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "NumFmts.h"

#include "../../XlsbFormat/Biff12_records/Fmt.h"
#include "../../XlsbFormat/Biff12_unions/ACFMT.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../XlsbFormat/Biff12_unions/FMTS.h"
#include "../../XlsbFormat/Biff12_records/BeginFmts.h"

#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Format.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CNumFmt::CNumFmt()
		{
		}
		CNumFmt::~CNumFmt()
		{
		}
		void CNumFmt::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CNumFmt::toXML() const
		{
			return _T("");
		}
		void CNumFmt::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXML2(writer, _T("numFmt"));
		}
		void CNumFmt::toXML2(NSStringUtils::CStringBuilder& writer, const wchar_t* sHeader) const
		{
			toXMLWithNS(writer, L"", sHeader, L"");
		}
		void CNumFmt::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
		{
			writer.StartNodeWithNS(node_ns, node_name);
			writer.StartAttributes();
			WritingStringNullableAttrInt(L"numFmtId", m_oNumFmtId, m_oNumFmtId->GetValue());
			WritingStringNullableAttrEncodeXmlString(L"formatCode", m_oFormatCode, *m_oFormatCode);
			WritingStringNullableAttrEncodeXmlString(L"x16r2:formatCode16", m_oFormatCode16, *m_oFormatCode16);
			WritingStringNullableAttrBool(L"sourceLinked", m_oSourceLinked);
			writer.EndAttributesAndNode();
		}
		void CNumFmt::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CNumFmt::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CNumFmt::toBin()
		{
			auto ptr(new XLSB::Fmt);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_oFormatCode.IsInit())
				ptr->stFmtCode = m_oFormatCode.get();
			else
				ptr->stFmtCode = L"";
			if(m_oNumFmtId.IsInit())
				ptr->ifmt = m_oNumFmtId->GetValue();
			else
				ptr->ifmt = 5;

			return objectPtr;
		}
		XLS::BaseObjectPtr CNumFmt::toXLS()
		{
			auto fmt = new XLS::Format;
			if(m_oNumFmtId.IsInit())
				fmt->ifmt = m_oNumFmtId->GetValue();
			if(m_oFormatCode.IsInit())
				fmt->stFormat = m_oFormatCode.get();
			return XLS::BaseObjectPtr(fmt);
		}
		EElementType CNumFmt::getType () const
		{
			return et_x_NumFmt;
		}
		void CNumFmt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("formatCode"),		m_oFormatCode )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("x16r2:formatCode16"),	m_oFormatCode16 )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("numFmtId"),		m_oNumFmtId )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("sourceLinked"),	m_oSourceLinked )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("ss:Format"),		m_oFormatCode )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CNumFmt::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::Fmt*>(obj.get());
			if(ptr != nullptr)
			{
				m_oFormatCode = ptr->stFmtCode.value();
				m_oNumFmtId   = ptr->ifmt;
			}
		}

		CNumFmts::CNumFmts()
		{
		}
		CNumFmts::~CNumFmts()
		{
		}
		void CNumFmts::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CNumFmts::toXML() const
		{
			return _T("");
		}
		void CNumFmts::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_arrItems.empty()) return;

			writer.WriteString(_T("<numFmts"));
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			writer.WriteString(_T(">"));
			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(_T("</numFmts>"));
		}
		void CNumFmts::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"numFmt" == sName)
				{
					CNumFmt* pNumFmt = new CNumFmt();
					*pNumFmt = oReader;
					m_arrItems.push_back(pNumFmt);

					if (m_arrItems.back()->m_oNumFmtId.IsInit())
					{
						m_mapNumFmtIndex.insert(std::make_pair(m_arrItems.back()->m_oNumFmtId->GetValue(), m_arrItems.size() - 1));
					}
				}
			}
		}
		void CNumFmts::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			ReadAttributes(obj);

			for(auto &fmt : obj)
			{
				XLS::BaseObjectPtr ptr = nullptr;
				if(fmt->get_type() == XLS::typeACFMT)
				{
					ptr = boost::shared_ptr<XLS::BaseObject>(static_cast<XLSB::ACFMT*>(fmt.get())->m_BrtFmt);
				}
				else if(fmt->get_type() == XLS::typeFmt)
				{
					ptr = boost::shared_ptr<XLS::BaseObject>(fmt);
				}

				m_arrItems.push_back(new CNumFmt(ptr));

				if (m_arrItems.back()->m_oNumFmtId.IsInit())
				{
					m_mapNumFmtIndex.insert(std::make_pair(m_arrItems.back()->m_oNumFmtId->GetValue(), m_arrItems.size() - 1));
				}
			}

		}
		XLS::BaseObjectPtr CNumFmts::toBin()
		{
			auto fmts(new XLSB::FMTS);
            auto beginfmt(new XLSB::BeginFmts);
            fmts->m_BrtBeginFmts = XLS::BaseObjectPtr{beginfmt};
			XLS::BaseObjectPtr objectPtr(fmts);
			std::vector<XLS::BaseObjectPtr> objectVector;
			for(auto i:m_arrItems)
			{
                fmts->m_arBrtFmt.push_back(i->toBin());
			}
            beginfmt->cfmts = fmts->m_arBrtFmt.size();
			return objectPtr;
		}
		std::vector<XLS::BaseObjectPtr> CNumFmts::toXLS()
		{
			std::vector<XLS::BaseObjectPtr> fmtVector;
			for(auto i:m_arrItems)
			{
				if(i->m_oNumFmtId.IsInit() && i->m_oNumFmtId->m_eValue < 164)
					continue;
				fmtVector.push_back(i->toXLS());
			}
			return fmtVector;
		}
		EElementType CNumFmts::getType () const
		{
			return et_x_NumFmts;
		}
		void CNumFmts::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CNumFmts::ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
			{
				m_oCount = (_INT32)obj.size();
			}

	} //Spreadsheet
} // namespace OOX
