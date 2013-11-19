
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XList.h"
#include "./../XText.h"
#include "./../XElement.h"
#include "./../XComment.h" 
#include "./../XContainer.h"


namespace XML
{
	namespace Private
	{

		XList::XList()
		{
		}


		XList::XList(const XAttribute& attribute)
		{
			Attributes->push_back(attribute);
		}


		XList::XList(const XNamespace& ns)
		{
			Namespaces->push_back(ns);
		}


		XList::XList(const XText& text)
		{
			Nodes->push_back(text);
		}


		XList::XList(const XElement& element)
		{
			Nodes->push_back(element);
		}


		XList::XList(const XComment& comment)
		{
			Nodes->push_back(comment);
		}


		XList::XList(const XML::XContainer& container)
		{
			Nodes = container->Nodes;
		}


		XList::XList(const XNode& node)
		{
			Nodes->push_back(node);
		}


		XList& XList::Add(const XList& list)
		{
			Attributes->merge(list.Attributes);
			Namespaces->merge(list.Namespaces);
			Nodes->merge(list.Nodes);
			return *this;
		}


		XList& XList::Add(const XAttribute& attribute)
		{
			Attributes->push_back(attribute);
			return *this;
		}


		XList& XList::Add(const XNamespace& ns)
		{
			Namespaces->push_back(ns);
			return *this;
		}


		XList& XList::Add(const XText& text)
		{
			Nodes->push_back(text);
			return *this;
		}


		XList& XList::Add(const XElement& element)
		{
			Nodes->push_back(element);
			return *this;
		}


		XList& XList::Add(const XComment& comment)
		{
			Nodes->push_back(comment);
			return *this;
		}


		XList& XList::Add(const XML::XContainer& container)
		{
			Nodes->push_back(container);
			return *this;
		}


		XList& XList::Add(const XNode& node)
		{
			Nodes->push_back(node);
			return *this;
		}

	} // namespace Private
} // namespace XML