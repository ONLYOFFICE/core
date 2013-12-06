
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Background.h"


namespace OOX
{
	namespace Logic
	{

		Background::Background()
		{
		}


		Background::~Background()
		{
		}
	

		Background::Background(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Background& Background::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Background::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Color	=	element.attribute("color").value();
			ThemeColor	=	element.attribute("themeColor").value();
			ThemeShade	=	element.attribute("themeShade").value();
		}


		const XML::XNode Background::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX