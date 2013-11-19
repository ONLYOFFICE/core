
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "OutlineLevelStyle.h"


namespace Odt
{
	namespace Styles
	{

		OutlineLevelStyle::OutlineLevelStyle()
		{
		}


		OutlineLevelStyle::~OutlineLevelStyle()
		{
		}


		OutlineLevelStyle::OutlineLevelStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const OutlineLevelStyle& OutlineLevelStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void OutlineLevelStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_level				=	element.attribute("level").value();
			m_numFormat		=	element.attribute("num-format").value();
			m_properties	= element.element("list-level-properties");

		}


		const XML::XNode OutlineLevelStyle::toXML() const
		{
			return 
				XML::XElement(ns.text + "outline-level-style",
					XML::XAttribute(ns.text + "level", m_level) +
					XML::XAttribute(ns.style + "num-format", m_numFormat) +
					XML::Write(m_properties)
				);
		}

	} // namespace Styles
} // namespace Odt