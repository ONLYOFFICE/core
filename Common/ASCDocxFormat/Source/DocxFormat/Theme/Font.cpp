
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Font.h"


namespace OOX
{
	namespace Theme
	{

		Font::Font()
		{
		}


		Font::~Font()
		{
		}
	

		Font::Font(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Font& Font::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Font::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_script		=	element.attribute("script").value();
			m_typeface	=	element.attribute("typeface").value();
		}


		const XML::XNode Font::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX