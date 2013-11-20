
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "F.h"


namespace OOX
{
	namespace Logic
	{
			
		F::F()
		{
		}


		F::~F()
		{
		}


		F::F(const XML::XNode& node)
		{
			fromXML(node);
		}


		const F& F::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void F::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Eqn = element.attribute("eqn").value();
		}

		
		const XML::XNode F::toXML() const
		{			
			return XML::XElement("f",XML::XAttribute("eqn", Eqn));
			//return XML::XElement(ns.v + "f",XML::XAttribute("eqn", Eqn));
		}

	} // namespace Logic
} // namespace OOX