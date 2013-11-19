
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Equation.h"


namespace Odt
{
	namespace Content
	{

		Equation::Equation()
		{
		}


		Equation::~Equation()
		{
		}


		Equation::Equation(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Equation& Equation::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Equation::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name		= element.attribute("name").value();
			Formula = element.attribute("formula").value();
		}


		const XML::XNode Equation::toXML() const
		{
			return 
				XML::XElement(ns.draw + "equation",
					XML::XAttribute(ns.draw + "name", Name) +
					XML::XAttribute(ns.draw + "formula", Formula) 
				);
		}

	} // namespace Content
} // namespace Odt