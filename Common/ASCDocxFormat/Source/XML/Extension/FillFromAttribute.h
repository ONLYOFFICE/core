#pragma once
#ifndef XML_FILL_FROM_ATTRIBUTE_INCLUDE_H_
#define XML_FILL_FROM_ATTRIBUTE_INCLUDE_H_

#include "property.h"
#include "./../XML.h"
#include <boost/call_traits.hpp>


namespace XML
{
	template<class T, typename V>
	void FillFromAttribute(T& container, const XML::XElement& element, const XML::XName& xname, typename boost::call_traits<V>::param_type value)
	{
		for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			XElement element(*i);
			if (element.attribute(xname).exist() && element.attribute(name).value() == value)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}


	template<class T, typename V>
	void FillFromAttribute(property<T>& container, const XML::XElement& element, const XML::XName& xname, typename boost::call_traits<V>::param_type value)
	{
		for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
		{
			XElement element(*i);
			if (element.attribute(name).exist() && element.attribute(name).value() == value)
			{
				container.push_back(T::value_type(XElement(*i)));
				container.back().this_is_not_xobject_class();
			}
		}
	}
} // namespace XML

#endif // XML_FILL_FROM_ATTRIBUTE_INCLUDE_H_