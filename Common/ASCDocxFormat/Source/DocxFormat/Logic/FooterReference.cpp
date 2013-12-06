
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FooterReference.h"


namespace OOX
{
	namespace Logic
	{

		FooterReference::FooterReference()
		{
		}


		FooterReference::~FooterReference()
		{
		}


		FooterReference::FooterReference(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FooterReference& FooterReference::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void FooterReference::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			rId		  = element.attribute("id").value().ToWString();
			Type  = element.attribute("type").value();
		}


		const XML::XNode FooterReference::toXML() const
		{
			return XML::XElement();

			//return
			//	XML::XElement(ns.w + "footerReference", 
			//		XML::XAttribute(ns.r + "id", rId) +
			//		XML::XAttribute(ns.w + "type", Type)
			//	);
		}


		const std::string FooterReference::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX