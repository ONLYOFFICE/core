
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FootnoteRef.h"


namespace OOX
{
	namespace Logic
	{

		FootnoteRef::FootnoteRef()
		{
		}


		FootnoteRef::~FootnoteRef()
		{
		}
	

		FootnoteRef::FootnoteRef(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FootnoteRef& FootnoteRef::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FootnoteRef::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode FootnoteRef::toXML() const
		{
			return XML::XElement(ns.w + "footnoteRef");
		}


		const std::string FootnoteRef::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX