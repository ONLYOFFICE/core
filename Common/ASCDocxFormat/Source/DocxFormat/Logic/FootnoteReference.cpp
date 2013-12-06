
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FootnoteReference.h"


namespace OOX
{
	namespace Logic
	{

		FootnoteReference::FootnoteReference()
		{
		}


		FootnoteReference::~FootnoteReference()
		{
		}


		FootnoteReference::FootnoteReference(const size_t index)
		{
			Id = index;
		}
	

		FootnoteReference::FootnoteReference(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FootnoteReference& FootnoteReference::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FootnoteReference::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Id				  = element.attribute("id").value();
			CustomMarkFollows = element.attribute("customMarkFollows").value();
		}


		const XML::XNode FootnoteReference::toXML() const
		{
		return XML::XElement();
		}


		const std::string FootnoteReference::toTxt() const
		{
			//return "[footnote]";
			return "[_]";
		}

	} // namespace Logic
} // namespace OOX