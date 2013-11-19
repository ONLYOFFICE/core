
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "OleObject.h"


namespace Odt
{
	namespace Content
	{

		OleObject::OleObject()
		{
		}


		OleObject::~OleObject()
		{
		}


		OleObject::OleObject(const XML::XNode& node)
		{
			fromXML(node);
		}


		const OleObject& OleObject::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void OleObject::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			StyleName	= element.attribute("style-name").value();
			Name		= element.attribute("name").value();
			Anchortype	= element.attribute("anchor-type").value();
			X			= element.attribute("x").value();
			Y			= element.attribute("y").value();
			Width		= element.attribute("width").value().ToString();
			Height		= element.attribute("height").value().ToString();
			ZIndex		= element.attribute("z-index").value();	
			Image		= element.element("image");
			ObjectOle	= element.element("object-ole");
			ContourPolygon = element.element("contour-polygon");
		}


		const XML::XNode OleObject::toXML() const
		{
			return
				XML::XElement(ns.draw + "frame",
					XML::XAttribute(ns.draw + "style-name", StyleName) + 
					XML::XAttribute(ns.draw + "name", Name) +
					XML::XAttribute(ns.text + "anchor-type", Anchortype) +
					XML::XAttribute(ns.svg + "x", X) +
					XML::XAttribute(ns.svg + "y", Y) +
					XML::XAttribute(ns.svg + "width", Width) +
					XML::XAttribute(ns.svg + "height", Height) +
					XML::XAttribute(ns.draw + "z-index", ZIndex) +
					XML::Write(ObjectOle) +
					XML::Write(Image) +
					XML::Write(ContourPolygon)
				);
		}


		const std::string OleObject::toTxt() const
		{
			return std::string("[") + Name.ToString() + std::string("]");
		}

	} // namespace Content
} // namespace Odt