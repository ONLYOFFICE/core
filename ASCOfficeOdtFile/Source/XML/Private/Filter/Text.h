#pragma once
#ifndef XML_PRIVATE_FILTER_TEXT_INCLUDE_H_
#define XML_PRIVATE_FILTER_TEXT_INCLUDE_H_

#include <functional>
#include "./../../XNode.h"


namespace XML
{
	namespace Private
	{
		namespace Filter
		{
			struct Text : public std::unary_function<XNode, const bool>
			{
				const bool operator ()(const XNode& xnode) const
				{
					return xnode.isText();
				}
			};
		}
	}
}

#endif // XML_PRIVATE_FILTER_TEXT_INCLUDE_H_