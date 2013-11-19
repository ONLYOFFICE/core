#pragma once
#ifndef OOX_ALTERNATECONTENT_WORD_INCLUDE_H_
#define OOX_ALTERNATECONTENT_WORD_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../WritingElement.h"
#include "../../XlsxFormat/WritingElement.h"
namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CAnchorLock 14.3.2.1 (Part 4)
		//--------------------------------------------------------------------------------	
		class CAlternateContent : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAlternateContent)
			CAlternateContent()
			{
			}
			virtual ~CAlternateContent()
			{
				Clear();
			}
			void Clear()
			{
				for ( int nIndex = 0; nIndex < m_arrChoiceItems.GetSize(); nIndex++ )
				{
					if ( m_arrChoiceItems[nIndex] )
						delete m_arrChoiceItems[nIndex];
					m_arrChoiceItems[nIndex] = NULL;
				}
				m_arrChoiceItems.RemoveAll();
				for ( int nIndex = 0; nIndex < m_arrFallbackItems.GetSize(); nIndex++ )
				{
					if ( m_arrFallbackItems[nIndex] )
						delete m_arrFallbackItems[nIndex];
					m_arrFallbackItems[nIndex] = NULL;
				}
				m_arrFallbackItems.RemoveAll();
				for ( int nIndex = 0; nIndex < m_arrSpreadsheetChoiceItems.GetSize(); nIndex++ )
				{
					if ( m_arrSpreadsheetChoiceItems[nIndex] )
						delete m_arrSpreadsheetChoiceItems[nIndex];
					m_arrSpreadsheetChoiceItems[nIndex] = NULL;
				}
				m_arrSpreadsheetChoiceItems.RemoveAll();
				for ( int nIndex = 0; nIndex < m_arrSpreadsheetFallbackItems.GetSize(); nIndex++ )
				{
					if ( m_arrSpreadsheetFallbackItems[nIndex] )
						delete m_arrSpreadsheetFallbackItems[nIndex];
					m_arrSpreadsheetFallbackItems[nIndex] = NULL;
				}
				m_arrSpreadsheetFallbackItems.RemoveAll();
			}
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return OOX::et_mc_alternateContent;
			}
		public:
			nullable<CString > m_sXml;
			CSimpleArray<WritingElement *> m_arrChoiceItems;
			CSimpleArray<WritingElement *> m_arrFallbackItems;
			CSimpleArray<OOX::Spreadsheet::WritingElement *> m_arrSpreadsheetChoiceItems;
			CSimpleArray<OOX::Spreadsheet::WritingElement *> m_arrSpreadsheetFallbackItems;
		};
	} // namespace Words
} // namespace OOX

#endif // OOX_ALTERNATECONTENT_WORD_INCLUDE_H_