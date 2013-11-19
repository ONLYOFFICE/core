#pragma once
#ifndef ODT_WRITING_VECTOR_INCLUDE_H_
#define ODT_WRITING_VECTOR_INCLUDE_H_

#include "WritingElement.h"
#include "IEnumerable.h"


namespace Odt
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
			return XML::Write(m_items);
		}
	};
} // namespace Odt

#endif // ODT_WRITING_VECTOR_INCLUDE_H_