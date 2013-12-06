
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FontScheme.h"


namespace OOX
{
	namespace Theme
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
			name			=	element.attribute("name").value();
			majorFont	=	element.element("majorFont");
			minorFont	=	element.element("minorFont");
		}


		const XML::XNode FontScheme::toXML() const
		{	
		return XML::XElement();
		}

	} // namespace Theme
} // namespace OOX