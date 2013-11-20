
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Wrap.h"

namespace OOX
{
	namespace Logic
	{			
		Wrap::Wrap()
		{

		}

		Wrap::~Wrap()
		{

		}	

		Wrap::Wrap(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Wrap& Wrap::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Wrap::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Type	=	element.attribute("type").value();
		    AnchorX	=	element.attribute("anchorx").value();
			AnchorY	=	element.attribute("anchory").value();
		}	

		const XML::XNode Wrap::toXML() const
		{
		    return XML::XElement(ns.w10 + "wrap", 
			            XML::XAttribute("type", Type) +
						XML::XAttribute("anchorx", AnchorX) +
						XML::XAttribute("anchory", AnchorY));
		}

	} // namespace Logic
} // namespace OOX
