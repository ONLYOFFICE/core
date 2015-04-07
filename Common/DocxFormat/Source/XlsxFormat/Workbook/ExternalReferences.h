#pragma once
#ifndef OOX_ExternalReferences_FILE_INCLUDE_H_
#define OOX_ExternalReferences_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		class CExternalReference : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CExternalReference)
			CExternalReference()
			{
			}
			virtual ~CExternalReference()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<externalReference"));
				if(m_oRid.IsInit())
				{
					CString sVal;sVal.Format(_T(" r:id=\"%ls\""), m_oRid->GetValue());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_ExternalReference;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),      m_oRid )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<SimpleTypes::CRelationshipId>				m_oRid;

		};

		class CExternalReferences  : public WritingElementWithChilds<CExternalReference>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CExternalReferences)
			CExternalReferences()
			{
			}
			virtual ~CExternalReferences()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<externalReferences>"));
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteString(_T("</externalReferences>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("externalReference") == sName )
						m_arrItems.push_back( new CExternalReference( oReader ));

				}
			}

			virtual EElementType getType () const
			{
				return et_ExternalReferences;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //ExternalReference
} // namespace OOX

#endif // OOX_ExternalReferences_FILE_INCLUDE_H_