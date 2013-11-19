#pragma once
#ifndef ODT_LOGIC_BACKGROUNDIMAGE_INCLUDE_H_
#define ODT_LOGIC_BACKGROUNDIMAGE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/Actuate.h"
#include "./../Limit/ImageType.h"
#include "./../Limit/Repeat.h"
#include "./../Limit/Position.h"


namespace Odt
{
	namespace Logic
	{
		class BackgroundImage : public WritingElement
		{
		public:
			BackgroundImage();
			virtual ~BackgroundImage();
			explicit BackgroundImage(const XML::XNode& node);
			const BackgroundImage& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<boost::filesystem::wpath>				Href;
			nullable_property<std::string, Limit::ImageType>	Type;
			nullable_property<std::string, Limit::Actuate>		Actuate;
			nullable_property<std::string, Limit::Repeat> Repeat;
			nullable_property<std::string, Limit::Position> Position;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_BACKGROUNDIMAGE_INCLUDE_H_
