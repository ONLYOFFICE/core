
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Path.h"


namespace OOX
{
	namespace Logic
	{
			
		Path::Path()
		{
		}


		Path::~Path()
		{
		}


		Path::Path(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Path& Path::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Path::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);			
			
			GradientShapeOk	= element.attribute("gradientshapeok").value();
			ConnectType		= element.attribute("connecttype").value();
			ConnectLocs		= element.attribute("connectlocs").value();
			TextBoxRect		= element.attribute("textboxrect").value();
			ConnectAngles	= element.attribute("connectangles").value();
			TextPathOk		= element.attribute("textpathok").value();
			Limo			= element.attribute("limo").value();
			ArrowOk			= element.attribute("arrowok").value();
			ExtrusionOk		= element.attribute("extrusionok").value();
		}

		
		const XML::XNode Path::toXML() const
		{			
			return
				XML::XElement(ns.v + "shape",
					XML::XAttribute("arrowok", ArrowOk) +
					XML::XAttribute(ns.o + "extrusionok", ExtrusionOk) +
					XML::XAttribute("gradientshapeok", GradientShapeOk) +
					XML::XAttribute("textpathok", TextPathOk) +
					XML::XAttribute(ns.o + "connecttype", ConnectType) +
					XML::XAttribute(ns.o + "connectlocs", ConnectLocs) +
					XML::XAttribute("textboxrect", TextBoxRect) +
					XML::XAttribute("limo", Limo) +
					XML::XAttribute(ns.o + "connectangles", ConnectAngles)
				);
		}

	} // namespace Logic
} // namespace OOX
