/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef OOX_LOGIC_DRAWING_EXT_INCLUDE_H_
#define OOX_LOGIC_DRAWING_EXT_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../WritingElement.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CSparklineGroups;
		class CAltTextTable;
		class CConditionalFormatting;
	}
	namespace Drawing
	{
		class CCompatExt : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCompatExt)
			CCompatExt()
			{
			}
			virtual ~CCompatExt()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
                //todo
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_compatExt;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("spid"), m_sSpId )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}

		public:
            nullable<std::wstring> m_sSpId;
		};
		class CDataModelExt : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDataModelExt)
			CDataModelExt()
			{
			}
			virtual ~CDataModelExt()
			{
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
                //todo
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_compatExt;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("relId"), m_oRelId )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}

		public:

			// Attributes
            nullable<std::wstring> m_oRelId;

			// Childs
		};


		//--------------------------------------------------------------------------------
		// COfficeArtExtension 20.1.2.2.14 (Part 1)
		//--------------------------------------------------------------------------------	
		class COfficeArtExtension : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COfficeArtExtension)
			COfficeArtExtension()
			{
			}
			virtual ~COfficeArtExtension();

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
                oNode.ReadAttributeBase( _T("uri"), m_sUri );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            virtual std::wstring toXML() const;
            std::wstring toXMLWithNS(const std::wstring& sNamespace) const;
			virtual EElementType getType() const
			{
				return OOX::et_a_ext;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start_No_NS( oReader )
                WritingElement_ReadAttributes_Read_if( oReader, _T("uri"), m_sUri )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}

		public:

			// Attributes
            nullable<std::wstring>  m_sUri;
            std::wstring            m_sAdditionalNamespace;

			// Childs
			nullable<CCompatExt>							m_oCompatExt;
			nullable<OOX::Spreadsheet::CSparklineGroups>	m_oSparklineGroups;
			nullable<CDataModelExt>							m_oDataModelExt;
			nullable<OOX::Spreadsheet::CAltTextTable>		m_oAltTextTable;
			
			std::vector<OOX::Spreadsheet::CConditionalFormatting*>	m_arrConditionalFormatting;
		};
		//--------------------------------------------------------------------------------
		// COfficeArtExtensionList 20.1.2.2.15 (Part 1)
		//--------------------------------------------------------------------------------	
		class COfficeArtExtensionList : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COfficeArtExtensionList)
			COfficeArtExtensionList()
			{
			}
			virtual ~COfficeArtExtensionList()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrExt.size(); nIndex++ )
				{
					if ( m_arrExt[nIndex] ) delete m_arrExt[nIndex];
					m_arrExt[nIndex] = NULL;
				}
				m_arrExt.clear();
			}

		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
                    if ( _T("ext") == sName )
					{
						OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension(oReader);
                        if (oExt) m_arrExt.push_back( oExt );
					}
				}
			}
            virtual std::wstring      toXML() const
            {
                return toXMLWithNS(_T("a:"));
            }
            std::wstring toXMLWithNS(const std::wstring& sNamespace) const
			{
                std::wstring sResult = _T("<");
                sResult += sNamespace;
                sResult += _T("extLst>");
				
                for ( unsigned int nIndex = 0; nIndex < m_arrExt.size(); nIndex++ )
				{
					if (m_arrExt[nIndex])
                        sResult += m_arrExt[nIndex]->toXMLWithNS(sNamespace);
				}

                sResult += _T("</");
                sResult += sNamespace;
                sResult += _T("extLst>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_extLst;
			}

		public:

			// Childs
            std::vector<OOX::Drawing::COfficeArtExtension*> m_arrExt;
		};
	} // namespace Drawing
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_EXT_INCLUDE_H_
