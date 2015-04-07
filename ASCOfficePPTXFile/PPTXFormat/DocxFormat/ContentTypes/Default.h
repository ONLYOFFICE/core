#pragma once
#ifndef OOX_CONTENT_TYPES_DEFAULT_INCLUDE_H_
#define OOX_CONTENT_TYPES_DEFAULT_INCLUDE_H_

#include "./../WritingElement.h"
#include "ExtensionTable.h"

namespace PPTX
{
	namespace ContentTypes
	{
		class Default : public WritingElement
		{
		public:
			Default() 
			{
				m_extension = _T("");
			}
			Default(const CString& extension) : m_extension(extension)
			{
			}
			virtual ~Default()
			{
			}
			explicit Default(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
			}
			const Default& operator =(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
				return *this;
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode& node)
			{
				m_extension = node.GetAttribute(_T("Extension"));
			}
			virtual CString      toXML() const
			{
				static const ExtensionTable table;
				
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("Extension"), m_extension);
				oAttr.Write(_T("ContentType"), table[m_extension]);

				return XmlUtils::CreateNode(_T("Default"), oAttr);
			}
			virtual EElementType getType() const
			{
				return et_Default;
			}

		public:
			const bool operator ==(const CString& rhs) const
			{
				return m_extension == rhs;
			}

		private:
			CString	m_extension;
		};
	} // namespace ContentTypes
} // namespace PPTX

#endif // OOX_CONTENT_TYPES_DEFAULT_INCLUDE_H_