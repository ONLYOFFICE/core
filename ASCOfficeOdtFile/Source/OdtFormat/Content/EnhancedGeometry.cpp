
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "EnhancedGeometry.h"


namespace Odt
{
	namespace Content
	{

		EnhancedGeometry::EnhancedGeometry()
		{
		}


		EnhancedGeometry::~EnhancedGeometry()
		{
		}


		EnhancedGeometry::EnhancedGeometry(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EnhancedGeometry& EnhancedGeometry::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void EnhancedGeometry::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			ViewBox				= element.attribute("viewBox").value();
			GluePoints		= element.attribute("glue-points").value();
			Type					= element.attribute("type").value();
			EnhancedPath	= element.attribute("enhanced-path").value();
			TextAreas			= element.attribute("text-areas").value();
			Modifiers			= element.attribute("modifiers").value();
			MirrorHorizontal	= element.attribute("mirror-horizontal").value();
			MirrorVertical		= element.attribute("mirror-vertical").value();
			TextPath					= element.attribute("text-path").value();
			TextPathMode			= element.attribute("text-path-mode").value();
			TextPathScale			= element.attribute("text-path-scale").value();
			TextPathSameLetterHeights	= element.attribute("text-path-same-letter-heights").value();
			XML::Fill(Equations, element, "equation");
			XML::Fill(Handles, element, "handle");
		}


		const XML::XNode EnhancedGeometry::toXML() const
		{
			return 
				XML::XElement(ns.draw + "enhanced-geometry",
					XML::XAttribute(ns.svg + "viewBox", ViewBox) +
					XML::XAttribute(ns.draw + "mirror-horizontal", MirrorHorizontal) +
					XML::XAttribute(ns.draw + "mirror-vertical", MirrorVertical) +
					XML::XAttribute(ns.draw + "text-areas", TextAreas) + 
					XML::XAttribute(ns.draw + "text-path", TextPath) +
					XML::XAttribute(ns.draw + "text-path-mode", TextPathMode) +
					XML::XAttribute(ns.draw + "text-path-scale", TextPathScale) +
					XML::XAttribute(ns.draw + "text-path-same-letter-heights", TextPathSameLetterHeights) +
					XML::XAttribute(ns.draw + "type", Type) +
					XML::XAttribute(ns.draw + "modifiers", Modifiers) +
					XML::XAttribute(ns.draw + "glue-points", GluePoints) +					
					XML::XAttribute(ns.draw + "enhanced-path", EnhancedPath) +					
					XML::Write(Equations) +
					XML::Write(Handles)
				);
		}

	} // namespace Content
} // namespace Odt