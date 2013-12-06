
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
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX
