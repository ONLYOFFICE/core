#pragma once
#ifndef OOX_LOGIC_DIR_INCLUDE_H_
#define OOX_LOGIC_DIR_INCLUDE_H_

#include "../WritingElement.h"
#include "RunProperty.h"

namespace OOX
{
	namespace Logic
	{
		class CDir : public WritingElementWithChilds<>
		{
		public:
			CDir()
			{
			}
			CDir(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CDir(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CDir()
			{
			}

		public:

			const CDir &operator =(const XmlUtils::CXmlNode& oNode)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CDir &operator =(const XmlUtils::CXmlLiteReader& oReader)
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
				return et_w_dir;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"), m_oVal )
					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CDirVal<>> m_oVal;
			// Childs
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DIR_INCLUDE_H_