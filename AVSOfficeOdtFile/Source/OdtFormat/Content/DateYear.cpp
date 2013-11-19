
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "DateYear.h"


namespace Odt
{
	namespace Content
	{

		DateYear::DateYear()
		{
		}


		DateYear::~DateYear()
		{
		}


		DateYear::DateYear(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DateYear& DateYear::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DateYear::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode DateYear::toXML() const
		{
			return 
				XML::XElement(ns.number + "year");
		}

	} // namespace Content
} // namespace Odt