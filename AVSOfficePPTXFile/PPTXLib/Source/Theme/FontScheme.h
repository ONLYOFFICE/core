#pragma once
#ifndef PPTX_THEME_FONTSCHEME_INCLUDE_H_
#define PPTX_THEME_FONTSCHEME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "./../Logic/FontCollection.h"

namespace PPTX
{
	namespace nsTheme
	{
		class FontScheme : public WrapperWritingElement
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
			property<std::string> name;
			property<Logic::FontCollection> majorFont;
			property<Logic::FontCollection> minorFont;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_FONTSCHEME_INCLUDE_H