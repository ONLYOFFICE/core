
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Styles.h"


namespace OOX
{

	Styles::LsdException::LsdException()
	{
	}


	Styles::LsdException::~LsdException()
	{
	}
	

	Styles::LsdException::LsdException(const XML::XNode& node)
	{
		fromXML(node);
	}


	const Styles::LsdException& Styles::LsdException::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}


	void Styles::LsdException::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		Name				=	element.attribute("name").value();
		SemiHidden	=	element.attribute("semiHidden").value();
		UiPriority	=	element.attribute("uiPriority").value();
		UnhideWhenUsed	=	element.attribute("unhideWhenUsed").value();
		QFormat					=	element.attribute("qFormat").value();
	}


	const XML::XNode Styles::LsdException::toXML() const
	{
		return 
			XML::XElement(ns.w + "lsdException",
				XML::XAttribute(ns.w + "name", Name) + 
				XML::XAttribute(ns.w + "semiHidden", SemiHidden) +
				XML::XAttribute(ns.w + "uiPriority", UiPriority) +
				XML::XAttribute(ns.w + "unhideWhenUsed", UnhideWhenUsed) + 
				XML::XAttribute(ns.w + "qFormat", QFormat)
			);
	}

} // namespace OOX