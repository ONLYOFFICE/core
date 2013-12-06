
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Element.h"

#include <utility>
#include <vector>

#include "XList.h"

#include "./../Exception/Parse.h"
#include "./../Exception/Namespace.h"
#include "Namespace.h"
#include "./../XAttribute.h"
#include "./../XAttribute.h"
#include "./../XNode.h"
#include "./../XElement.h"
#include "./../XText.h"
#include "./../XContainer.h"
#include "Exception/not_implement.h"
#include "Utility.h"

namespace XML
{
	namespace Private
	{

		Element::Element()
		{
		}


		Element::Element(const XML::XName& xname)
			: XName(xname)
		{
		}


		Element::Element(const XML::XName& xname, const XList& list)
			: NodeContainer(list.Nodes),
				XName(xname),
				Attributes(list.Attributes),
				Namespaces(list.Namespaces)
		{
		}


		Element::Element(NSCommon::smart_ptr<XSingleSource> source, const XNamespaceContainer& ns, const XSpace& space)
			: Space(space)
		{
			fromSource(source, ns, Space);
		}


		Element::Element(NSCommon::smart_ptr<XWideSource> source, const XNamespaceContainer& ns, const XSpace& space)
			: Space(space)
		{
			fromSource(source, ns, Space);
		}


		Element::~Element()
		{
		}


		const bool Element::isElement() const
		{
			return true;
		}


		const bool Element::isText() const
		{
			return false;
		}


		const bool Element::isComment() const
		{
			return false;
		}


		void Element::fromSource(NSCommon::smart_ptr<XSingleSource> source, const XNamespaceContainer& defineNamespaces, const XSpace& space)
		{
			Space = space;
			source->skipSpace();

			const std::pair<const std::string, const std::string> name = source->getName();
			source->skipSpace();
			std::list<std::pair<std::pair<std::string, std::string>, std::string> > attributes;

			while (source->get() != '/' && source->get() != '>')
			{
				const std::pair<const std::pair<const std::string, const std::string>, const std::string> attribute = source->getAttribute();

				if (attribute.first.first == "xmlns" || attribute.first.first.empty() && attribute.first.second == "xmlns")
				{
					Namespaces->push_back(XNamespace(attribute.first.second, attribute.second));
				}
				else if (attribute.first.first == "xml")
				{
					if (attribute.first.second == "space")
					{
						if (attribute.second == "default")
							Space->setDefault();
						else if (attribute.second == "preserve")
							Space->setPreserve();
						else
							throw Exception::Parse("bad space parametr");
					}
				}
				else
				{
					attributes.push_back(attribute);
				}
				source->skipSpace();
			}

			typedef std::pair<std::pair<std::string, std::string>, std::string> attribute_type;

			for (std::list<attribute_type>::iterator iter = attributes.begin();	iter != attributes.end();++iter)
			{
				if ((*iter).first.first.empty())
				{
					Attributes.push_back(XAttribute((*iter).first.second, (*iter).second));
				}
				else
				{
					XNamespace thisNs = Namespaces[(*iter).first.first];
					XNamespace defineNs = defineNamespaces[(*iter).first.first];

					if (thisNs.exist())
						Attributes.push_back(XAttribute(thisNs + (*iter).first.second, (*iter).second));
					else if (defineNs.exist())
						Attributes.push_back(XAttribute(defineNs + (*iter).first.second, (*iter).second));
					else
					{
						//Attributes.push_back(XAttribute((*iter).first.second, (*iter).second));
						//throw Exception::Namespace("not define namespace");
					}
				}
			}

			XName->Name = name.second;

			if (!name.first.empty())
			{
				XNamespace thisNs = Namespaces[name.first];
				XNamespace defineNs = defineNamespaces[name.first];

				if (thisNs.exist())
					XName->Ns = thisNs;
				else if (defineNs.exist())
					XName->Ns = defineNs;
				else
				{
					//throw Exception::Namespace("not define namespace");
				}
			}

			if (source->get() == '/')
			{
				source->findAndSkip('>');
			}
			else
			{
				source->findAndSkip('>');
				XNamespaceContainer summaryNamespaces = Namespaces;
				summaryNamespaces.merge(defineNamespaces);
				NodeContainer::fromSource(source, summaryNamespaces, Space);
				source->skipSpace();
				const std::pair<const std::string, const std::string> name = source->getName();
				source->findAndSkip('>');
			}
		}


