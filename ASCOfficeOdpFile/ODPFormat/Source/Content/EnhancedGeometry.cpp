#include "EnhancedGeometry.h"

namespace Odp
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
			ViewBox = element.attribute("viewBox").value();
			Type = element.attribute("type").value();
			if((Type.get() == "ellipse") && (!element.attribute("enhanced-path").exist()))
				EnhancedPath = "M 0 10800 Y 10800 0 21600 10800 10800 21600 0 10800 Z N";
			else
				EnhancedPath = element.attribute("enhanced-path").value();
			TextAreas = element.attribute("text-areas").value();
			Modifiers = element.attribute("modifiers").value();
			MirrorHorizontal = element.attribute("mirror-horizontal").value();
			MirrorVertical = element.attribute("mirror-vertical").value();
			GluePoints = element.attribute("glue-points").value();
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
					XML::XAttribute(ns.draw + "type", Type) +
					XML::XAttribute(ns.draw + "modifiers", Modifiers) +
					XML::XAttribute(ns.draw + "glue-points", GluePoints) +
					XML::XAttribute(ns.draw + "enhanced-path", EnhancedPath) +
					XML::Write(Equations) +
					XML::Write(Handles)
				);
		}

		const std::string EnhancedGeometry::GetODString()const
		{
			XML::XElement element(toXML());
			element.Add(ns.svg);
			element.Add(ns.draw);
			return element.ToString();
		}

	} // namespace Content
} // namespace Odp