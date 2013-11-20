#pragma once
#ifndef OOX_LOGIC_IMAGE_DATA_INCLUDE_H_
#define OOX_LOGIC_IMAGE_DATA_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include <string>
#include "./../RId.h"


namespace OOX
{
	namespace Logic
	{
		class ImageData : public WritingElement
		{
		public:
			ImageData();
			virtual ~ImageData();
			explicit ImageData(const XML::XNode& node);
			const ImageData& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<OOX::RId>				rId;
			nullable_property<std::string>	Title;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_IMAGE_DATA_INCLUDE_H_