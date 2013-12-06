
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Text.h"


namespace OOX
{
	namespace Logic
	{

		Text::Text()
		{
		}


		Text::~Text()
		{
		}


		Text::Text(const std::string& text)
		{
			fromTxt(text);
		}
	

		Text::Text(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Text& Text::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const Text& Text::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void Text::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_text = element.text();
		}


		void Text::fromTxt(const std::string& text)
		{
			m_text = text;
		}


		const XML::XNode Text::toXML() const
		{
		return XML::XElement();
		}


		const std::string Text::toTxt() const
		{
			return m_text;
		}

	} // namespace Logic
} // namespace OOX