#pragma once
#ifndef OOX_WRITING_VECTOR_INCLUDE_H_
#define OOX_WRITING_VECTOR_INCLUDE_H_

#include "WritingElement.h"

namespace PPTX
{
	template <typename T>
	class WritingVector : public WritingElement
	{
	public:
		std::vector<T> m_items;

	public:
		WritingVector() : m_items()  {}
		virtual ~WritingVector()	 {}
		explicit WritingVector(XmlUtils::CXmlNode& node) 
		{
			fromXML(node);
		}

		WritingVector& operator =(XmlUtils::CXmlNode& node) 
		{
			fromXML(node); 
			return *this;
		}

	public:
		virtual void fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();

				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode nodeTemp;
					oNodes.GetAt(i, nodeTemp);

					m_items.push_back(T(nodeTemp));
				}			
			}
		}

		virtual CString toXML() const
		{
			CString strResult = _T("");
			size_t nCount = m_items.size();

			for (size_t i = 0; i < nCount; ++i)
			{
				strResult += m_items[i].toXML();
			}

			return strResult;
		}
		virtual EElementType getType() const
		{
			return et_Unknown;
		}
	};
} // namespace PPTX

#endif // OOX_WRITING_VECTOR_INCLUDE_H_