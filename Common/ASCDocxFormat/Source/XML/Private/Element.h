#pragma once
#ifndef XML_PRIVATE_ELEMENT_INCLUDE_H_
#define XML_PRIVATE_XELEMENT_INCLUDE_H_

#include <list>
#include <string>

#include "NodeContainer.h"
#include "property.h"
#include "XAttributeContainer.h"
#include "XNamespaceContainer.h"
#include "./../XName.h"
#include "XString.h"
#include "XSpace.h"
#include "XSingleSource.h"
#include "XWideSource.h"

namespace XML
{
	class XAttribute;
	class XNamespace;
	class XNode;
	class XText;
	class XContainer;
	
	namespace Private
	{
		class XList;

		class Element : public NodeContainer
		{
		public:
			Element();
			Element(const XML::XName& xname);
			Element(const XML::XName& xname, const XList& list);
			Element(NSCommon::smart_ptr<XSingleSource> source, const XNamespaceContainer& ns, const XSpace& space);
			Element(NSCommon::smart_ptr<XWideSource> source, const XNamespaceContainer& ns, const XSpace& space);

			virtual ~Element();

		public:
			virtual const bool isElement() const;
			virtual const bool isText() const;
			virtual const bool isComment() const;

		public:
			void fromSource(NSCommon::smart_ptr<XSingleSource> source, const XNamespaceContainer& defineNamespaces, const XSpace& space);
			void fromSource(NSCommon::smart_ptr<XWideSource> source, const XNamespaceContainer& defineNamespaces, const XSpace& space);
			virtual const std::string ToString() const;
			virtual const std::wstring ToWString() const;
			virtual void SaveToStringList(std::list<std::string>& strList)const;
			virtual void SaveToWStringList(std::list<std::wstring>& strList)const;

		public:
			virtual const bool exist() const;
			virtual const XString text() const;
			const XNamespaceContainer usedNamespace() const;

		public:
			void Add(const XAttribute& attribute);
			void Add(const XNamespace& ns);
			void Add(const XNode& node);
			void Add(const XElement& element);
			void Add(const XText& text);
			void Add(const XML::XContainer& container);
			void Add(const XML::Private::XList& list);

		public:
			XAttribute& attribute(const XName& xname);
			const XAttribute& attribute(const XName& xname) const;

			XElement element(const XName& xname);
			const XElement element(const XName& xname) const;

			XElement element(const XName& xname, const std::string& value);
			const XElement element(const XName& xname, const std::string& value) const;

		public:
			property<XName>						XName;
			XAttributeContainer				Attributes;
			XNamespaceContainer				Namespaces;
			property<XSpace>					Space;
		};
	} // namespace Private
} // namespace XML

#endif // XML_XELEMENT_INCLUDE_H_