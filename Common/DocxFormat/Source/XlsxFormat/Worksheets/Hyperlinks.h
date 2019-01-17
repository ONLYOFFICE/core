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
#pragma once
#ifndef OOX_HYPERLINKS_FILE_INCLUDE_H_
#define OOX_HYPERLINKS_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CHyperlink : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CHyperlink)
			CHyperlink()
			{
			}
			virtual ~CHyperlink()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<hyperlink"));
				WritingStringNullableAttrEncodeXmlString(L"display", m_oDisplay, m_oDisplay.get());
				WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
				WritingStringNullableAttrEncodeXmlString(L"location", m_oLocation, m_oLocation.get());
				WritingStringNullableAttrEncodeXmlString(L"ref", m_oRef, m_oRef.get());
				WritingStringNullableAttrEncodeXmlString(L"tooltip", m_oTooltip, m_oTooltip.get());
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_Hyperlink;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("display"),      m_oDisplay)
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),      m_oRid )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("location"),      m_oLocation )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("tooltip"),      m_oTooltip )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<std::wstring>						m_oDisplay;
				nullable<SimpleTypes::CRelationshipId>	m_oRid;
				nullable<std::wstring>						m_oLocation;
				nullable<std::wstring>						m_oRef;
				nullable<std::wstring>						m_oTooltip;
		};

		class CHyperlinks  : public WritingElementWithChilds<CHyperlink>
		{
		public:
			WritingElement_AdditionConstructors(CHyperlinks)
			CHyperlinks()
			{
			}
			virtual ~CHyperlinks()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_arrItems.empty()) return;

				writer.WriteString(_T("<hyperlinks>"));
					
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				
				writer.WriteString(_T("</hyperlinks>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("hyperlink") == sName )
						m_arrItems.push_back( new CHyperlink( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Hyperlinks;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_HYPERLINKS_FILE_INCLUDE_H_
