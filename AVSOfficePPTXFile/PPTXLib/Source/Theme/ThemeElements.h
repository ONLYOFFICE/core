#pragma once
#ifndef PPTX_THEME_THEMEELEMENTS_INCLUDE_H_
#define PPTX_THEME_THEMEELEMENTS_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "ClrScheme.h"
#include "FontScheme.h"
#include "FmtScheme.h"

namespace PPTX
{
	namespace nsTheme
	{
		class ThemeElements : public WrapperWritingElement
		{
		public:
			ThemeElements();
			virtual ~ThemeElements();			
			explicit ThemeElements(const XML::XNode& node);
			const ThemeElements& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<ClrScheme> clrScheme;
			property<FontScheme> fontScheme;
			property<FmtScheme> fmtScheme;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_THEMEELEMENTS_INCLUDE_H