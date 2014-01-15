
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "EndnoteRef.h"


namespace OOX
{
	namespace Logic
	{

		EndnoteRef::EndnoteRef()
		{
		}


		EndnoteRef::~EndnoteRef()
		{
		}
	

		EndnoteRef::EndnoteRef(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EndnoteRef& EndnoteRef::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void EndnoteRef::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode EndnoteRef::toXML() const
		{
			return XML::XElement(ns.w + "endnoteRef");
		}


		const std::string EndnoteRef::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX