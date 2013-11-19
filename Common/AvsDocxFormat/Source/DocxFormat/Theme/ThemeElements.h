#pragma once
#ifndef OOX_THEME_THEME_ELEMENTS_INCLUDE_H_
#define OOX_THEME_THEME_ELEMENTS_INCLUDE_H_

#include "./../WritingElement.h"
#include "ClrScheme.h"
#include "FontScheme.h"
#include "FmtScheme.h"


namespace OOX
{
	namespace Theme
	{
		class ThemeElements : public WritingElement
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
			nullable_property<ClrScheme>  clrScheme;
			nullable_property<FontScheme> fontScheme;
			nullable_property<FmtScheme>  fmtScheme;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_THEME_ELEMENTS_INCLUDE_H_