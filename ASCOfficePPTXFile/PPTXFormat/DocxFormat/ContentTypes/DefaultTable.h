#pragma once
#ifndef OOX_CONTENT_TYPES_DEFAULT_TABLE_INCLUDE_H_
#define OOX_CONTENT_TYPES_DEFAULT_TABLE_INCLUDE_H_

#include "./../WritingVector.h"
#include "Default.h"


namespace PPTX
{
	namespace ContentTypes
	{
		class DefaultTable : public WritingVector<Default>
		{
		public:
			DefaultTable()
			{
				m_items.Add(Default(_T("rels")));
				m_items.Add(Default(_T("xml")));
			}
			virtual ~DefaultTable()
			{
			}
			explicit DefaultTable(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
			}
			const DefaultTable& operator =(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.LoadArray(_T("Default"), m_items);
			}

		public:
			void add(const OOX::CPath& path)
			{
				CString ext = path.GetExtention();
				const CString extension = ext.Mid(1);
				
				size_t nCount = m_items.GetCount();
				size_t nIndex = 0;	

				while (nIndex < nCount)
				{
					if (m_items[nIndex] == extension)
						break;
					++nIndex;
				}
				
				if (nIndex == nCount)
					m_items.Add(Default(extension));
			}
		};
	} // namespace ContentTypes
} // namespace PPTX

#endif // OOX_CONTENT)TYPES_DEFAULT_TABLE_INCLUDE_H_