#pragma once
#ifndef OOX_LOGIC_BDO_INCLUDE_H_
#define OOX_LOGIC_BDO_INCLUDE_H_

#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Word.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CBdo 17.3.2.3 (Part 1)
		//--------------------------------------------------------------------------------	
		class CBdo : public WritingElementWithChilds<>
		{
		public:
			CBdo()
			{
			}
			CBdo(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CBdo(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CBdo()
			{
			}

		public:

			const CBdo &operator =(const XmlUtils::CXmlNode& oNode)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CBdo &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const;
			virtual EElementType getType() const
			{
				return et_w_bdo;
			}

		private: 

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:val"), m_oVal )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			SimpleTypes::CDirection<SimpleTypes::directionLTR> m_oVal;

			// Childs
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BDO_INCLUDE_H_
