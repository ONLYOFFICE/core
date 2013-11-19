
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ThemeElements.h"


namespace OOX
{
	namespace Theme
	{

		ThemeElements::ThemeElements()
		{
		}


		ThemeElements::~ThemeElements()
		{
		}
	

		ThemeElements::ThemeElements(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ThemeElements& ThemeElements::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ThemeElements::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			clrScheme		=	element.element("clrScheme");
			fontScheme	=	element.element("fontScheme");
			fmtScheme		=	element.element("fmtScheme");
		}


		const XML::XNode ThemeElements::toXML() const
		{
			return
				XML::XElement(ns.a + "themeElements",
					XML::Write(clrScheme) +
					XML::Write(fontScheme) +
					XML::Write(fmtScheme)
				);
		}

	} // namespace Theme
} // namespace OOX