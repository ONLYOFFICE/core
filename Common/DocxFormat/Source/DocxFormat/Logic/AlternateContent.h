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
				for ( unsigned int nIndex = 0; nIndex < m_arrChoiceItems.size(); nIndex++ )
				{
					if ( m_arrChoiceItems[nIndex] )
						delete m_arrChoiceItems[nIndex];
					m_arrChoiceItems[nIndex] = NULL;
				}
				m_arrChoiceItems.clear();
				for (unsigned int nIndex = 0; nIndex < m_arrFallbackItems.size(); nIndex++ )
				{
					if ( m_arrFallbackItems[nIndex] )
						delete m_arrFallbackItems[nIndex];
					m_arrFallbackItems[nIndex] = NULL;
				}
				m_arrFallbackItems.clear();
				for (unsigned int nIndex = 0; nIndex < m_arrSpreadsheetChoiceItems.size(); nIndex++ )
				{
					if ( m_arrSpreadsheetChoiceItems[nIndex] )
						delete m_arrSpreadsheetChoiceItems[nIndex];
					m_arrSpreadsheetChoiceItems[nIndex] = NULL;
				}
				m_arrSpreadsheetChoiceItems.clear();
				for (unsigned int nIndex = 0; nIndex < m_arrSpreadsheetFallbackItems.size(); nIndex++ )
				{
					if ( m_arrSpreadsheetFallbackItems[nIndex] )
						delete m_arrSpreadsheetFallbackItems[nIndex];
					m_arrSpreadsheetFallbackItems[nIndex] = NULL;
				}
				m_arrSpreadsheetFallbackItems.clear();
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
			
			std::vector<WritingElement *> m_arrChoiceItems;
			std::vector<WritingElement *> m_arrFallbackItems;
			
			std::vector<OOX::Spreadsheet::WritingElement *> m_arrSpreadsheetChoiceItems;
			std::vector<OOX::Spreadsheet::WritingElement *> m_arrSpreadsheetFallbackItems;
		};
	} // namespace Words
} // namespace OOX

#endif // OOX_ALTERNATECONTENT_WORD_INCLUDE_H_