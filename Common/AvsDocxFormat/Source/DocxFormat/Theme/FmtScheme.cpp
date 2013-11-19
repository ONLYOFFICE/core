
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FmtScheme.h"


namespace OOX
{
	namespace Theme
	{

		FmtScheme::FmtScheme()
		{
		}


		FmtScheme::~FmtScheme()
		{
		}
	

		FmtScheme::FmtScheme(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FmtScheme& FmtScheme::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FmtScheme::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			name					=	element.attribute("name").value();
			fillStyleLst	=	element.element("fillStyleLst");
			lnStyleLst		=	element.element("lnStyleLst");
			effectStyleLst	=	element.element("effectStyleLst");
			bgFillStyleLst	=	element.element("bgFillStyleLst");
		}


		const XML::XNode FmtScheme::toXML() const
		{
			return 
				XML::XElement(ns.a + "fmtScheme",
					XML::XAttribute("name", name) +
					XML::Write(fillStyleLst) +
					XML::Write(lnStyleLst) +
					XML::Write(effectStyleLst) +
					XML::Write(bgFillStyleLst)
				);
		}

	} // namespace Theme
} // namespace OOX