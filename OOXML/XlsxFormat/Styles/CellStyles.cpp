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

#include "CellStyles.h"
#include "../../XlsbFormat/Biff12_records/Style.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CCellStyle::CCellStyle()
		{
		}
		CCellStyle::~CCellStyle()
		{
		}
		void CCellStyle::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCellStyle::toXML() const
		{
			return _T("");
		}
		void CCellStyle::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<cellStyle"));
			WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
			WritingStringNullableAttrInt(L"xfId", m_oXfId, m_oXfId->GetValue());
			WritingStringNullableAttrInt(L"builtinId", m_oBuiltinId, m_oBuiltinId->GetValue());
			writer.WriteString(_T("/>"));
		}
		void CCellStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CCellStyle::fromBin(XLS::BaseObjectPtr& obj)
		 {
			 ReadAttributes(obj);
		 }
		EElementType CCellStyle::getType () const
		{
			return et_x_CellStyle;
		}
		void CCellStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("builtinId"),      m_oBuiltinId )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("customBuiltin"),      m_oCustomBuiltin )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("hidden"),      m_oHidden )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("iLevel"),      m_oILevel )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),      m_oName )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("xfId"),      m_oXfId )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CCellStyle::ReadAttributes(XLS::BaseObjectPtr& obj)
		 {
			 auto ptr = static_cast<XLSB::Style*>(obj.get());
			 if(ptr != nullptr)
			 {
				 m_oBuiltinId     = ptr->fBuiltIn;
				 m_oCustomBuiltin = ptr->fCustom;
				 m_oHidden        = ptr->fHidden;
				 m_oILevel        = ptr->iLevel;
				 m_oName          = ptr->stName;
				 m_oXfId          = ptr->ixf;
			 }
		 }

		CCellStyles::CCellStyles()
		{
		}
		CCellStyles::~CCellStyles()
		{
		}
		void CCellStyles::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCellStyles::toXML() const
		{
			return _T("");
		}
		void CCellStyles::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<cellStyles"));
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			writer.WriteString(_T(">"));

			 for ( size_t i = 0; i < m_arrItems.size(); ++i)
			 {
				 if ( m_arrItems[i] )
				 {
					 m_arrItems[i]->toXML(writer);
				 }
			 }

			writer.WriteString(_T("</cellStyles>"));
		}
		void CCellStyles::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("cellStyle") == sName )
					m_arrItems.push_back( new CCellStyle( oReader ));
			}
		}
		void CCellStyles::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		 {
			 ReadAttributes(obj);

			 for(auto &style : obj)
			 {
				 CCellStyle *pXfs = new CCellStyle(style);
				 m_arrItems.push_back(pXfs);
			 }
		 }
		EElementType CCellStyles::getType () const
		{
			return et_x_CellStyles;
		}
		void CCellStyles::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("count"),      m_oCount )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CCellStyles::ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
			{
				m_oCount = (_UINT32)obj.size();
			}

	} //Spreadsheet
} // namespace OOX
