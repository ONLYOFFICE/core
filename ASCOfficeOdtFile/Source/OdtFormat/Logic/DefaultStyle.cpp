
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "DefaultStyle.h"


namespace Odt
{
	namespace Logic
	{

		DefaultStyle::DefaultStyle()
		{
		}


		DefaultStyle::~DefaultStyle()
		{
		}
	

		DefaultStyle::DefaultStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DefaultStyle& DefaultStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DefaultStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Family			=	element.attribute("family").value();
			Properties	= element;
		}


		const XML::XNode DefaultStyle::toXML() const
		{
			return 
				XML::XElement(ns.style + "default-style", 
					XML::XAttribute(ns.style + "family", Family) +
					XML::Write(Properties)
				);
		}

	} // namespace Logic
} // namespace Odt