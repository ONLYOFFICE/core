#include "TextFont.h"

namespace PPTX
{
	namespace Logic
	{

		TextFont::TextFont()
		{
		}


		TextFont::~TextFont()
		{
		}


		TextFont::TextFont(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TextFont& TextFont::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TextFont::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			charset = element.attribute("charset").value();
			panose = element.attribute("panose").value();
			pitchFamily = element.attribute("pitchFamily").value();
			typeface = element.attribute("typeface").value();
		}


		const XML::XNode TextFont::toXML() const
		{
			return	XML::XElement(m_namespace + m_name.get(),
				XML::XAttribute("typeface", typeface) +
				XML::XAttribute("pitchFamily", pitchFamily) +
				XML::XAttribute("charset", charset) +
				XML::XAttribute("panose", panose)
			);
		}

		void TextFont::Merge(nullable_property<TextFont>& font)const
		{
			if(!font.is_init())
				font = TextFont();
			if(charset.is_init()) font->charset = *charset;
			if(panose.is_init()) font->panose = *panose;
			if(pitchFamily.is_init()) font->pitchFamily = *pitchFamily;
			font->typeface = *typeface;
		}
	} // namespace Logic
} // namespace PPTX