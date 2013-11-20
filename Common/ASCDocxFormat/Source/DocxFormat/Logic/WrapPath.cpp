
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "WrapPath.h"


namespace OOX
{
	namespace Logic
	{

		WrapPath::WrapPath()
		{
		}


		WrapPath::~WrapPath()
		{
		}


		WrapPath::WrapPath(const XML::XNode& node)
		{
			fromXML(node);
		}


		const WrapPath& WrapPath::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void WrapPath::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			start = element.element("start");
			XML::Fill(points, element, "lineTo");
			edited = element.attribute("edited").value();
		}


		const XML::XNode WrapPath::toXML() const
		{
			return XML::XElement(ns.wp + "wrapPolygon",
				XML::XAttribute("edited", edited) +
				XML::Write(start) +
				XML::Write(points)
				);
		}

	} // namespace Logic
} // namespace OOX