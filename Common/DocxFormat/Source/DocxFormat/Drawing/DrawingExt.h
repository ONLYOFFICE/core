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
			virtual CString      toXML() const
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
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<CString> m_sSpId;

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
			virtual ~COfficeArtExtension()
			{
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				oNode.ReadAttributeBase( _T("uri"), m_oUri );
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
            CString toXMLWithNS(const CString& sNamespace) const;
			virtual EElementType getType() const
			{
				return OOX::et_a_ext;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("uri"), m_oUri )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<CString> m_oUri;
            CString m_sAdditionalNamespace;

			// Childs
			nullable<CCompatExt> m_oCompatExt;
			nullable<OOX::Spreadsheet::CSparklineGroups> m_oSparklineGroups;
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

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
				// TO DO: Реализовать
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    CString sName = XmlUtils::GetNameNoNS(oReader.GetName());
                    if ( _T("ext") == sName )
					{
						OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension(oReader);
                        if (oExt) m_arrExt.push_back( oExt );
					}
				}
			}
			virtual CString      toXML() const
            {
                return toXMLWithNS(_T("a:"));
            }
            CString toXMLWithNS(const CString& sNamespace) const
			{
                CString sResult = _T("<");
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
