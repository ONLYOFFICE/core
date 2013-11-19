
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "DateDay.h"


namespace Odt
{
	namespace Content
	{

		DateDay::DateDay()
		{
		}


		DateDay::~DateDay()
		{
		}


		DateDay::DateDay(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DateDay& DateDay::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DateDay::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_style = element.attribute("style").value();
		}


		const XML::XNode DateDay::toXML() const
		{
			return 
				XML::XElement(ns.number + "day", 
					XML::XAttribute(ns.number + "style", m_style)
				);
		}

	} // namespace Content
} // namespace Odt