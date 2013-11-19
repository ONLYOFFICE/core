#include "ThemeElements.h"

namespace PPTX
{
	namespace nsTheme
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

			clrScheme = element.element("clrScheme");
			fontScheme = element.element("fontScheme");
			fmtScheme = element.element("fmtScheme");

			FillParentPointersForChilds();
		}

		const XML::XNode ThemeElements::toXML() const
		{
			return XML::XElement(ns.a + "themeElements",
					XML::Write(clrScheme) +
					XML::Write(fontScheme) +
					XML::Write(fmtScheme)
				);
		}

		void ThemeElements::FillParentPointersForChilds()
		{
			clrScheme->SetParentPointer(*this);
	//			if(fontScheme.is_init())
				fontScheme->SetParentPointer(*this);
	//			if(fmtScheme.is_init())
				fmtScheme->SetParentPointer(*this);
		}
	} // namespace nsTheme
} // namespace PPTX