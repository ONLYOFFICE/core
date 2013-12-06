#pragma once
#ifndef XML_XELEMENT_INCLUDE_H_
#define XML_XELEMENT_INCLUDE_H_

#include <string>
#include "Private/XPointer.h"
#include "Private/Element.h"
#include "property.h"
#include "setter.h"
#include "getter.h"
#include <boost/shared_ptr.hpp>
#include "Private/XString.h"
#include "Private/XNamespaceContainer.h"
#include "Private/XSpace.h"
#include "Private/XSingleSource.h"
#include "Private/XWideSource.h"
#include <list>


namespace XML
{
	class XAttribute;
	class XNamespace;
	class XName;
	class XNode;
	class XText;
	class XContainer;
	class XObject;

	namespace Private
	{
		class XList;
		class XAttributeContainer;
		class XNodeContainer;
		class XElementContainer;
		class XTextContainer;
		class NullXElement;
	}

	class XElement : public Private::XPointer<Private::Element>
	{
	public:
		XElement();
		XElement(const Private::NullXElement&);
		explicit XElement(const XName& xname);
		XElement(const XName& xname, const Private::XList& list);
		XElement(boost::shared_ptr<Private::XSingleSource> source, const Private::XNamespaceContainer& ns, const Private::XSpace& space);
		XElement(boost::shared_ptr<Private::XWideSource> source, const Private::XNamespaceContainer& ns, const Private::XSpace& space);
		const XElement& operator= (const XElement& rhs);
		XElement(const XNode& xnode);

	public:
		template<class T>
		operator const nullable__<T>() const
		{
			if (exist())
				return T(*this);
			return nullable__<T>();
		}

		template<class T, class S, class G>
		operator const property<T, S, G>() const
		{
			return T(*this);
		}

		template<class T, class S, class G>
		operator const nullable_property<T, S, G>() const
		{
			if (exist())
				return T(*this);
			return nullable_property<T, S, G>();
		}

	public:
		XElement const* const	operator->() const	{return this;}
		XElement*							operator->()				{return this;}

	public:
		void Save(const OOX::CPath& path) const;

	public:
		void fromSource(boost::shared_ptr<Private::XSingleSource> source, const Private::XNamespaceContainer& ns, const Private::XSpace& space);
		void fromSource(boost::shared_ptr<Private::XWideSource> source, const Private::XNamespaceContainer& ns, const Private::XSpace& space);
		const std::string ToString() const;
		const std::wstring ToWString() const;
		virtual void SaveToStringList(std::list<std::string>& strList)const;
		virtual void SaveToWStringList(std::list<std::wstring>& strList)const;

	public:
		const bool exist() const;
		const Private::XString text() const;
		const Private::XNamespaceContainer usedNamespace() const;

	public:
		void Add(const XAttribute& attribute);
		void Add(const XNamespace& ns);
		void Add(const XNode& node);
		void Add(const XElement& element);
		void Add(const XText& text);
		void Add(const XContainer& container);
		void Add(const Private::XList& list);

	public:
		XAttribute& attribute(const XName& xname);
		const XAttribute& attribute(const XName& xname) const;

		XElement element(const XName& xname);
		const XElement element(const XName& xname) const;

		XElement element(const XName& xname, const std::string& value);
		const XElement element(const XName& xname, const std::string& value) const;

	public:
		property<XName>&							XName;
		Private::XNamespaceContainer& Namespaces;
		Private::XAttributeContainer&	Attributes;
		Private::XNodeContainer&			Nodes;
		Private::XElementContainer&		Elements;
		Private::XTextContainer&			Texts;
		property<Private::XSpace>&		Space;
	};
} // namespace XML


template<typename T>
const nullable__<T>& nullable_setter(nullable__<T>& lhs, const XML::XElement& rhs)
{
	return ::nullable_setter(lhs, nullable__<T>(rhs));
}


template<typename T, class S, class G>
const property<T, S, G>& property_setter(property<T, S, G>& lhs, const XML::XElement& rhs)
{
	return ::property_setter(lhs, T(rhs));
}


template<typename T, class S, class G>
const nullable_property<T, S, G>& nullable_property_setter(nullable_property<T, S, G>& lhs, const XML::XElement& rhs)
{
	return ::nullable_property_setter(lhs, nullable_property<T, S, G>(rhs));
}


#endif // XML_XELEMENT_INCLUDE_H_