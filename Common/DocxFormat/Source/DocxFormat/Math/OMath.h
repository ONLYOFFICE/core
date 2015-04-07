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
		class CMathArgNodes : public WritingElementWithChilds<>
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
			}

		public:

			const CMathArgNodes &operator =(const XmlUtils::CXmlNode& oNode)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CMathArgNodes &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual CString      toXML() const
			{
				CString sResult = _T("<") + sNodeName + _T(">");

				for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
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
		};

		
		
	}//namespace Logic
}//namespace OOX