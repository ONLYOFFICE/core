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
#ifndef OOX_SI_FILE_INCLUDE_H_
#define OOX_SI_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "PhoneticPr.h"
#include "Run.h"
#include "../../XlsbFormat/Biff12_records/SSTItem.h"

namespace NSBinPptxRW
{
	class CBinaryFileReader;
	class CXlsbBinaryWriter;
}

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CSi : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CSi)
            WritingElement_XlsbConstructors(CSi)
			CSi()
			{
			}
			virtual ~CSi()
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
				writer.WriteString(_T("<si>"));
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				writer.WriteString(_T("</si>"));
			}
			virtual void toXML2(NSStringUtils::CStringBuilder& writer) const
			{
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
			}
			std::wstring ToString()
			{
				std::wstring sRes;

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
                    if ( m_arrItems[i] == NULL) continue;

                    OOX::Spreadsheet::WritingElement* we = m_arrItems[i];
					
					if(OOX::et_x_r == we->getType())
					{
						OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(we);

                        for ( std::vector<CText*>::iterator it1 = pRun->m_arrItems.begin(); it1 != pRun->m_arrItems.end(); it1++)
						{
							CText* pText = *it1;
							if (pText)
							{
								sRes.append(pText->ToString());
							}
						}
					}
					else if(OOX::et_x_t == we->getType())
					{
						CText* pText = static_cast<CText*>(we);
						sRes.append(pText->ToString());
					}
				}
				return sRes;
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

					WritingElement *pItem = NULL;
					if ( _T("phoneticPr") == sName )
						pItem = new CPhonetic( oReader );
					else if ( _T("r") == sName )
						pItem = new CRun( oReader );
					else if ( _T("rPh") == sName )
						pItem = new CRPh( oReader );
					else if ( _T("t") == sName )
						pItem = new CText( oReader );

					if ( NULL != pItem )
						m_arrItems.push_back( pItem );
				}
			}

            void fromBin(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::SSTItem*>(obj.get());
                CText* text             = nullptr;
                CPhonetic* phoneticPr   = nullptr;
                CRPh* rPh               = nullptr;
                CRun* r                 = nullptr;
                if(ptr != nullptr)
                {
                    text = new CText();
                    text->fromBin(ptr->richStr.str.value());
                    m_arrItems.push_back(text);

                    for(auto &phRun : ptr->richStr.rgsPhRun)
                    {
                        phoneticPr = new CPhonetic();
                        phoneticPr->fromBin(phRun);
                        m_arrItems.push_back(phoneticPr);

                        rPh = new CRPh();
                        rPh->fromBin(phRun, ptr->richStr.phoneticStr.value());
                        m_arrItems.push_back(rPh);
                    }

                    for(auto &strRun : ptr->richStr.rgsStrRun)
                    {
                        r = new CRun();
                        r->fromBin(strRun, ptr->richStr.str.value());
                        m_arrItems.push_back(r);
                    }

                }
            }

			void fromXLSBExt (NSBinPptxRW::CBinaryFileReader& oStream);
			void toXLSBExt (NSBinPptxRW::CXlsbBinaryWriter& oStream);
			_UINT32 getXLSBSize() const;

			virtual EElementType getType () const
			{
				return et_x_Si;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SI_FILE_INCLUDE_H_
