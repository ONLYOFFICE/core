#pragma once
#ifndef OOX_LOGIC_RFONTS_INCLUDE_H_
#define OOX_LOGIC_RFONTS_INCLUDE_H_

#include <string>

#include "./../WritingElement.h"
#include "nullable_property.h"

#include "../.././../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../.././../../Common/DocxFormat/Source/Xml/XmlUtils.h"

namespace OOX
{
	namespace Logic
	{
		class RFonts : public WritingElement
		{
		public:
			RFonts();
			virtual ~RFonts();
			explicit RFonts(const XML::XNode& node);
			const RFonts& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			void setFontName(const nullable__<std::string>& fontName);
			const std::string fontType() const;
			const std::string fontTypeAscii() const;
			const std::string fontTypeHAnsi() const;
			const std::string fontTypeEA() const;
			const std::string fontTypeCS() const;
			const nullable__<std::string> fontName() const;

		public:
			nullable_property<std::string>		AsciiTheme;
			nullable_property<std::string>		EastAsiaTheme;
			nullable_property<std::string>		HAnsiTheme;
			nullable_property<std::string>		Cstheme;

			nullable_property<std::string>		Hint;

			nullable_property<std::string>		ascii;
			nullable_property<std::string>		hAnsi;

			nullable_property<std::string>		EastAsia;
			nullable_property<std::string>		Cs;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_RFONTS_INCLUDE_H_