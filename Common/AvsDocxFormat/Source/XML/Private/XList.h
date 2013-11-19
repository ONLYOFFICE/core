#pragma once
#ifndef XML_PRIVATE_XLIST_INCLUDE_H_
#define XML_PRIVATE_XLIST_INCLUDE_H_

#include "XAttributeContainer.h"
#include "XNamespaceContainer.h"
#include "XNodeContainer.h"
#include "property.h"


namespace XML
{
	class XText;
	class XElement;
	class XComment;
	class XContainer;
	class XNode;

	namespace Private
	{
		class XList
		{
		public:
			XList();
			XList(const XAttribute& attribute);
			XList(const XNamespace& ns);
			XList(const XText& text);
			XList(const XElement& element);
			XList(const XComment& comment);
			XList(const XML::XContainer& container);
			XList(const XNode& node);

		public:
			XList& Add(const XList& list);
			XList& Add(const XAttribute& attribute);
			XList& Add(const XNamespace& ns);
			XList& Add(const XText& text);
			XList& Add(const XElement& element);
			XList& Add(const XComment& comment);
			XList& Add(const XML::XContainer& container);
			XList& Add(const XNode& node);

		public:
			property<XAttributeContainer> Attributes;
			property<XNamespaceContainer> Namespaces;
			property<XNodeContainer>			Nodes;
		};
	} // namespace Private

	template<class T>
	Private::XList& operator ,(Private::XList& rhs, const T& lhs)
	{
		return rhs.Add(lhs);
	}

	template<class T>
	Private::XList operator ,(const XNamespace& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator ,(const XAttribute& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator ,(const XText& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator ,(const XElement& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator ,(const XComment& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator ,(const XContainer& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator ,(const XNode& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}


	template<class T>
	Private::XList& operator +(Private::XList& rhs, const T& lhs)
	{
		return rhs.Add(lhs);
	}

	template<class T>
	Private::XList operator +(const XAttribute& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator +(const XNamespace& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator +(const XText& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator +(const XElement& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator +(const XComment& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator +(const XContainer& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

	template<class T>
	Private::XList operator +(const XNode& rhs, const T& lhs)
	{
		return Private::XList(rhs).Add(lhs);
	}

} // namespace XML

#endif // XML_PRIVATE_XLIST_INCLUDE_H_