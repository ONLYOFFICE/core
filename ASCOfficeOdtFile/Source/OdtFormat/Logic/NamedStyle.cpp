
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "NamedStyle.h"


namespace Odt
{
	namespace Logic
	{

		NamedStyle::NamedStyle()
		{
		}


		NamedStyle::~NamedStyle()
		{
		}
	

		NamedStyle::NamedStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NamedStyle& NamedStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NamedStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name					=	element.attribute("name").value();
			DisplayName		=	element.attribute("display-name").value();
			Family				=	element.attribute("family").value();
			ParentName		=	element.attribute("parent-style-name").value();
			NextName			=	element.attribute("next-style-name").value();
			MasterPageName= element.attribute("master-page-name").value();
			Class					=	element.attribute("class").value();
			ListStyleName = element.attribute("list-style-name").value();
			DefaultOutlineLevel = element.attribute("default-outline-level").value();
			Properties = element;
		}


		const XML::XNode NamedStyle::toXML() const
		{
			return 
				XML::XElement(ns.style + "style", 
					XML::XAttribute(ns.style + "name", Name) + 
					XML::XAttribute(ns.style + "display-name", DisplayName) +
					XML::XAttribute(ns.style + "family", Family) +
					XML::XAttribute(ns.style + "parent-style-name", ParentName) +
					XML::XAttribute(ns.style + "next-style-name", NextName) +
					XML::XAttribute(ns.style + "master-page-name", MasterPageName) +
					XML::XAttribute(ns.style + "list-style-name", ListStyleName) +
					XML::XAttribute(ns.style + "class", Class) +
					XML::XAttribute(ns.style + "default-outline-level", DefaultOutlineLevel) +
					XML::Write(Properties)
				);
		}


		const bool NamedStyle::isSimple() const
		{
			/*if (Name == "Standard")
				return false;*/
			if (ParentName.is_init()/* && ParentName != "Standard"*/)
				return false;
			if (!Properties->isSimple())
				return false;
			return true;
		}

	} // namespace Logic
} // namespace Odt