
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FldChar.h"


namespace OOX
{
	namespace Logic
	{

		FldChar::FldChar()
		{
		}


		FldChar::~FldChar()
		{
		}
	

		FldChar::FldChar(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FldChar& FldChar::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FldChar::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			FldCharType = element.attribute("fldCharType").value();
		}


		const XML::XNode FldChar::toXML() const
		{
			return 
				XML::XElement(ns.w + "fldChar",
					XML::XAttribute(ns.w + "fldCharType", FldCharType)
				);
		}


		const std::string FldChar::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX