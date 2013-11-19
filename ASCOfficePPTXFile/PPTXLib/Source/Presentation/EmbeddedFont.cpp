#include "EmbeddedFont.h"

namespace PPTX
{
	namespace nsPresentation
	{
		EmbeddedFont::EmbeddedFont()
		{
		}

		EmbeddedFont::~EmbeddedFont()
		{
		}

		EmbeddedFont::EmbeddedFont(const XML::XNode& node)
		{
			fromXML(node);
		}

		const EmbeddedFont& EmbeddedFont::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void EmbeddedFont::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Bold = element.element("bold");
			BoldItalic = element.element("boldItalic");
			Italic = element.element("italic");
			Regular = element.element("regular");
			Font = element.element("font");

			FillParentPointersForChilds();
		}

		const XML::XNode EmbeddedFont::toXML() const
		{
			return	XML::XElement(ns.p + "embeddedFont",
				XML::Write(Bold) +
				XML::Write(BoldItalic) +
				XML::Write(Italic) +
				XML::Write(Regular) +
				XML::Write(Font)
			);
		}

		void EmbeddedFont::FillParentPointersForChilds()
		{
			if(Bold.is_init())
				Bold->SetParentPointer(*this);
			if(BoldItalic.is_init())
				BoldItalic->SetParentPointer(*this);
			if(Italic.is_init())
				Italic->SetParentPointer(*this);
			if(Regular.is_init())
				Regular->SetParentPointer(*this);
			Font->SetParentPointer(*this);
		}
	} // namespace nsPresentation
} // namespace PPTX