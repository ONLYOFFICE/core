
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Frame.h"


namespace Odt
{
	namespace Content
	{

		Frame::Frame()
		{
		}


		Frame::~Frame()
		{
		}


		Frame::Frame(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Frame& Frame::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Frame::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Style		= element.attribute("style-name").value();
			Name		= element.attribute("name").value();
			Anchortype	= element.attribute("anchor-type").value();
			Size		= XML::XElement2NullableSize<UnitCm>(element, "height", "width");
			ZIndex		= element.attribute("z-index").value();
			if(element.attribute("x").exist())
				X = element.attribute("x").value().ToString();
			if(element.attribute("y").exist())
				Y = element.attribute("y").value().ToString();
			Image		= element.element("image");
			ObjectOle	= element.element("object-ole");
		}


		const XML::XNode Frame::toXML() const
		{
			return
				XML::XElement(ns.draw + "frame",
					XML::XAttribute(ns.draw + "style-name", Style) + 
					XML::XAttribute(ns.draw + "name", Name) +
					XML::XAttribute(ns.text + "anchor-type", Anchortype) +
					XML::XAttribute(ns.svg + "x", X) +
					XML::XAttribute(ns.svg + "y", Y) +
					XML::XAttribute(ns.svg + "width", Size->Width) +
					XML::XAttribute(ns.svg + "height", Size->Height) +
					XML::XAttribute(ns.draw + "z-index", ZIndex) +
					XML::Write(ObjectOle) +
					XML::Write(Image)
				);
		}


		const std::string Frame::toTxt() const
		{
			return std::string("[") + Name.ToString() + std::string("]");
		}


		const bool Frame::isOle() const
		{
			return ObjectOle.is_init();
		}

	} // namespace Content
} // namespace Odt