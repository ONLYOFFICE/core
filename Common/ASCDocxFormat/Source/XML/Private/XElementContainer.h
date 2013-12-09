#pragma once
#ifndef XML_PRIVATE_XELEMENT_CONTAINER_INCLUDE_H_
#define XML_PRIVATE_XELEMENT_CONTAINER_INCLUDE_H_

#include <list>

#include "Filter/Element.h"
#include "XNamespaceContainer.h"

#include "../../../../../Common/DocxFormat/Source/Base/SmartPtr.h"

namespace XML
{
	class XNode;
	class XElement;
	class XName;

	namespace Private
	{
		class XNodeContainer;

		class XElementContainer
		{
		public:
			XElementContainer(const XNodeContainer& nodes);

		public:
			const bool empty() const;
			const size_t size() const;
			void push_back(const XElement& element);
			void Add(const XElement& element);

		public:
		
			typedef std::list<XNode>::iterator iterator;
			typedef std::list<XNode>::const_iterator const_iterator;

		public:
			XElementContainer const* const	operator->() const	{return this;}
			XElementContainer*				operator->()		{return this;}

			const bool exist(const XName& xname) const;
			XElement operator[] (const XName& xname);
			const XElement operator[] (const XName& xname) const;

			XElement get(const XName& xname, const std::string& value);
			const XElement get(const XName& xname, const std::string& value) const;

			const XNamespaceContainer usedNamespace() const;

		public:
			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;

		private:
			NSCommon::smart_ptr<std::list<XNode> >	m_container;
		};

	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XELEMENT_CONTAINER_INCLUDE_H_