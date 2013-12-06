
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Pic.h"

namespace OOX
{
	namespace Logic
	{
		Pic::Pic()
		{

		}

		Pic::~Pic()
		{

		}	

		Pic::Pic(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Pic& Pic::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Pic::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Id		=	element.element("nvPicPr").element("cNvPr").attribute("id").value();
			Name	=	element.element("nvPicPr").element("cNvPr").attribute("name").value();

			if (element.element("blipFill").element("blip").attribute("embed").exist())
				rId	=	element.element("blipFill").element("blip").attribute("embed").value().ToWString();

			Off		=	XML::XElement2NullablePoint<UnitSx>(element.element("spPr").element("xfrm").element("off"), "x", "y");
			Ext		=	XML::XElement2NullableSize<UnitSx>(element.element("spPr").element("xfrm").element("ext"), "cy", "cx");
			Prst	=	element.element("spPr").element("prstGeom").attribute("prst").value();

			if (element.element("spPr").element("xfrm").attribute("rot").exist())
				rot	=	element.element("spPr").element("xfrm").attribute("rot").value().ToString();

			if (element.element("spPr").element("xfrm").attribute("flipH").exist())
				flipH	=	element.element("spPr").element("xfrm").attribute("flipH").value().ToString();

			if (element.element("spPr").element("xfrm").attribute("flipV").exist())
				flipV	=	element.element("spPr").element("xfrm").attribute("flipV").value().ToString();
		}

		const XML::XNode Pic::toXML() const
		{
			return XML::XElement();
				//XML::XElement(ns.pic + "pic",
				//	XML::XElement(ns.pic + "nvPicPr",
				//		XML::XElement(ns.pic + "cNvPr",
				//			XML::XAttribute("id", Id) +
				//			XML::XAttribute("name", Name)
				//		) +
				//		XML::XElement(ns.pic + "cNvPicPr")
				//	) +
				//	XML::XElement(ns.pic + "blipFill",
				//		XML::XElement(ns.a + "blip", XML::XAttribute(ns.r + "embed", rId)) +
				//		XML::XElement(ns.a + "stretch",
				//			XML::XElement(ns.a + "fillRect")
				//		)
				//	) +
				//	XML::XElement(ns.pic + "spPr",
				//		XML::XElement(ns.a + "xfrm",
				//			XML::WriteIf(	// TODO исправить, после создания нормальных Size и Point
				//				XML::XElement(ns.a + "off",
				//					XML::XAttribute("x", Off.get_value_or_default().X) +
				//					XML::XAttribute("y", Off.get_value_or_default().Y)
				//				), Off.is_init()
				//			) +
				//			XML::WriteIf( // TODO исправить, после создания нормальных Size и Point
				//				XML::XElement(ns.a + "ext",
				//					XML::XAttribute("cx", Ext.get_value_or_default().Width) +
				//					XML::XAttribute("cy", Ext.get_value_or_default().Height)
				//				), Ext.is_init()
				//			)
				//		) +
				//		XML::XElement(ns.a + "prstGeom",
				//			XML::XAttribute("prst", Prst) +
				//			XML::XElement(ns.a + "avLst")
				//		)
				//	)
				//);
		}

	} // namespace Logic
} // namespace OOX