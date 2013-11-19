
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "PageNumber.h"
#include "ToString.h"


namespace Odt
{
	namespace Content
	{

		PageNumber::PageNumber()
		{
		}


		PageNumber::~PageNumber()
		{
		}


		PageNumber::PageNumber(const XML::XNode& node)
		{
			fromXML(node);
		}


		const PageNumber& PageNumber::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void PageNumber::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			SelectPage = element.attribute("select-page").value();
			Value			 = element.text();
			NumFormat	 = element.attribute("num-format").value();
		}


		const XML::XNode PageNumber::toXML() const
		{
			return
				XML::XElement(ns.text + "page-number",
					XML::XAttribute(ns.text + "select-page", SelectPage) +
					XML::XAttribute(ns.style + "num-format", NumFormat) +
					XML::XText(Value)
				);
		}


		const std::string PageNumber::toTxt() const
		{
			return Value.ToString();
		}

	} // namespace Content
} // namespace Odt