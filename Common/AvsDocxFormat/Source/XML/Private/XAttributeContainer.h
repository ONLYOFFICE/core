#pragma once
#ifndef XML_PRIVATE_XATTRIBUTE_CONTAINER_INCLUDE_H_
#define XML_PRIVATE_XATTRIBUTE_CONTAINER_INCLUDE_H_

#include "XContainer.h"
#include "./../XAttribute.h"
#include "XNamespaceContainer.h"


namespace XML
{
	class XName;

	namespace Private
	{
		class XAttributeContainer : public XContainer<XAttribute>
		{
		public:
			const bool exist(const XName& xname) const;

			XAttributeContainer const* const	operator->() const	{return this;}
			XAttributeContainer*							operator->()				{return this;}

			XAttribute& operator[] (const XName& xname);
			const XAttribute& operator[] (const XName& xname) const;

			const XNamespaceContainer usedNamespace() const;

		public:
			typedef std::list<XAttribute>::iterator iterator;
			typedef std::list<XAttribute>::const_iterator const_iterator;

		public:
			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XATTRIBUTE_CONTAINER_INCLUDE_H_