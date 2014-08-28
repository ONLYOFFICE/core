#pragma once
#ifndef OOX_LOGIC_DRAWING_EXT_INCLUDE_H_
#define OOX_LOGIC_DRAWING_EXT_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../WritingElement.h"

namespace OOX
{
	namespace Drawing
	{
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
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:ext ");
				
				if ( m_oUri.IsInit() )
				{
					sResult += _T("uri=\"");
					sResult += m_oUri->GetString();
					sResult += _T("\">");
				}
				else
					sResult += _T(">");

				sResult += _T("</a:ext>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_ext;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("uri"), m_oUri )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<CString> m_oUri;

			// Childs
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
                    CString sName = oReader.GetName();
					if ( _T("a:ext") == sName )
					{
						OOX::Drawing::COfficeArtExtension oExt = oReader;
                        m_arrExt.push_back( oExt );
					}
				}
			}
			virtual CString      toXML() const
			{
				CString sResult = _T("<a:extLst>");
				
                for ( unsigned int nIndex = 0; nIndex < m_arrExt.size(); nIndex++ )
					sResult += m_arrExt[nIndex].toXML();

				sResult += _T("</a:extLst>");

				return sResult;
			}
			virtual EElementType getType() const
			{
				return OOX::et_a_extLst;
			}

		public:

			// Childs
            //CSimpleArray<OOX::Drawing::COfficeArtExtension> m_arrExt;
            std::vector<OOX::Drawing::COfficeArtExtension> m_arrExt;
		};
	} // namespace Drawing
} // namespace OOX

#endif // OOX_LOGIC_DRAWING_EXT_INCLUDE_H_
