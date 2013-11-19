
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "OutlineStyle.h"


namespace Odt
{
	namespace Styles
	{

		OutlineStyle::OutlineStyle()
		{
		}


		OutlineStyle::~OutlineStyle()
		{
		}


		OutlineStyle::OutlineStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const OutlineStyle& OutlineStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void OutlineStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name = element.attribute("name").value();
			XML::Fill(m_list, element);
		}


		const XML::XNode OutlineStyle::toXML() const
		{
			return
				XML::XElement(ns.text + "outline-style",
					XML::XAttribute(ns.style + "name", m_name) +
					XML::Write(m_list)
				);
		}

	} // namespace Styles
} // namespaces Odt