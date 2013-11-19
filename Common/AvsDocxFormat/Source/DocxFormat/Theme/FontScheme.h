#pragma once
#ifndef OOX_THEME_FONT_SCHEME_INCLUDE_H_
#define OOX_THEME_FONT_SCHEME_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "MajorFont.h"
#include "MinorFont.h"


namespace OOX
{
	namespace Theme
	{
		class FontScheme : public WritingElement
		{
		public:
			FontScheme();
			virtual ~FontScheme();
			explicit FontScheme(const XML::XNode& node);
			const FontScheme& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string> name;
			nullable_property<MajorFont>   majorFont;
			nullable_property<MinorFont>   minorFont;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_FONT_SCHEME_INCLUDE_H_