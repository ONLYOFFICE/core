#pragma once
#ifndef ODT_LOGIC_PROPERTY_DRAWINGPAGE_INCLUDE_H_
#define ODT_LOGIC_PROPERTY_DRAWINGPAGE_INCLUDE_H_

#include "./../../WritingElement.h"
#include "nullable_property.h"
#include "./../../Limit/Background.h"
#include "./../../Limit/Fill.h"
#include "./../../Unit.h"
#include "./../../Limit/Repeat.h"
#include "./../Color.h"
#include "./../../Limit/SmilType.h"

namespace Odt
{
	namespace Logic
	{
		namespace Property
		{
			class DrawingPage : public WritingElement
			{
			public:
				DrawingPage();
				virtual ~DrawingPage();
				explicit DrawingPage(const XML::XNode& node);
				const DrawingPage& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;

			public:
				static const DrawingPage merge(const DrawingPage& prev, const DrawingPage& current);

			public:
				const bool isSimple() const;

			public:
				nullable_property<bool> BackgroundVisible;
				nullable_property<bool> BackgroundObjectsVisible;
				nullable_property<bool> DisplayHeader;
				nullable_property<bool> DisplayFooter;
				nullable_property<bool> DisplayPageNumber;
				nullable_property<bool> DisplayDateTime;
				nullable_property<std::string, Odt::Limit::Background>	BackgroundSize;
				nullable_property<std::string, Odt::Limit::Fill>		Fill;
				nullable_property<Color>								FillColor;
				nullable_property<UniversalUnit>						Width;
				nullable_property<UniversalUnit>						Height;
				nullable_property<std::string>							FillImageName;
				nullable_property<std::string>							FillGradientName;
				nullable_property<std::string, Odt::Limit::Repeat>		Repeat;
				nullable_property<std::string, Odt::Limit::SmilType>	SmilType;
				nullable_property<std::string>							SmilSubType;
			};
		} // namespace Property
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTY_DRAWINGPAGE_INCLUDE_H_