#include "FontScheme.h"

namespace PPTX
{
	namespace nsTheme
	{
		FontScheme::FontScheme()
		{
		}

		FontScheme::~FontScheme()
		{
		}

		FontScheme::FontScheme(const XML::XNode& node)
		{
			fromXML(node);
		}

		const FontScheme& FontScheme::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void FontScheme::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			name = element.attribute("name").value();
			majorFont = element.element("majorFont");
			minorFont = element.element("minorFont");

			FillParentPointersForChilds();
		}

		const XML::XNode FontScheme::toXML() const
		{
			return XML::XElement(ns.a + "fontScheme",
					XML::XAttribute("name", name) +
					XML::Write(majorFont) +
					XML::Write(minorFont)
				);
		}

		void FontScheme::FillParentPointersForChilds()
		{
			majorFont->SetParentPointer(*this);
			minorFont->SetParentPointer(*this);
		}
	} // namespace nsTheme
} // namespace PPTX