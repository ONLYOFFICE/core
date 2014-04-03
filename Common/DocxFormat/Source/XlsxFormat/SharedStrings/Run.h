#pragma once
#ifndef OOX_RUN_FILE_INCLUDE_H_
#define OOX_RUN_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "Text.h"
#include "../Styles/rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		class CRun : public WritingElementWithChilds<CText>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CRun)
			CRun()
			{
			}
			virtual ~CRun()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<r>"));
				if(m_oRPr.IsInit())
					m_oRPr->toXML(writer);
				for(int i = 0, length = m_arrItems.GetSize(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteString(_T("</r>"));
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

					if ( _T("rPr") == sName )
						m_oRPr = oReader;
					else if ( _T("t") == sName )
						m_arrItems.Add( new CText( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_r;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CRPr>	m_oRPr;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_RUN_FILE_INCLUDE_H_