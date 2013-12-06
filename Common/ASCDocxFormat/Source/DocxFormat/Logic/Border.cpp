
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Border.h"


namespace OOX
{
	namespace Logic
	{

		Border::Border()
		{
		}


		Border::~Border()
		{
		}
	

		Border::Border(const XML::XNode& node)
		{
			fromXML(node);
		}


		Border::Border(const std::string& name, const BorderItem& border)
			: Name(name),
				Bdr(border)
		{
		}


		const Border& Border::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Border::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name			= element.XName->Name; 
			Bdr->Value		= element.attribute("val").value();
			if (element.attribute("sz").value().ToString() == "auto")
				Bdr->Sz		= 0;
			else
				Bdr->Sz		= element.attribute("sz").value();
			Bdr->Space		= element.attribute("space").value();
			Bdr->Color		= element.attribute("color").value();
			Bdr->ThemeColor = element.attribute("themeColor").value();
		}


		const XML::XNode Border::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX