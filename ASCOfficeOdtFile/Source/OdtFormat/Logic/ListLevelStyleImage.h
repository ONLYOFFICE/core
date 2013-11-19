#pragma once
#ifndef ODT_LOGIC_LIST_LEVEL_STYLE_IMAGE_INCLUDE_H_
#define ODT_LOGIC_LIST_LEVEL_STYLE_IMAGE_INCLUDE_H_

#include "ListLevelStyleItem.h"
#include "property.h"
#include "nullable_property.h"
#include <string>
#include "./../Limit/Show.h"
#include "./../Limit/Actuate.h"
#include "./../Limit/ImageType.h"


namespace Odt
{
	namespace Logic
	{
		class ListLevelStyleImage : public ListLevelStyleItem
		{
		public:
			ListLevelStyleImage();
			virtual ~ListLevelStyleImage();
			explicit ListLevelStyleImage(const XML::XNode& node);
			const ListLevelStyleImage& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:			
			nullable_property<std::string>						Href;
			nullable_property<std::string, Limit::ImageType>	Type;
			nullable_property<std::string, Limit::Show>			Show;
			nullable_property<std::string, Limit::Actuate>		Actuate;
			
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_LIST_LEVEL_STYLE_IMAGE_INCLUDE_H_