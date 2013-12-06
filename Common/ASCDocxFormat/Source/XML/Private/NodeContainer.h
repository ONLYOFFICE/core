#pragma once
#ifndef XML_PRIVATE_NODE_CONTAINER_INCLUDE_H_
#define XML_PRIVATE_NODE_CONTAINER_INCLUDE_H_

#include "Node.h"
#include <string>
#include "XNodeContainer.h"
#include "XElementContainer.h"
#include "XTextContainer.h"
#include "property.h"
#include "XSpace.h"
#include "XSingleSource.h"
#include "XWideSource.h"
#include <list>

namespace XML
{
	class XElement;
	class XComment;
	class XText;

	namespace Private
	{
		class XNamespaceContainer;
		class XList;

		class NodeContainer : public Node
		{
		public:
			NodeContainer();
			NodeContainer(const XNodeContainer& nodes);
			virtual ~NodeContainer();

		public:
			explicit NodeContainer(const XElement& element);
			explicit NodeContainer(const XText& text);
			explicit NodeContainer(const XComment& comment);
			explicit NodeContainer(const XList& list);

		public:
			virtual const bool isElement() const;
			virtual const bool isText() const;
			virtual const bool isComment() const;

		public:
			void fromSource(boost::shared_ptr<XSingleSource> source, const XNamespaceContainer& ns, const XSpace& space);
			void fromSource(boost::shared_ptr<XWideSource> source, const XNamespaceContainer& ns, const XSpace& space);
			virtual const std::string ToString() const;
			virtual const std::wstring ToWString() const;
			virtual void SaveToStringList(std::list<std::string>& strList)const;
			virtual void SaveToWStringList(std::list<std::wstring>& strList)const;

		public:
			template<template<typename T, typename A> class C, typename T, typename A>
			explicit NodeContainer(const C<T, A>& container)
				: Nodes(),
					Elements(Nodes),
					Texts(Nodes)
			{
				for (C<T, A>::const_iterator i = container.begin(); i != container.end(); ++i)
					Nodes.push_back(Write(*i));
			}

		public:
			void Add(const XNode& node);
			void Add(const XElement& node);
			void Add(const XText& text);
			void Add(const XComment& comment);
			void Add(const XList& list);

		private:
			const bool isComment(boost::shared_ptr<XSingleSource> source);
			void insertComment(boost::shared_ptr<XSingleSource> source);
			const bool isComment(boost::shared_ptr<XWideSource> source);
			void insertComment(boost::shared_ptr<XWideSource> source);

		public:
			XNodeContainer		Nodes;
			XElementContainer	Elements;
			XTextContainer		Texts;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_NODE_CONTAINER_INCLUDE_H_