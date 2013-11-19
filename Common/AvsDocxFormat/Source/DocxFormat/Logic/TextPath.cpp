
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TextPath.h"


namespace OOX
{
	namespace Logic
	{
			
		TextPath::TextPath()
		{
		}


		TextPath::~TextPath()
		{
		}


		TextPath::TextPath(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TextPath& TextPath::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TextPath::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			On				= element.attribute("on").value();
			FitShape	= element.attribute("fitshape").value();
			Trim			= element.attribute("trim").value();
			String		= element.attribute("string").value();
			Style			= element.attribute("style").value();
			FitPath		= element.attribute("fitpath").value();
		}

		
		const XML::XNode TextPath::toXML() const
		{			
			return
				XML::XElement(ns.v + "textpath",
					XML::XAttribute("style", Style) +
					XML::XAttribute("on", On) +
					XML::XAttribute("fitshape", FitShape) +
					XML::XAttribute("trim", Trim) +
					XML::XAttribute("fitpath", FitPath) +
					XML::XAttribute("string", String)					
				);
		}

	} // namespace Logic
} // namespace OOX
