
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Space.h"


namespace Odt
{
	namespace Content
	{

		Space::Space()
			: Count(1)
		{
		}


		Space::~Space()
		{
		}


		Space::Space(const size_t count)
			: Count(count)
		{
		}


		Space::Space(const XML::XNode& node)
		{
			fromXML(node);
		}


		Space::Space(const std::string& text)
		{
			fromTxt(text);
		}


		const Space& Space::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const Space& Space::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void Space::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			if (element.attribute("c").exist())
				Count = element.attribute("c").value();
			else
				Count = 1;
		}


		void Space::fromTxt(const std::string& text)
		{
			Count = text.size();
		}


		const XML::XNode Space::toXML() const
		{
			return XML::XElement(ns.text + "s",	XML::XAttribute(ns.text + "c", Count));
		}


		const std::string Space::toTxt() const
		{
			return std::string(Count, ' ');
		}

	} // namespace Content
} // namespace Odt