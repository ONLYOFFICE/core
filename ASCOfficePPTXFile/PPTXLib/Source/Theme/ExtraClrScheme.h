#pragma once
#ifndef PPTX_THEME_EXTRACLRSCHEME_INCLUDE_H_
#define PPTX_THEME_EXTRACLRSCHEME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "ClrScheme.h"
#include "./../Logic/ClrMap.h"

namespace PPTX
{
	namespace nsTheme
	{
		class ExtraClrScheme : public WrapperWritingElement
		{
		public:
			ExtraClrScheme();
			virtual ~ExtraClrScheme();			
			explicit ExtraClrScheme(const XML::XNode& node);
			const ExtraClrScheme& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<ClrScheme> clrScheme;
			nullable_property<Logic::ClrMap> clrMap;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_EXTRACLRSCHEME_INCLUDE_H