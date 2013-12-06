#pragma once
#ifndef OOX_WRITING_VECTOR_INCLUDE_H_
#define OOX_WRITING_VECTOR_INCLUDE_H_

#include "WritingElement.h"
#include "IEnumerable.h"

namespace OOX
{
	template<class T>
	class WritingVector : public WritingElement, public IEnumerable<T>
	{
	public:
		WritingVector() {}
		virtual ~WritingVector() {}
		explicit WritingVector(const XML::XNode& node) 
		{
			fromXML(node);
		}

		const WritingVector& operator =(const XML::XNode& node) 
		{
			fromXML(node); 
			return *this;
		}

	public:
		virtual void fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element);
		}

		virtual const XML::XNode toXML() const 
		{
			return XML::XElement();//
			//return XML::Write(m_items);
		}
	};
} // namespace OOX

#endif // OOX_WRITING_VECTOR_INCLUDE_H_