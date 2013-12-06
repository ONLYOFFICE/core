#pragma once
#ifndef XML_EXTENSION_WRITE_INCLUDE_H_
#define XML_EXTENSION_WRITE_INCLUDE_H_

#include "nullable.h"
#include "property.h"
#include "nullable_property.h"
#include <boost/shared_ptr.hpp>
#include "./../XNode.h"
#include "./../XName.h"
#include <boost/call_traits.hpp>
#include <boost/foreach.hpp>


namespace XML
{
	class XObject;

	const XNode Write(const XObject& object);

	template<class O, typename T>
	const XNode Write(const O& object, const T& value)
	{
		return object.toXML(value);
	}


	template<class T>
	const XNode Write(const nullable__<T>& object)
	{
		if (object.is_init())
		{
			object->this_is_not_xobject_class();
			return object->toXML();
		}
		return XNode();
	}


	template<class T, class S, class G>
	const XNode Write(const property<T, S, G>& object)
	{
		object->this_is_not_xobject_class();
		return object->toXML();
	}


	template<class T, class S, class G, typename V>
	const XNode Write(const property<T, S, G>& object, const V& value)
	{
		object->this_is_not_xobject_class();
		return object->toXML(value);
	}


	template<class T, class S, class G>
	const XNode Write(const nullable_property<T, S, G>& object)
	{
		if (object.is_init())
		{
			object->this_is_not_xobject_class();
			return object->toXML();
		}
		return XNode();
	}


	template<class T, class S, class G, typename V>
	const XNode Write(const nullable_property<T, G, S>& object, const V& value)
	{
		if (object.is_init())
		{
			object->this_is_not_xobject_class();
			return object->toXML(value);
		}
		return XNode();
	}


	template<class T>
	const XNode Write(T* object)
	{
		if (object != 0)
		{
			object->this_is_not_xobject_class();
			return object->toXML();
		}
		return XNode();
	}


	template<class T, typename V>
	const XNode Write(T* object, const V& value)
	{
		if (object != 0)
		{
			object->this_is_not_xobject_class();
			return object->toXML(value);
		}
		return XNode();
	}


	template<class T>
	const XNode Write(const boost::shared_ptr<T>& object)
	{
		if (object != 0)
		{
			object->this_is_not_xobject_class();
			return object->toXML();
		}
		return XNode();
	}


	template<class T, typename V>
	const XNode Write(const boost::shared_ptr<T>& object, const V& value)
	{
		if (object != 0)
		{
			object->this_is_not_xobject_class();
			return object->toXML(value);
		}
		return XNode();
	}


	template<template<typename T, class A> class C, typename T, class A>
	const XNode Write(const C<T, A>& container)
	{
		return XContainer(container);
	}


	template<template<typename T, class A> class C, typename T, class A, typename V>
	const XNode Write(const C<T, A>& container, const V& value)
	{
		XContainer xcontainer;
		BOOST_FOREACH(const T& object, container)
		{
			xcontainer.Add(object.toXML(value));
		}
		return xcontainer;
	}


	template<template<typename T, class A> class C, typename T, class A>
	const XNode Write(const property<C<T, A> >& container)
	{
		return XContainer(container);
	}


	template<template<typename T, class A> class C, typename T, class A, typename V>
	const XNode Write(const property<C<T, A> >& container, const V& value)
	{
		XContainer xcontainer;
		BOOST_FOREACH(const T& object, *container)
		{
			xcontainer.Add(object.toXML(value));
		}
		return xcontainer;
	}


	template<typename T>
	const XML::XNode Write(const XML::XName& element, const XML::XName& attribute, const T& value)
	{
		return XML::XElement(element, XML::XAttribute(attribute, value));
	}


	template<typename T>
	const XML::XNode Write(const XML::XName& element, const XML::XName& attribute, const nullable__<T>& value)
	{
		if (value.is_init())
			return XML::XElement(element, XML::XAttribute(attribute, *value));
		return XML::XNode();
	}


	template<typename T, class S, class G>
	const XML::XNode Write(const XML::XName& element, const XML::XName& attribute, const property<T, S, G>& value)
	{
		return XML::XElement(element, XML::XAttribute(attribute, value));
	}


	template<typename T, class S, class G>
	const XML::XNode Write(const XML::XName& element, const XML::XName& attribute, const nullable_property<T, S, G>& value)
	{
		if (value.is_init())
			return XML::XElement(element, XML::XAttribute(attribute, value));
		return XML::XNode();
	}

} // namespace XML

#endif // XMK_EXTENSION_WRITE_INCLUDE_H_