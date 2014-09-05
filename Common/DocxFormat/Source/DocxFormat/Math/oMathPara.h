#pragma once
#include "oMathContent.h"

namespace OOX
{	
	namespace Logic
	{
		//typedef class		CMathArgNodes COMath;
		//--------------------------------------------------------------------------------
		// COMathPara 22.1.2.78 (Office Math Paragraph) 
		//--------------------------------------------------------------------------------
		
		class COMathPara : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COMathPara)
			COMathPara()
			{
			}
			virtual ~COMathPara()
			{
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();				
					WritingElement *pItem = NULL;

					if ( _T("m:oMath") == sName )
						pItem = new COMath( oReader );
					else if ( _T("m:oMathParaPr") == sName )
						pItem = new COMathParaPr( oReader );

					if ( pItem )
						m_arrItems.push_back( pItem );
					
				}
			}
			virtual CString      toXML() const;
			

			virtual EElementType getType() const
			{
				return et_m_oMathPara;
			}

		public:				
			//Childs
			std::vector<WritingElement* >	m_arrItems;
		};
	}//namespace Logic
}//namespace OOX