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
			WritingElementSpreadsheet_AdditionConstructors(CHyperlink)
			CHyperlink()
			{
			}
			virtual ~CHyperlink()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<hyperlink"));
				if(m_oDisplay.IsInit())
				{
					CString sVal; sVal.Format(_T(" display=\"%s\""), XmlUtils::EncodeXmlString(m_oDisplay.get()));
					writer.WriteString(sVal);
				}
				if(m_oRid.IsInit())
				{
					CString sVal; sVal.Format(_T(" r:id=\"%s\""), m_oRid->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oLocation.IsInit())
				{
					CString sVal; sVal.Format(_T(" location=\"%s\""), XmlUtils::EncodeXmlString(m_oLocation.get()));
					writer.WriteString(sVal);
				}
				if(m_oRef.IsInit())
				{
					CString sVal; sVal.Format(_T(" ref=\"%s\""), XmlUtils::EncodeXmlString(m_oRef.get()));
					writer.WriteString(sVal);
				}
				if(m_oTooltip.IsInit())
				{
					CString sVal; sVal.Format(_T(" tooltip=\"%s\""), XmlUtils::EncodeXmlString(m_oTooltip.get()));
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
				return et_Hyperlink;
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
				nullable<CString>						m_oDisplay;
				nullable<SimpleTypes::CRelationshipId>	m_oRid;
				nullable<CString>						m_oLocation;
				nullable<CString>						m_oRef;
				nullable<CString>						m_oTooltip;
		};

		class CHyperlinks  : public WritingElementWithChilds<CHyperlink>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CHyperlinks)
			CHyperlinks()
			{
			}
			virtual ~CHyperlinks()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_arrItems.size() > 0)
				{
					writer.WriteString(_T("<hyperlinks>"));
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(_T("</hyperlinks>"));
				}
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

					if ( _T("hyperlink") == sName )
						m_arrItems.push_back( new CHyperlink( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_Hyperlinks;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_HYPERLINKS_FILE_INCLUDE_H_