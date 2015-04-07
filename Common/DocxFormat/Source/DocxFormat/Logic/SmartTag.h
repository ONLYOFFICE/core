#pragma once
#ifndef OOX_LOGIC_SMARTTAG_INCLUDE_H_
#define OOX_LOGIC_SMARTTAG_INCLUDE_H_

#include "../../Base/Nullable.h"

#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/SimpleTypes_Shared.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// smartTag 17.5.1.9 (Part 1)
		//--------------------------------------------------------------------------------	
		class CSmartTag : public WritingElementWithChilds<>
		{
		public:
			CSmartTag()
			{
			}
			CSmartTag(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CSmartTag(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CSmartTag()
			{
			}

		public:

			const CSmartTag &operator =(const XmlUtils::CXmlNode& oNode)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CSmartTag &operator =(const XmlUtils::CXmlLiteReader& oReader)
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
				return et_w_smartTag;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				if ( oReader.GetAttributesCount() <= 0 )
					return;
				
				if ( !oReader.MoveToFirstAttribute() )
					return;
				
				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
				{					
					if      ( _T("w:element")         == wsName ) m_sElement         = oReader.GetText();
					else if ( _T("w:uri")          == wsName ) m_sUri          = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}
				oReader.MoveToElement();
			}

		public:

			// Attributes
			nullable<CString> m_sElement;
			nullable<CString> m_sUri;

			// Childs
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SMARTTAG_INCLUDE_H_