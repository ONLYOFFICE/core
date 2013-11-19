
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "DocPr.h"


namespace OOX
{
	namespace Logic
	{

		DocPr::DocPr()
		{
		}


		DocPr::~DocPr()
		{
		}
	

		DocPr::DocPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DocPr& DocPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DocPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Id = element.attribute("id").value();
			Name = element.attribute("name").value();
			Descr =	element.attribute("descr").value();			
		}


		const XML::XNode DocPr::toXML() const
		{
			return
				XML::XElement(ns.wp + "docPr", 
					XML::XAttribute("id", Id) +
					XML::XAttribute("name", Name) +
					XML::XAttribute("descr", Descr)
				);
		}

	} // namespace Logic
} // namespace OOX