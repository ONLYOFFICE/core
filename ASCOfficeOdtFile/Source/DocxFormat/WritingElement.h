#pragma once
#ifndef OOX_WRITING_ELEMENT_INCLUDE_H_
#define OOX_WRITING_ELEMENT_INCLUDE_H_

#include "NamespaceOwn.h"
#include "XML.h"

namespace OOX
{
	class WritingElement : public NamespaceOwn, public XML::XObject
	{
	public:
		WritingElement(){};
		virtual ~WritingElement()  = 0 {};
	};

	class NullElement : public WritingElement
	{
	public:
		NullElement() {}
		virtual ~NullElement() {}		
		explicit NullElement(const XML::XNode& node) {	fromXML(node);	}
		const NullElement& operator =(const XML::XNode& node) {	return *this;	}
	public:
		virtual void fromXML(const XML::XNode& node) {}
		virtual const XML::XNode toXML() const { return	XML::XElement(); }
	};

} // namespace OOX

#endif // OOX_WRITING_ELEMENT_INCLUDE_H_