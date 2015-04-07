#pragma once
#ifndef OOX_CONTENT_TYPES_OVERRIDE_TABLE_INCLUDE_H_
#define OOX_CONTENT_TYPES_OVERRIDE_TABLE_INCLUDE_H_

#include "./../WritingVector.h"
#include "Override.h"

namespace PPTX
{
	namespace ContentTypes
	{
		class OverrideTable : public WritingVector<Override>
		{
		public:
			OverrideTable()
			{
			}
			virtual ~OverrideTable()
			{
			}
			explicit OverrideTable(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
			}
			const OverrideTable& operator =(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
				return *this;
			}

		public:
			void fromXML(XmlUtils::CXmlNode& node)
			{
				node.LoadArray(_T("Override"), m_items);
			}

		public:
			void add(const CString& type, const OOX::CPath& path)
			{
				m_items.push_back(Override(type, path));
			}
		};
	} // namespace ContentTypes
} // namespace PPTX

#endif // OOX_CONTENT_TYPES_OVERRIDE_TABLE_INCLUDE_H_