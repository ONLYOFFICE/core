
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "DateMonth.h"


namespace Odt
{
	namespace Content
	{

		DateMonth::DateMonth()
		{
		}


		DateMonth::~DateMonth()
		{
		}


		DateMonth::DateMonth(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DateMonth& DateMonth::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DateMonth::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_style = element.attribute("style").value();
		}


		const XML::XNode DateMonth::toXML() const
		{
			return 
				XML::XElement(ns.number + "month", 
					XML::XAttribute(ns.number + "style", m_style)
				);
		}

	} // namespace Content
} // namespace Odt