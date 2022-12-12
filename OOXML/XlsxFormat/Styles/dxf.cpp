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

#include "dxf.h"

#include "../../XlsbFormat/Biff12_unions/DXF.h"
#include "../../XlsbFormat/Biff12_unions/FRTDXF.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"
namespace OOX
{
	namespace Spreadsheet
	{
		CDxf::CDxf()
		{
		}
		CDxf::~CDxf()
		{
		}
		void CDxf::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDxf::toXML() const
		{
			return _T("");
		}
		void CDxf::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			toXML2(writer, L"dxf");
		}
		void CDxf::toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring &node_name) const
		{
			toXMLWithNS(writer, L"", node_name, L"");
		}
		void CDxf::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
		{
			writer.StartNodeWithNS(node_ns, node_name);
			writer.StartAttributes();
			writer.EndAttributes();
			if(m_oFont.IsInit())
				m_oFont->toXMLWithNS(writer, child_ns, L"font", child_ns);
			if(m_oNumFmt.IsInit())
				m_oNumFmt->toXMLWithNS(writer, child_ns, L"numFmt", child_ns);
			if(m_oFill.IsInit())
				m_oFill->toXMLWithNS(writer, child_ns, L"fill", child_ns);
			if(m_oAlignment.IsInit())
				m_oAlignment->toXMLWithNS(writer, child_ns, L"alignment", child_ns);
			if(m_oBorder.IsInit())
				m_oBorder->toXMLWithNS(writer, child_ns, L"border", child_ns);
			if(m_oProtection.IsInit())
				m_oProtection->toXMLWithNS(writer, child_ns, L"protection", child_ns);
			writer.EndNodeWithNS(node_ns, node_name);
		}
		void CDxf::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("alignment") == sName )
					m_oAlignment = oReader;
				else if ( _T("border") == sName )
					m_oBorder = oReader;
				else if ( _T("fill") == sName )
					m_oFill = oReader;
				else if ( _T("font") == sName )
					m_oFont = oReader;
				else if ( _T("numFmt") == sName )
					m_oNumFmt = oReader;
				else if ( _T("protection") == sName )
					m_oProtection = oReader;
			}
		}
		void CDxf::fromBin(XLS::BaseObjectPtr& obj)
		{
			if(obj->get_type() == XLS::typeDXF)
			{
				auto ptrDXF = static_cast<XLSB::DXF*>(obj.get());

				if(ptrDXF != nullptr)
				{
					std::wstringstream strm;
					ptrDXF->serialize(strm);

					XmlUtils::CXmlLiteReader oReader;
					std::wstring str = strm.str();

					if ( !oReader.FromString(str))
						return;

					int nCurDepth = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nCurDepth ) )
					{
						std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

						if ( _T("alignment") == sName )
							m_oAlignment = oReader;
						else if ( _T("border") == sName )
							m_oBorder = oReader;
						else if ( _T("fill") == sName )
							m_oFill = oReader;
						else if ( _T("font") == sName )
							m_oFont = oReader;
						else if ( _T("numFmt") == sName )
							m_oNumFmt = oReader;
						else if ( _T("protection") == sName )
							m_oProtection = oReader;
					}
				}
			}

			else if(obj->get_type() == XLS::typeDXF14)
			{
				auto ptrDXF14 = static_cast<XLSB::DXF14*>(obj.get());
				if(ptrDXF14 != nullptr)
				{
					std::wstringstream strm;
					ptrDXF14->serialize(strm);

					XmlUtils::CXmlLiteReader oReader;
					std::wstring str = strm.str();

					if ( !oReader.FromString(str))
						return;

					int nCurDepth = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nCurDepth ) )
					{
						std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

						if ( _T("alignment") == sName )
							m_oAlignment = oReader;
						else if ( _T("border") == sName )
							m_oBorder = oReader;
						else if ( _T("fill") == sName )
							m_oFill = oReader;
						else if ( _T("font") == sName )
							m_oFont = oReader;
						else if ( _T("numFmt") == sName )
							m_oNumFmt = oReader;
						else if ( _T("protection") == sName )
							m_oProtection = oReader;
					}
				}
			}

		}
		EElementType CDxf::getType () const
		{
			return et_x_Dxf;
		}
		void CDxf::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

		CDxfs::CDxfs()
		{
		}
		CDxfs::~CDxfs()
		{
		}
		void CDxfs::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CDxfs::toXML() const
		{
			return _T("");
		}
		void CDxfs::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<dxfs"));
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			writer.WriteString(_T(">"));

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(_T("</dxfs>"));
		}
		void CDxfs::toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
		{
			writer.WriteString(_T("<"));
			writer.WriteString(sName);
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			writer.WriteString(_T(">"));

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString(L"</");
			writer.WriteString(sName);
			writer.WriteString(L">");
		}
		void CDxfs::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("dxf") == sName )
					m_arrItems.push_back( new CDxf(oReader));
			}
		}
		EElementType CDxfs::getType () const
		{
			return et_x_Dxfs;
		}
		void CDxfs::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			ReadAttributes(obj);

			for(auto &dxf : obj)
			{
				if(dxf->get_type() == XLS::typeuDXF)
				{
					auto ptruDXF = static_cast<XLSB::uDXF*>(dxf.get());

					if(ptruDXF && ptruDXF->m_BrtDXF != nullptr)
					{
						m_arrItems.push_back(new CDxf(ptruDXF->m_BrtDXF));
					}
					if(ptruDXF && ptruDXF->m_BrtFRTDXF != nullptr)
					{
						auto ptr = static_cast<XLSB::FRTDXF*>(ptruDXF->m_BrtFRTDXF.get())->m_BrtDXF;
						m_arrItems.push_back(new CDxf(ptr));
					}
				}
				else
					m_arrItems.push_back(new CDxf(dxf));
			}
		}
		void CDxfs::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"), m_oCount )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CDxfs::ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
		{
			m_oCount = (_UINT32)obj.size();
		}

	} //Spreadsheet
} // namespace OOX
