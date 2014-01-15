#pragma once
#ifndef XML_PRIVATE_XNAMESPACE_CONTAINER_INCLUDE_H_
#define XML_PRIVATE_XNAMESPACE_CONTAINER_INCLUDE_H_

#include "XContainer.h"
#include "./../XNamespace.h"
#include <string>


namespace XML
{
	namespace Private
	{
		class XNamespaceContainer : public XContainer<XNamespace>
		{
		public:
			const bool exist(const std::string& name) const;

			XNamespaceContainer const* const	operator->() const	{return this;}
			XNamespaceContainer*							operator->()				{return this;}

			XNamespace& operator[] (const std::string& name);
			const XNamespace& operator[] (const std::string& name) const;

			void erase(const std::string& name);
			void erase(const XNamespaceContainer& rhs);
		public:
			typedef std::list<XNamespace>::iterator iterator;
			typedef std::list<XNamespace>::const_iterator const_iterator;

		public:
			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_XNAMESPACE_CONTAINER_INCLUDE_H_