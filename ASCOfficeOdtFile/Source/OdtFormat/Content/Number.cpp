
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Number.h"


namespace Odt
{
	namespace Content
	{

		Number::Number()
		{
		}


		Number::~Number()
		{
		}


		Number::Number(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Number& Number::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Number::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_minIntegerDigits = element.attribute("min-integer-digits").value();
		}


		const XML::XNode Number::toXML() const
		{
			return 
				XML::XElement(ns.number + "number", 
					XML::XAttribute(ns.number + "min-integer-digits", m_minIntegerDigits)
				);
		}

	} // namespace Content
} // namespace Odt