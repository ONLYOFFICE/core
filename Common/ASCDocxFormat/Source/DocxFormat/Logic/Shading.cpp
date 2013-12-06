
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Shading.h"

namespace OOX
{
	namespace Logic
	{
		Shading::Shading()
		{

		}

		Shading::~Shading()
		{

		}	

		Shading::Shading(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Shading& Shading::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Shading::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			value			=	element.attribute("val").value();
			color			=	element.attribute("color").value();
			fill			=	element.attribute("fill").value();
		}

		const XML::XNode Shading::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX