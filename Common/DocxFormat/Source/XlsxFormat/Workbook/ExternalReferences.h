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
#ifndef OOX_ExternalReferences_FILE_INCLUDE_H_
#define OOX_ExternalReferences_FILE_INCLUDE_H_

#include "../CommonInclude.h"
#include "../../XlsbFormat/Biff12_unions/SUP.h"
#include "../../XlsbFormat/Biff12_records/SupBookSrc.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CExternalReference : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExternalReference)
                        WritingElement_XlsbConstructors(CExternalReference)
			CExternalReference()
			{
			}
			virtual ~CExternalReference()
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
				writer.WriteString((L"<externalReference"));
				WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
				writer.WriteString((L"/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

                        virtual void fromBin(XLS::BaseObjectPtr& obj)
                        {
                            ReadAttributes(obj);
                        }

			virtual EElementType getType () const
			{
				return et_x_ExternalReference;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, L"id", m_oRid )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}

                        void ReadAttributes(XLS::BaseObjectPtr& obj)
                        {
                            auto ptr = static_cast<XLSB::SUP*>(obj.get());
                            if(ptr != nullptr && ptr->m_source != nullptr)
                            {
                                if(ptr->m_source->get_type() == XLS::typeSupBookSrc)
                                {
                                    if(!static_cast<XLSB::SupBookSrc*>(ptr->m_source.get())->strRelID.value.value().empty())
                                        m_oRid = static_cast<XLSB::SupBookSrc*>(ptr->m_source.get())->strRelID.value.value();
                                }
                            }
                        }

		public:
				nullable<SimpleTypes::CRelationshipId>				m_oRid;

		};

		class CExternalReferences  : public WritingElementWithChilds<CExternalReference>
		{
		public:
			WritingElement_AdditionConstructors(CExternalReferences)
                        WritingElement_XlsbVectorConstructors(CExternalReferences)
			CExternalReferences()
			{
			}                        
			virtual ~CExternalReferences()
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
				if (m_arrItems.empty()) return;

				writer.WriteString((L"<externalReferences>"));
				
                                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                                {
                                    if (  m_arrItems[i] )
                                    {
                                        m_arrItems[i]->toXML(writer);
                                    }
                                }
				
				writer.WriteString((L"</externalReferences>"));
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

					if ( (L"externalReference") == sName )
						m_arrItems.push_back( new CExternalReference( oReader ));

				}
			}

                        virtual void fromBin(std::vector<XLS::BaseObjectPtr>& obj)
                        {
                            //ReadAttributes(obj);

                            if (obj.empty())
                                return;

                            for(auto &externalReference : obj)
                            {
                                m_arrItems.push_back(new CExternalReference(externalReference));
                            }
                        }

			virtual EElementType getType () const
			{
				return et_x_ExternalReferences;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //ExternalReference
} // namespace OOX

#endif // OOX_ExternalReferences_FILE_INCLUDE_H_
