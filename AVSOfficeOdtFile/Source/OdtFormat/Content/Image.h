#pragma once
#ifndef ODT_CONTENT_IMAGE_INCLUDE_H_
#define ODT_CONTENT_IMAGE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include "./../Limit/Show.h"
#include "./../Limit/Actuate.h"
#include "./../Limit/ImageType.h"


namespace Odt
{
	namespace Content
	{
		class Image : public WritingElement
		{
		public:
			Image();
			virtual ~Image();
			explicit Image(const XML::XNode& node);
			const Image& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<boost::filesystem::wpath>			Href;
			nullable_property<std::string, Limit::ImageType>	Type;
			nullable_property<std::string, Limit::Show>			Show;
			nullable_property<std::string, Limit::Actuate>		Actuate;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_IMAGE_INCLUDE_H_
