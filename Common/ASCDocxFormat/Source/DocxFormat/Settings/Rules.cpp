
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Rules.h"


namespace OOX
{
	namespace Settings
	{

		Rules::Rules()
		{
		}


		Rules::~Rules()
		{
		}


		Rules::Rules(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Rules& Rules::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Rules::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Fill(m_rules, element, "r");
			m_ext = element.attribute("ext").value();

		}


		const XML::XNode Rules::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Settings
} // namespace OOX