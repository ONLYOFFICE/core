
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "HeaderReference.h"


namespace OOX
{
	namespace Logic
	{

		HeaderReference::HeaderReference()
		{
		}


		HeaderReference::~HeaderReference()
		{
		}


		HeaderReference::HeaderReference(const XML::XNode& node)
		{
			fromXML(node);
		}


		const HeaderReference& HeaderReference::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void HeaderReference::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			rId		= element.attribute("id").value().ToWString();
			Type  = element.attribute("type").value();
		}


		const XML::XNode HeaderReference::toXML() const
		{
			return XML::XElement();
			//	XML::XElement(ns.w + "headerReference", 
			//		XML::XAttribute(ns.r + "id", rId) +
			//		XML::XAttribute(ns.w + "type", Type)
			//	);
		}


		const std::string HeaderReference::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX