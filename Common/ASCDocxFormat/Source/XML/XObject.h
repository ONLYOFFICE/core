#pragma once
#ifndef XML_XOBJECT_INCLUDE_H_
#define XML_XOBJECT_INCLUDE_H_

#include "XNode.h"


namespace XML
{
	class XObject
	{
	public:
		XObject();
		virtual ~XObject();

	public:
		virtual void fromXML(const XNode& node) = 0;
		virtual const XNode toXML() const = 0;

	public:
		void this_is_not_xobject_class() const {};
	};
} // namespace XML

#endif // XML_XOBJECT_INCLUDE_H_