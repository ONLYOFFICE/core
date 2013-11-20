
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "EffectExtent.h"


namespace OOX
{
	namespace Logic
	{

		EffectExtent::EffectExtent()
		{
		}


		EffectExtent::~EffectExtent()
		{
		}
	

		EffectExtent::EffectExtent(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EffectExtent& EffectExtent::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void EffectExtent::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Left	 = element.attribute("l").value().ToString();
			Top		 = element.attribute("t").value().ToString();
			Right	 =	element.attribute("r").value().ToString();
			Bottom =	element.attribute("b").value().ToString();	
		}


		const XML::XNode EffectExtent::toXML() const
		{
			return
				XML::XElement(ns.wp + "effectExtent", 
					XML::XAttribute("l", Left) +
					XML::XAttribute("t", Top) +
					XML::XAttribute("r", Right) +
					XML::XAttribute("b", Bottom)
				);
		}

	} // namespace Logic
} // namespace OOX