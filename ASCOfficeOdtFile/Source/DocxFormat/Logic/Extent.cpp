
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Extent.h"


namespace OOX
{
	namespace Logic
	{

		Extent::Extent()
		{
		}


		Extent::~Extent()
		{
		}
	

		Extent::Extent(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Extent& Extent::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Extent::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Size = XML::XElement2Size<UnitSx>(element, "cy", "cx");
		}


		const XML::XNode Extent::toXML() const
		{
			return
				XML::XElement(ns.wp + "extent", 
					XML::XAttribute("cx", Size->Width) +
					XML::XAttribute("cy", Size->Height)
				);
		}

	} // namespace Logic
} // namespace OOX