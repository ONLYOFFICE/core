
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "DateText.h"


namespace Odt
{
	namespace Content
	{

		DateText::DateText()
		{
		}


		DateText::~DateText()
		{
		}


		DateText::DateText(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DateText& DateText::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DateText::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_text = element.text();
		}


		const XML::XNode DateText::toXML() const
		{
			return 
				XML::XElement(ns.number + "text", 
					XML::XText(m_text)
				);
		}

	} // namespace Content
} // namespace Odt