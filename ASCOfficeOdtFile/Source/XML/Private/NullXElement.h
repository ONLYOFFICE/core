#pragma once
#ifndef XML_PRIVATE_NULL_XELEMENT_INCLUDE_H_
#define XML_PRIVATE_NULL_XELEMENT_INCLUDE_H_

#include "./../XElement.h"


namespace XML
{
	namespace Private
	{
		class NullXElement : public XElement
		{
		public:
			NullXElement();

		public:
			virtual const bool exist() const;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_NULL_XELEMENT_INCLUDE_H_