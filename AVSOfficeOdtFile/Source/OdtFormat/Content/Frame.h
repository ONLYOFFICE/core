#pragma once
#ifndef ODT_CONTENT_FRAME_INCLUDE_H_
#define ODT_CONTENT_FRAME_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include "./../Limit/Anchortype.h"
#include "./../Unit.h"
#include "Image.h"
#include "Common.h"
#include "ObjectOle.h"


namespace Odt
{
	namespace Content
	{
		class Frame : public ParagraphItemBase
		{
		public:
			Frame();
			virtual ~Frame();
			explicit Frame(const XML::XNode& node);
			const Frame& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			const bool isOle() const;

		public:
			property<std::string>					Style;
			property<std::string>					Name;
			property<std::string, Limit::Anchortype>	Anchortype;
			nullable_property<Common::Size<UnitCm> >	Size;
			nullable_property<UniversalUnit>		X;
			nullable_property<UniversalUnit>		Y;
			property<int>							ZIndex;
			property<Image>							Image;
			nullable_property<ObjectOle>			ObjectOle;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_FRAME_INCLUDE_H_
