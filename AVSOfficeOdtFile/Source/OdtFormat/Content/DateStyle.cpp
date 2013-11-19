
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "DateStyle.h"


namespace Odt
{
	namespace Content
	{

		DateStyles::DateStyles()
		{
		}


		DateStyles::~DateStyles()
		{
		}


		DateStyles::DateStyles(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DateStyles& DateStyles::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DateStyles::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name = element.attribute("name").value();
			m_automaticOrder = element.attribute("automatic-order").value();
			m_day.fromXML(element.element("day"));
			m_month.fromXML(element.element("month"));
			m_year.fromXML(element.element("year"));
			XML::Fill(m_separators, element, "text");
		}


		const XML::XNode DateStyles::toXML() const
		{
			return 
				XML::XElement(ns.number + "date-style",
					XML::XAttribute(ns.style + "name", m_name) + 
					XML::XAttribute(ns.number  + "automatic-order", m_automaticOrder) + 
					XML::Write(m_day) + 
					XML::Write(m_separators[0]) + 
					XML::Write(m_month) + 
					XML::Write(m_separators[1]) + 
					XML::Write(m_year)
				);
		}

	} // namespace Content
} // namespace Odt