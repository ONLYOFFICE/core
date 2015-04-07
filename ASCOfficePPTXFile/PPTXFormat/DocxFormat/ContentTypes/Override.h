#pragma once
#ifndef OOX_CONTENT_TYPES_OVERRIDE_INCLUDE_H_
#define OOX_CONTENT_TYPES_OVERRIDE_INCLUDE_H_

#include "./../WritingElement.h"
#include "../../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

namespace PPTX
{
	namespace ContentTypes
	{
		class Override : public WritingElement
		{
		public:
			Override()
			{
			}
			Override(const CString& type, const OOX::CPath& path) : m_type(type), m_part(path)
			{
			}
			virtual ~Override()
			{
			}
			explicit Override(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
			}
			const Override& operator =(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_part	= node.GetAttribute(_T("PartName"));
				m_type	= node.GetAttribute(_T("ContentType"));
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("PartName"), _T("/") + m_part.m_strFilename);
				oAttr.Write(_T("ContentType"), m_type);

				return XmlUtils::CreateNode(_T("Override"), oAttr);
			}
			virtual EElementType getType() const
			{
				return et_Override;
			}

		public:
			AVSINLINE const CString type() const
			{
				return m_type;
			}
			AVSINLINE const OOX::CPath filename() const
			{
				return m_part;
			}

		private:
			CString						m_type;
			OOX::CPath					m_part;
		};
	} // namespace ContentTypes
} // namespace PPTX

#endif // OOX_CONTENT_TYPES_OVERRIDE_INCLUDE_H_