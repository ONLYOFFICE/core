
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FillStyle.h"

namespace OOX
{
	namespace Logic
	{			
		FillStyle::FillStyle()
		{

		}

		FillStyle::~FillStyle()
		{

		}

		FillStyle::FillStyle(const XML::XNode& node)
		{
			fromXML(node);
		}

		const FillStyle& FillStyle::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void FillStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element.attribute("filled").exist())
				filled		=	element.attribute("filled").value();
			if (element.attribute("fillcolor").exist())
				fillcolor	=	element.attribute("fillcolor").value();			

			const XML::XElement fillXML	= element.element("fill");
			if (fillXML.is_init())
			{
				if (fillXML.attribute("opacity").exist())
					opacity	=	fillXML.attribute ("opacity").value();
				if (fillXML.attribute("id").exist())
					Id		=	fillXML.attribute("id").value();
				if (fillXML.attribute("type").exist())
					type	=	fillXML.attribute("type").value();
				if (fillXML.attribute("recolor").exist())
					recolor	=	fillXML.attribute("recolor").value();
				if (fillXML.attribute("rotate").exist())
					rotate	=	fillXML.attribute("rotate").value();
			}	

			if (element.attribute("color").exist())
				fillcolor	=	element.attribute("color").value();			
			if (element.attribute("opacity").exist())
				opacity		=	element.attribute("opacity").value();
		}

		const XML::XNode FillStyle::toXML() const
		{			
			return XML::XElement();
		}
	} 
}
