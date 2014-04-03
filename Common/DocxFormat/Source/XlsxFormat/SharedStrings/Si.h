#pragma once
#ifndef OOX_SI_FILE_INCLUDE_H_
#define OOX_SI_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "PhoneticPr.h"
#include "Run.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CSi : public WritingElementWithChilds<>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CSi)
			CSi()
			{
			}
			virtual ~CSi()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<si>"));
				for(int i = 0, length = m_arrItems.GetSize(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteString(_T("</si>"));
			}
			virtual void toXML2(XmlUtils::CStringWriter& writer) const
			{
				for(int i = 0, length = m_arrItems.GetSize(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
			}
			CString ToString()
			{
				CString sRes;
				for(int i = 0, length = m_arrItems.GetSize(); i < length; ++i)
				{
					WritingElement* we = m_arrItems[i];
					if(OOX::Spreadsheet::et_r == we->getType())
					{
						CRun* pRun = static_cast<CRun*>(we);
						for(int j = 0, length2 = pRun->m_arrItems.GetSize(); j < length2; ++j)
						{
							CText* pText = pRun->m_arrItems[j];
							sRes.Append(pText->ToString());
						}
					}
					else if(OOX::Spreadsheet::et_t == we->getType())
					{
						CText* pText = static_cast<CText*>(we);
						sRes.Append(pText->ToString());
					}
				}
				return sRes;
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();

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
						m_arrItems.Add( pItem );
				}
			}

			virtual EElementType getType () const
			{
				return et_Si;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SI_FILE_INCLUDE_H_