
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
		NumFormat::NumFormat(const std::wstring& format)
		{
			fromStringW(format);
		}

		NumFormat::NumFormat(XmlUtils::CXmlNode& oNode)
		{
			fromXML(oNode);
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

		void NumFormat::fromXML(XmlUtils::CXmlNode& oNode)
		{
			fromStringW(std::wstring(static_cast<const wchar_t*>(oNode.GetAttributeBase( _T("w:val")))));
		}

		const XML::XNode NumFormat::toXML() const
		{
		return XML::XElement();
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

		const std::wstring NumFormat::ToStringW() const
		{
			switch (type())
			{
			case upperLetter:
				return L"upperLetter";
			case lowerLetter:
				return L"lowerLetter";
			case upperRoman:
				return L"upperRoman";
			case lowerRoman:
				return L"lowerRoman";
			case decimal:
				return L"decimal";
			case symbol:
				return L"symbol";
			case bullet:
				return L"bullet";
			case chicago:
				return L"chicago";
			default:
				return L"decimal";
			}

			return L"decimal";
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
		void NumFormat::fromStringW(const std::wstring& value)
		{
			if (value == L"upperLetter")
				setUpperLetter();
			else if (value == L"lowerLetter")
				setLowerLetter();
			else if (value == L"upperRoman")
				setUpperRoman();
			else if (value == L"lowerRoman")
				setLowerRoman();
			else if (value == L"symbol")
				setSymbol();
			else if (value == L"bullet")
				setBullet();
			else if (value == L"chicago")
				setChicago();

			setDecimal();
		}

	} // namespace Logic
} // namespace OOX