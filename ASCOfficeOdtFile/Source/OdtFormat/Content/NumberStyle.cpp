
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "NumberStyle.h"


namespace Odt
{
	namespace Content
	{

		NumberStyle::NumberStyle()
		{
		}


		NumberStyle::~NumberStyle()
		{
		}


		NumberStyle::NumberStyle(const XML::XNode& node)
		{
			fromXML(node);
		}

		const NumberStyle& NumberStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NumberStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name = element.attribute("name").value();
			m_number = element.element("number");
		}


		const XML::XNode NumberStyle::toXML() const
		{
			return
				XML::XElement(ns.number + "number-style",
					XML::XAttribute(ns.style + "name", m_name) + 
					XML::Write(m_number)
				);
		}

	} // namespace Content
} // namespace Odt