
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "NumFormat.h"


namespace OOX
{
	namespace Logic
	{

		NumFormat::NumFormat()
		{
		}


		NumFormat::~NumFormat()
		{
		}


		NumFormat::NumFormat(const Common::NumFormat& numFormat)
		{
			fromBase(numFormat);
		}


		NumFormat::NumFormat(const XML::XNode& node)
		{
			fromXML(node);
		}


		NumFormat::NumFormat(const std::string& format)
		{
			fromString(format);
		}


		const NumFormat& NumFormat::operator = (const Common::NumFormat& numFormat)
		{
			fromBase(numFormat);
			return *this;
		}


		const NumFormat& NumFormat::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const NumFormat& NumFormat::operator = (const std::string& format)
		{
			fromString(format);	
			return *this;
		}

	
		void NumFormat::fromXML(const XML::XNode& node)
		{
			XML::XElement element(node);
			fromString(element.attribute("val").value());
		}


		const XML::XNode NumFormat::toXML() const
		{
			return 
				XML::XElement(ns.w + "numFmt",
					XML::XAttribute(ns.w + "val", ToString())
				);
		}


		const std::string NumFormat::ToString() const
		{
			switch (type())
			{
			case upperLetter:
				return "upperLetter";
			case lowerLetter:
				return "lowerLetter";
			case upperRoman:
				return "upperRoman";
			case lowerRoman:
				return "lowerRoman";
			case decimal:
				return "decimal";
			case symbol:
				return "symbol";
			case bullet:
				return "bullet";
			case chicago:
				return "chicago";
			default:
				return "decimal";
			}
		}


		void NumFormat::fromString(const std::string& value)
		{
			if (value == "upperLetter")
				setUpperLetter();
			else if (value == "lowerLetter")
				setLowerLetter();
			else if (value == "upperRoman")
				setUpperRoman();
			else if (value == "lowerRoman")
				setLowerRoman();
			else if (value == "symbol")
				setSymbol();
			else if (value == "bullet")
				setBullet();
			else if (value == "chicago")
				setChicago();
			else
				setDecimal();
		}

	} // namespace Logic
} // namespace OOX