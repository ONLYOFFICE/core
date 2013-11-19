#pragma once
#ifndef OOX_LOGIC_HYPERLINK_INCLUDE_H_
#define OOX_LOGIC_HYPERLINK_INCLUDE_H_

#include "../../Base/Nullable.h"

#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/SimpleTypes_Shared.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CHyperlink 17.16.22 (Part 1)
		//--------------------------------------------------------------------------------	
		class CHyperlink : public WritingElement
		{
		public:
			CHyperlink()
			{
			}
			CHyperlink(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CHyperlink(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CHyperlink()
			{
				for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.RemoveAll();
			}

		public:

			const CHyperlink &operator =(const XmlUtils::CXmlNode& oNode)
			{
				Clear();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CHyperlink &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				Clear();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

			void Clear()
			{
				m_sAnchor.reset();
				m_sDocLocation.reset();
				m_oHistory.reset();
				m_oId.reset();
				m_sTgtFrame.reset();
				m_sTooltip.reset();
				
				for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						delete m_arrItems[nIndex];

					m_arrItems[nIndex] = NULL;
				}

				m_arrItems.RemoveAll();
			}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return et_w_hyperlink;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:anchor"),      m_sAnchor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:docLocation"), m_sDocLocation )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:history"),     m_oHistory )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:id"),          m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:tgtFrame"),    m_sTgtFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:tooltip"),     m_sTooltip )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<CString                                      > m_sAnchor;
			nullable<CString                                      > m_sDocLocation;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse> > m_oHistory;
			nullable<SimpleTypes::CRelationshipId                 > m_oId;
			nullable<CString                                      > m_sTgtFrame;
			nullable<CString                                      > m_sTooltip;

			// Childs
			CSimpleArray<WritingElement *>                          m_arrItems;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_HYPERLINK_INCLUDE_H_