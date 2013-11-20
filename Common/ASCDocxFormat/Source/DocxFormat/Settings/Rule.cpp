
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Rule.h"


namespace OOX
{
	namespace Settings
	{

		Rule::Rule()
		{
		}


		Rule::~Rule()
		{
		}


		Rule::Rule(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Rule& Rule::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Rule::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_id	  =	element.attribute("id").value();
			m_type	  =	element.attribute("type").value();
			m_idref	  =	element.attribute("idref").value();
		}


		const XML::XNode Rule::toXML() const
		{
			return 
				XML::XElement(ns.o + "r",
					XML::XAttribute("id", m_id) +
					XML::XAttribute("type", m_type) +
					XML::XAttribute("idref", m_idref)
				);
		}

	} // namespace Settings
} // namespace OOX