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
				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					CWCharWrapper sName = oReader.GetName();
					if ( _T("m:oMath") == sName )
						m_oOMath = oReader;
					else if ( _T("m:oMathParaPr") == sName )
						m_oOMathParaPr = oReader;
				}
			}
			virtual CString      toXML() const;
			

			virtual EElementType getType() const
			{
				return et_m_oMathPara;
			}

		public:				
			//Childs
			nullable<OOX::Logic::COMath>		m_oOMath;
			nullable<OOX::Logic::COMathParaPr>  m_oOMathParaPr;
		};
	}//namespace Logic
}//namespace OOX