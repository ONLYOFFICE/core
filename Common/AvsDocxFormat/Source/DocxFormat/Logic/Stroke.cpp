
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Stroke.h"

namespace OOX
{
	namespace Logic
	{			
		Stroke::Stroke()
		{

		}

		Stroke::~Stroke()
		{

		}	

		Stroke::Stroke(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Stroke& Stroke::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Stroke::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Join		=	element.attribute("joinstyle").value();
			DashStyle	=	element.attribute("dashstyle").value();
			EndCap		=	element.attribute("endcap").value();
		}

		const XML::XNode Stroke::toXML() const
		{			
			return XML::XElement(ns.v + "stroke", XML::XAttribute("joinstyle", Join) + XML::XAttribute("dashstyle", DashStyle) + XML::XAttribute("endcap", EndCap));
		}

	} // namespace Logic
} // namespace OOX
