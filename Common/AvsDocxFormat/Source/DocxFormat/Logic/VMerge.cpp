
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "VMerge.h" 


namespace OOX
{
	namespace Logic
	{

		VMerge::VMerge()
		{
		}


		VMerge::~VMerge()
		{
		}
	

		VMerge::VMerge(const XML::XNode& node)
		{
			fromXML(node);
		}


		const VMerge& VMerge::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void VMerge::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Value = element.attribute("val").value();
		}


		const XML::XNode VMerge::toXML() const
		{
			return XML::XElement(ns.w + "vMerge", XML::XAttribute(ns.w + "val", Value));
		}

	} // namespace Logic
} // namespace OOX