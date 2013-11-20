#pragma once
#ifndef XML_PRIVATE_FILTER_ELEMENT_INCLUDE_H_
#define XML_PRIVATE_FILTER_ELEMENT_INCLUDE_H_

#include <functional>
#include "./../../XNode.h"


namespace XML
{
	namespace Private
	{
		namespace Filter
		{
			struct Element : public std::unary_function<XNode, const bool>
			{
				const bool operator ()(const XNode& xnode) const
				{
					return xnode.isElement();
				}
			};
		}
	}
}

#endif // XML_PRIVATE_FILTER_ELEMENT_INCLUDE_H_