		void Element::fromSource(NSCommon::smart_ptr<XWideSource> source, const XNamespaceContainer& defineNamespaces, const XSpace& space)
		{
			Space = space;
			source->skipSpace();

			const std::pair<const std::wstring, const std::wstring> name = source->getName();
			source->skipSpace();
			std::list<std::pair<std::pair<std::wstring, std::wstring>, std::wstring> > attributes;

			while (source->get() != L'/' && source->get() != L'>')
			{
				const std::pair<const std::pair<const std::wstring, const std::wstring>, const std::wstring> attribute = source->getAttribute();

				if ((attribute.first.first == L"xmlns") || ((attribute.first.first.empty()) && (attribute.first.second == L"xmlns")))
				{
					Namespaces->push_back(XNamespace(Encoding::unicode2utf8(attribute.first.second), Encoding::unicode2utf8(attribute.second)));
				}
				else if (attribute.first.first == L"xml")
				{
					if (attribute.first.second == L"space")
					{
						if (attribute.second == L"default")
							Space->setDefault();
						else if (attribute.second == L"preserve")
							Space->setPreserve();
						else
							throw Exception::Parse("bad space parametr");
					}
				}
				else
				{
					attributes.push_back(attribute);
				}
				source->skipSpace();
			}

			typedef std::pair<std::pair<std::wstring, std::wstring>, std::wstring> attribute_type;
		
			for (std::list<attribute_type >::iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
			{
				if ((*iter).first.first.empty())
				{
					Attributes.push_back(XAttribute(Encoding::unicode2utf8((*iter).first.second), Encoding::unicode2utf8((*iter).second)));
				}
				else
				{
					XNamespace thisNs = Namespaces[Encoding::unicode2utf8((*iter).first.first)];
					XNamespace defineNs = defineNamespaces[Encoding::unicode2utf8((*iter).first.first)];

					if (thisNs.exist())
						Attributes.push_back(XAttribute(thisNs + Encoding::unicode2utf8((*iter).first.second), Encoding::unicode2utf8((*iter).second)));
					else if (defineNs.exist())
						Attributes.push_back(XAttribute(defineNs + Encoding::unicode2utf8((*iter).first.second), Encoding::unicode2utf8((*iter).second)));
					else
						throw Exception::Namespace("not define namespace");
				}
			}

			XName->Name = Encoding::unicode2utf8(name.second);

			if (!name.first.empty())
			{
				XNamespace thisNs = Namespaces[Encoding::unicode2utf8(name.first)];
				XNamespace defineNs = defineNamespaces[Encoding::unicode2utf8(name.first)];

				if (thisNs.exist())
					XName->Ns = thisNs;
				else if (defineNs.exist())
					XName->Ns = defineNs;
				else
					throw Exception::Namespace("not define namespace");
			}

			if (source->get() == L'/')
			{
				source->findAndSkip(L'>');
			}
			else
			{
				source->findAndSkip(L'>');
				XNamespaceContainer summaryNamespaces = Namespaces;
				summaryNamespaces.merge(defineNamespaces);
				NodeContainer::fromSource(source, summaryNamespaces, Space);
				source->skipSpace();
				const std::pair<const std::wstring, const std::wstring> name = source->getName();
				source->findAndSkip(L'>');
			}
		}


		const std::string Element::ToString() const
		{
			std::string node;
			node += "<" + XName->ToString();
			if (Space->isPreserve())
				node += " xml:space=\"preserve\" ";
			node += Namespaces->ToString();
			node += Attributes->ToString();

			if (Nodes->empty())
				node += "/>";
			else
				node += ">" +  Nodes->ToString() + "</" + XName->ToString() + ">";

			return node;
		}


		const std::wstring Element::ToWString() const
		{
			std::wstring node;
			node += L"<" + XName->ToWString();
			if (Space->isPreserve())
				node += L" xml:space=\"preserve\" ";
			node += Namespaces->ToWString();
			node += Attributes->ToWString();

			if (Nodes->empty())
				node += L"/>";
			else
				node += L">" +  Nodes->ToWString() + L"</" + XName->ToWString() + L">";

			return node;
		}


		void Element::SaveToStringList(std::list<std::string>& strList)const
		{
			//const std::string name = XName->ToString();
			strList.push_back("<");// + name);
			XName->SaveToStringList(strList);
			//strList.push_back(name);
			if (Space->isPreserve())
				strList.push_back(" xml:space=\"preserve\" ");
			Namespaces->SaveToStringList(strList);
			Attributes->SaveToStringList(strList);

			if (Nodes->empty())
				strList.push_back("/>");
			else
			{
				strList.push_back(">");
				Nodes->SaveToStringList(strList);
				strList.push_back("</");// + name + ">");
				XName->SaveToStringList(strList);
				strList.push_back(">");
			}
		}


		void Element::SaveToWStringList(std::list<std::wstring>& strList)const
		{
			//const std::string name = XName->ToString();
			strList.push_back(L"<");// + name);
			XName->SaveToWStringList(strList);
			//strList.push_back(name);
			if (Space->isPreserve())
				strList.push_back(L" xml:space=\"preserve\" ");
			Namespaces->SaveToWStringList(strList);
			Attributes->SaveToWStringList(strList);

			if (Nodes->empty())
				strList.push_back(L"/>");
			else
			{
				strList.push_back(L">");
				Nodes->SaveToWStringList(strList);
				strList.push_back(L"</");// + name + ">");
				XName->SaveToWStringList(strList);
				strList.push_back(L">");
			}
		}


		const bool Element::exist() const
		{
			return true;
		}


		const XString Element::text() const
		{
			return Texts->text();
		}


		void Element::Add(const XAttribute& attribute)
		{
			Attributes.Add(attribute);
		}


		void Element::Add(const XNamespace& ns)
		{
			Namespaces.Add(ns);
		}


		void Element::Add(const XNode& node)
		{
			Nodes.Add(node);
		}


		void Element::Add(const XElement& element)
		{
			Nodes.Add(element);
		}


		void Element::Add(const XText& text)
		{
			Nodes.Add(text);
		}


		void Element::Add(const XML::XContainer& container)
		{
			Nodes.Add(container);
		}


		void Element::Add(const XML::Private::XList& list)
		{
			Attributes->merge(list.Attributes);
			Namespaces->merge(list.Namespaces);
			Nodes->merge(list.Nodes);
		}


		const XNamespaceContainer Element::usedNamespace() const
		{
			XNamespaceContainer container;
			if (XName->Ns.is_init())
				container.push_back(XName->Ns);

			container.merge(Attributes.usedNamespace());
			container.merge(Elements.usedNamespace());

			return container;
		}


		XAttribute& Element::attribute(const XML::XName& xname)
		{
			return Attributes[xname];
		}


		const XAttribute& Element::attribute(const XML::XName& xname) const
		{
			return Attributes[xname];
		}


		XElement Element::element(const XML::XName& xname)
		{
			return Elements[xname];
		}


		const XElement Element::element(const XML::XName& xname) const
		{
			return Elements[xname];
		}


		XElement Element::element(const XML::XName& xname, const std::string& value)
		{
			return Elements.get(xname, value);
		}


		const XElement Element::element(const XML::XName& xname, const std::string& value) const
		{
			return Elements.get(xname, value);
		}

	} // namespace Private
} // namespace XML