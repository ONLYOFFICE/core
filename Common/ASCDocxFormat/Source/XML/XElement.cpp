
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XElement.h"
#include "XName.h"
#include "Private/XList.h"
#include "XDocument.h"
#include "Private/XNamespaceContainer.h"
#include "Private/XAttributeContainer.h"
#include "Private/XNodeContainer.h"
#include "Private/XElementContainer.h"
#include "XNode.h"
#include "XText.h"
#include <boost/shared_ptr.hpp>
#include "Private/NullXElement.h"
#include "Private/NullElement.h"
#include "XAttribute.h"
#include "Exception/not_implement.h"


namespace XML
{

	XElement::XElement()
		: base(new Private::Element()),
			XName(m_ptr->XName),
			Namespaces(m_ptr->Namespaces),
			Attributes(m_ptr->Attributes),
			Nodes(m_ptr->Nodes),
			Elements(m_ptr->Elements),
			Texts(m_ptr->Texts),
			Space(m_ptr->Space)
	{
	}


	XElement::XElement(const Private::NullXElement&)
		: base(new Private::NullElement()),
			XName(m_ptr->XName),
			Namespaces(m_ptr->Namespaces),
			Attributes(m_ptr->Attributes),
			Nodes(m_ptr->Nodes),
			Elements(m_ptr->Elements),
			Texts(m_ptr->Texts),
			Space(m_ptr->Space)
	{
	}


	XElement::XElement(const XML::XName& xname)
		: base(new Private::Element(xname)),
			XName(m_ptr->XName),
			Namespaces(m_ptr->Namespaces),
			Attributes(m_ptr->Attributes),
			Nodes(m_ptr->Nodes),
			Elements(m_ptr->Elements),
			Texts(m_ptr->Texts),
			Space(m_ptr->Space)
	{
	}


	XElement::XElement(const XML::XName& xname, const Private::XList& list)
		: base(new Private::Element(xname, list)),
			XName(m_ptr->XName),
			Namespaces(m_ptr->Namespaces),
			Attributes(m_ptr->Attributes),
			Nodes(m_ptr->Nodes),
			Elements(m_ptr->Elements),
			Texts(m_ptr->Texts),
			Space(m_ptr->Space)
	{
	}


	XElement::XElement(boost::shared_ptr<Private::XSingleSource> source, const Private::XNamespaceContainer& ns, const Private::XSpace& space)
		: base(new Private::Element(source, ns, space)),
			XName(m_ptr->XName),
			Namespaces(m_ptr->Namespaces),
			Attributes(m_ptr->Attributes),
			Nodes(m_ptr->Nodes),
			Elements(m_ptr->Elements),
			Texts(m_ptr->Texts),
			Space(m_ptr->Space)
	{
	}


	XElement::XElement(boost::shared_ptr<Private::XWideSource> source, const Private::XNamespaceContainer& ns, const Private::XSpace& space)
		: base(new Private::Element(source, ns, space)),
			XName(m_ptr->XName),
			Namespaces(m_ptr->Namespaces),
			Attributes(m_ptr->Attributes),
			Nodes(m_ptr->Nodes),
			Elements(m_ptr->Elements),
			Texts(m_ptr->Texts),
			Space(m_ptr->Space)
	{
	}


	const XElement& XElement::operator =(const XElement& rhs)
	{
		m_ptr = rhs.m_ptr;
		XName = rhs.XName;
		Namespaces = rhs.Namespaces;
		Attributes = rhs.Attributes;
		Nodes = rhs.Nodes;
		Elements = rhs.Elements;
		Texts = rhs.Texts;
		Space = rhs.Space;
		return *this;
	}


	XElement::XElement(const XNode& xnode)
		: base(boost::dynamic_pointer_cast<Private::Element>(xnode.get_ptr())),
			XName(m_ptr->XName),
			Namespaces(m_ptr->Namespaces),
			Attributes(m_ptr->Attributes),
			Nodes(m_ptr->Nodes),
			Elements(m_ptr->Elements),
			Texts(m_ptr->Texts),
			Space(m_ptr->Space)
	{
	}

	void XElement::Save(const OOX::CPath& path) const
	{
		XDocument(*this).Save(path);
	}


	void XElement::fromSource(boost::shared_ptr<Private::XSingleSource> source, const Private::XNamespaceContainer& ns, const Private::XSpace& space)
	{
		m_ptr->fromSource(source, ns, space);
	}


	void XElement::fromSource(boost::shared_ptr<Private::XWideSource> source, const Private::XNamespaceContainer& ns, const Private::XSpace& space)
	{
		m_ptr->fromSource(source, ns, space);
	}


	const std::string XElement::ToString() const 
	{
		return m_ptr->ToString();
	}


	const std::wstring XElement::ToWString() const
	{
		return m_ptr->ToWString();
	}


	void XElement::SaveToStringList(std::list<std::string>& strList)const
	{
		m_ptr->SaveToStringList(strList);
	}


	void XElement::SaveToWStringList(std::list<std::wstring>& strList)const
	{
		m_ptr->SaveToWStringList(strList);
	}


	const bool XElement::exist() const
	{
		return m_ptr->exist();
	}


	const Private::XNamespaceContainer XElement::usedNamespace() const
	{
		return m_ptr->usedNamespace();
	}


	const Private::XString XElement::text() const
	{
		return m_ptr->text();
	}


	void XElement::Add(const XAttribute& attribute)
	{
		m_ptr->Add(attribute);
	}


	void XElement::Add(const XNamespace& ns)
	{
		m_ptr->Add(ns);
	}


	void XElement::Add(const XNode& node)
	{
		m_ptr->Add(node);
	}


	void XElement::Add(const XElement& element)
	{
		m_ptr->Add(element);
	}


	void XElement::Add(const XText& text)
	{
		m_ptr->Add(text);
	}


	void XElement::Add(const XContainer& container)
	{
		m_ptr->Add(container);
	}


	void XElement::Add(const Private::XList& list)
	{
		m_ptr->Add(list);
	}


	XAttribute& XElement::attribute(const XML::XName& xname)
	{
		return m_ptr->attribute(xname);
	}


	const XAttribute& XElement::attribute(const XML::XName& xname) const
	{
		return m_ptr->attribute(xname);
	}


	XElement XElement::element(const XML::XName& xname)
	{
		return m_ptr->element(xname);
	}


	const XElement XElement::element(const XML::XName& xname) const
	{
		return m_ptr->element(xname);
	}


	XElement XElement::element(const XML::XName& xname, const std::string& value)
	{
		return m_ptr->element(xname, value);
	}


	const XElement XElement::element(const XML::XName& xname, const std::string& value) const
	{
		return m_ptr->element(xname, value);
	}

} // namespace XML