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
#include "../CommonInclude.h"
#include "../../XlsbFormat/Biff12_records/HLink.h"

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
			CHyperlink(OOX::Document *pMain = NULL) : WritingElement(pMain)
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
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString((L"<hyperlink"));
				WritingStringNullableAttrEncodeXmlString(L"display", m_oDisplay, m_oDisplay.get());
				WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
				WritingStringNullableAttrEncodeXmlString(L"location", m_oLocation, m_oLocation.get());
				WritingStringNullableAttrEncodeXmlString(L"ref", m_oRef, m_oRef.get());
				WritingStringNullableAttrEncodeXmlString(L"tooltip", m_oTooltip, m_oTooltip.get());
				writer.WriteString((L"/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }
			void toBin(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::HLink*>(obj.get());
				if (ptr != nullptr)
				{
					if (m_oDisplay.IsInit())
						ptr->display = m_oDisplay.get();
					else
						ptr->display = L"";

					if (m_oRid.IsInit())
						ptr->relId = m_oRid->GetValue();
					else
						ptr->relId = std::wstring(L"");

					if (m_oLocation.IsInit())
						ptr->location = m_oLocation.get();
					else
						ptr->location = L"";

					if (m_oRef.IsInit())
						ptr->rfx = m_oRef.get();
					else
						ptr->rfx = std::wstring(L"");

					if (m_oTooltip.IsInit())
						ptr->tooltip = m_oTooltip.get();
					else
						ptr->tooltip = L"";

				}
			}

			virtual EElementType getType () const
			{
				return et_x_Hyperlink;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, (L"display"),	m_oDisplay)
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"r:id"),		m_oRid )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"relationships:id"), m_oRid )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"location"),	m_oLocation )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"ref"),		m_oRef )
					WritingElement_ReadAttributes_Read_else_if	( oReader, (L"tooltip"),	m_oTooltip )
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::HLink*>(obj.get());
				if (ptr != nullptr)
				{
					if(!ptr->display.value().empty())
						m_oDisplay = ptr->display.value();

					if (!ptr->relId.value.value().empty())
						m_oRid = ptr->relId.value.value();

					if (!ptr->location.value().empty())
						m_oLocation = ptr->location.value();

					if (!ptr->rfx.toString().empty())
						m_oRef = ptr->rfx.toString();

					if (!ptr->tooltip.value().empty())
						m_oTooltip = ptr->tooltip.value();
				}

            }

		public:
				nullable_string							m_oDisplay;
				nullable<SimpleTypes::CRelationshipId>	m_oRid;
				nullable_string							m_oLocation;
				nullable_string							m_oRef;
				nullable_string							m_oTooltip;
				
				nullable_string							m_oLink;
		};

		class CHyperlinks  : public WritingElementWithChilds<CHyperlink>
		{
		public:
            WritingElement_AdditionConstructors(CHyperlinks)
            WritingElement_XlsbVectorConstructors(CHyperlinks)
			CHyperlinks(OOX::Document *pMain = NULL) : WritingElementWithChilds<CHyperlink>(pMain)
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
				return (L"");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_arrItems.empty()) return;

				writer.WriteString((L"<hyperlinks>"));
					
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				
				writer.WriteString((L"</hyperlinks>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( (L"hyperlink") == sName )
					{
						CHyperlink *pHyperlink = new CHyperlink();
						m_arrItems.push_back(pHyperlink);

						pHyperlink->fromXML(oReader);
					}
				}
			}
            void fromBin(std::vector<XLS::BaseObjectPtr>& obj)
            {
                if (obj.empty())
                    return;

                for(auto &hyperlink : obj)
                {
                    CHyperlink *pHyperlink = new CHyperlink(m_pMainDocument);
                    m_arrItems.push_back(pHyperlink);

                    pHyperlink->fromBin(hyperlink);
                }
            }
			void toBin(std::vector<XLS::BaseObjectPtr>& obj)
			{
				obj.reserve(m_arrItems.size());
				for (size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if (m_arrItems[i])
					{
						XLS::BaseObjectPtr item(new XLSB::HLink());
						m_arrItems[i]->toBin(item);
						obj.push_back(item);
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Hyperlinks;
			}
		};
	} //Spreadsheet
} // namespace OOX
