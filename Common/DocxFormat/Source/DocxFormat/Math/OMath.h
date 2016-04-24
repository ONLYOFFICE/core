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
				eType		= et_Unknown;
			}
			CMathArgNodes(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
				sNodeName	= GetMathNodeName(getType());
			}
			CMathArgNodes(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
				sNodeName	= GetMathNodeName(getType());
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
				if (sNodeName.IsEmpty()) return L"";

				CString sResult = _T("<") + sNodeName + _T(">");

				for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex])
					{
						sResult += m_arrItems[nIndex]->toXML();
					}
				}

				sResult += _T("</") + sNodeName + _T(">");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return eType;
			}
		private:
			EElementType	eType;
		public:
			CString			sNodeName;

			CString GetMathNodeName(const EElementType & enumType)  const
			{//todooo вытащить в одно место - пересекается с MathBottomNodes
				switch(enumType)
				{
					case OOX::et_m_deg:		return L"m:deg";
					case OOX::et_m_den:		return L"m:den";
					case OOX::et_m_e:		return L"m:e";
					case OOX::et_m_fName:	return L"m:fName";
					case OOX::et_m_lim:		return L"m:lim";
					case OOX::et_m_num:		return L"m:num";
					case OOX::et_m_oMath:	return L"m:oMath";
					case OOX::et_m_sub:		return L"m:sub";
					case OOX::et_m_sup:		return L"m:sup";
				}
				return L"";
			}
			// Childs
		};

		template <EElementType EnumType = OOX::et_Unknown>
		class CMathArgNodesEx : public CMathArgNodes
		{
		public:
			CMathArgNodesEx(XmlUtils::CXmlNode& oNode)
			{
				fromXML( oNode );
			}
			CMathArgNodesEx(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual EElementType getType() const
			{
				return EnumType;
			}
		};		
		
	}//namespace Logic
}//namespace OOX