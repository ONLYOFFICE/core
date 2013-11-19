#pragma once
#ifndef XML_XCONTAINER_INCLUDE_H_
#define XML_XCONTAINER_INCLUDE_H_

#include "property.h"
#include "Private/XPointer.h"
#include "Private/NodeContainer.h"


namespace XML
{
	class XNode;
	class XElement;
	class XText;

	namespace Private
	{
		class XList;
	}

	class XContainer : public Private::XPointer<Private::NodeContainer>
	{
	public:
		XContainer();
		XContainer(const XNode& node);

	public:
		explicit XContainer(const XElement& element);
		explicit XContainer(const XText& text);
		explicit XContainer(const Private::XList& list);

	public:
		template<template<typename T, typename A> class C, class S, class G, class T, class A>
		explicit XContainer(const property<C<T, A>, S, G>& container)
			: base(new Private::NodeContainer(*container))
		{
		}

		template<template<typename T, typename A> class C, typename T, typename A>
		explicit XContainer(const C<T, A>& container)
			: base(new Private::NodeContainer(container))
		{
		}

	public:
		void Add(const XNode& node);
		void Add(const XElement& element);
		void Add(const XText& text);
		void Add(const Private::XList& list);
	};
} // namespace XML

#endif // XML_XCONTAINER_INCLUDE_H_