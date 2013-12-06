
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "EndnoteReference.h"


namespace OOX
{
	namespace Logic
	{

		EndnoteReference::EndnoteReference()
		{
		}


		EndnoteReference::~EndnoteReference()
		{
		}


		EndnoteReference::EndnoteReference(const size_t index)
		{
			Id = index;
		}
	

		EndnoteReference::EndnoteReference(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EndnoteReference& EndnoteReference::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void EndnoteReference::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Id				  = element.attribute("id").value();
			CustomMarkFollows = element.attribute("customMarkFollows").value();
		}


		const XML::XNode EndnoteReference::toXML() const
		{
		return XML::XElement();
		}

		const std::string EndnoteReference::toTxt() const
		{
			//return "[endnote]";
			return "[_]";
		}

	} // namespace Logic
} // namespace OOX