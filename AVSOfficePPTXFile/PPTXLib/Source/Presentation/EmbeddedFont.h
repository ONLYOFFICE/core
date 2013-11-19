#pragma once
#ifndef PPTX_PRESENTATION_EMBEDDEDFONT_INCLUDE_H_
#define PPTX_PRESENTATION_EMBEDDEDFONT_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "EmbeddedFontDataId.h"
#include "./../Logic/TextFont.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class EmbeddedFont : public WrapperWritingElement
		{
		public:
			EmbeddedFont();
			virtual ~EmbeddedFont();
			explicit EmbeddedFont(const XML::XNode& node);
			const EmbeddedFont& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<EmbeddedFontDataId> Bold;
			nullable_property<EmbeddedFontDataId> BoldItalic;
			nullable_property<EmbeddedFontDataId> Italic;
			nullable_property<EmbeddedFontDataId> Regular;
			property<Logic::TextFont> Font;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_EMBEDDEDFONT_INCLUDE_H_