#pragma once
#include "../WritingElement.h"
#include "../../Base/Nullable.h"

namespace OOX
{	
	namespace Logic
	{	
		//--------------------------------------------------------------------------------
		// COMath 22.1.2.77 
		//--------------------------------------------------------------------------------	
		class CMathArgNodes : public WritingElement
		{
		public:
			CMathArgNodes()
			{
				eType = et_Unknown;
			}
			CMathArgNodes(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CMathArgNodes(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CMathArgNodes()
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

			const CMathArgNodes &operator =(const XmlUtils::CXmlNode& oNode)
			{
				Clear();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CMathArgNodes &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				Clear();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

			void Clear()
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
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual CString      toXML() const
			{
				CString sResult = _T("<") + sNodeName + _T(">");

				for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
				{
					if ( m_arrItems[nIndex])
					{
						sResult += m_arrItems[nIndex]->toXML();
					}
				}

				sResult = _T("</") + sNodeName + _T(">");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return eType;
			}
		private:
			EElementType eType;
			CString sNodeName;

		public:
			// Childs
			CSimpleArray<WritingElement *> m_arrItems;
		};

		
		
	}//namespace Logic
}//namespace OOX