
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "NumPr.h"


namespace OOX
{
	namespace Logic
	{

		NumPr::NumPr()
		{
		}


		NumPr::~NumPr()
		{
		}


		NumPr::NumPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NumPr& NumPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void NumPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Ilvl	= element.element("ilvl").attribute("val").value();
			NumId	=	element.element("numId").attribute("val").value();
		}


		const XML::XNode NumPr::toXML() const
		{
			return
				XML::XElement(ns.w + "numPr",
					XML::Write(ns.w + "ilvl", ns.w + "val", Ilvl) +
					XML::XElement(ns.w + "numId", XML::XAttribute(ns.w + "val", NumId))
				);
		}

	} // namespace Logic
} // namespace OOX