
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "LinenumberingConfiguration.h"


namespace Odt
{
	namespace Styles
	{

		LinenumberingConfiguration::LinenumberingConfiguration()
		{
		}


		LinenumberingConfiguration::~LinenumberingConfiguration()
		{
		}

	
		LinenumberingConfiguration::LinenumberingConfiguration(const XML::XNode& node)
		{
			fromXML(node);
		}


		const LinenumberingConfiguration& LinenumberingConfiguration::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void LinenumberingConfiguration::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_numberLines = element.attribute("number-lines").value();
			m_offset			=	element.attribute("offset").value();
			m_numFormat		=	element.attribute("num-format").value();
			m_numberPosition	= element.attribute("number-position").value();
			m_increment				=	element.attribute("increment").value();
		}


		const XML::XNode LinenumberingConfiguration::toXML() const
		{
			return 
				XML::XElement(ns.text + "linenumbering-configuration",
					XML::XAttribute(ns.text + "number-lines", m_numberLines) +
					XML::XAttribute(ns.text + "offset", m_offset) +
					XML::XAttribute(ns.style + "num-format", m_numFormat) +
					XML::XAttribute(ns.text + "number-position", m_numberPosition) +
					XML::XAttribute(ns.text + "increment", m_increment)
				);
		}

	} // namespace Styles
} // namespace Odt