
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "RFonts.h"

namespace OOX
{
	namespace Logic
	{
		RFonts::RFonts()
		{

		}

		RFonts::~RFonts()
		{

		}	

		RFonts::RFonts(const XML::XNode& node)
		{
			fromXML(node);
		}

		const RFonts& RFonts::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void RFonts::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			AsciiTheme		=	element.attribute("asciiTheme").value();
			EastAsiaTheme	=	element.attribute("eastAsiaTheme").value();
			HAnsiTheme		=	element.attribute("hAnsiTheme").value();
			Cstheme			=	element.attribute("cstheme").value();
			Hint			=	element.attribute("hint").value();
			ascii			=	element.attribute("ascii").value();
			EastAsia		=	element.attribute("eastAsia").value();
			hAnsi			=	element.attribute("hAnsi").value();
			Cs				=	element.attribute("cs").value();
		}

		const XML::XNode RFonts::toXML() const
		{
		return XML::XElement();
		}

		void RFonts::setFontName(const nullable__<std::string>& fontName)
		{
			ascii		=	fontName;
			hAnsi		=	fontName;
			EastAsia	=	fontName;
			Cs			=	fontName;
		}

		const std::string RFonts::fontType() const
		{
			if (ascii.is_init() || hAnsi.is_init() || Cs.is_init() || EastAsia.is_init())
			{
				return "fontName";
			}
			else if (AsciiTheme.is_init())
			{
				if (AsciiTheme->find("minor") != std::string::npos)
					return "minor";
				else if (AsciiTheme->find("major") != std::string::npos)
					return "major";
				else
					return "none";
			}
			else if (EastAsiaTheme.is_init())
			{
				if (EastAsiaTheme->find("minor") != std::string::npos)
					return "minor";
				else if (EastAsiaTheme->find("major") != std::string::npos)
					return "major";
				else
					return "none";
			}
			else if (HAnsiTheme.is_init())
			{
				if (HAnsiTheme->find("minor") != std::string::npos)
					return "minor";
				else if (HAnsiTheme->find("major") != std::string::npos)
					return "major";
				else
					return "none";
			}
			else if (Cstheme.is_init())
			{
				if (Cstheme->find("minor") != std::string::npos)
					return "minor";
				else if (Cstheme->find("major") != std::string::npos)
					return "major";
				else
					return "none";
			}
			else
			{
				return "none";
			}
		}

		const std::string RFonts::fontTypeAscii() const
		{
			if (AsciiTheme.is_init())
			{
				if (AsciiTheme->find("minor") != std::string::npos)
					return "minor";
				else if (AsciiTheme->find("major") != std::string::npos)
					return "major";
			}
			else if (ascii.is_init())
				return "fontName";
			else
				return "none";
            return "";
		}

		const std::string RFonts::fontTypeHAnsi() const
		{
			if (HAnsiTheme.is_init())
			{
				if (HAnsiTheme->find("minor") != std::string::npos)
					return "minor";
				else if (HAnsiTheme->find("major") != std::string::npos)
					return "major";
			}
			else if (hAnsi.is_init())
				return "fontName";
			else
				return "none";
            return "";
		}

		const std::string RFonts::fontTypeEA() const
		{
			if (EastAsiaTheme.is_init())
			{
				if (EastAsiaTheme->find("minor") != std::string::npos)
					return "minor";
				else if (EastAsiaTheme->find("major") != std::string::npos)
					return "major";
			}
			else if (EastAsia.is_init())
				return "fontName";
			else
				return "none";
            return "";
		}

		const std::string RFonts::fontTypeCS() const
		{
			if (Cstheme.is_init())
			{
				if (Cstheme->find("minor") != std::string::npos)
					return "minor";
				else if (Cstheme->find("major") != std::string::npos)
					return "major";
			}
			else if (Cs.is_init())
				return "fontName";
			else
				return "none";
            return "";
		}

		const nullable__<std::string> RFonts::fontName() const
		{
			if (ascii.is_init())	return ascii;
			if (hAnsi.is_init())	return hAnsi;
			if (EastAsia.is_init())	return EastAsia;
			if (Cs.is_init())		return Cs;

			return nullable__<std::string>();
		}

	} // namespace Logic
} // namespace OOX