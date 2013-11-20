#pragma once
#ifndef OOX_LOGIC_BACKGROUND_INCLUDE_H_
#define OOX_LOGIC_BACKGROUND_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "Color.h"


namespace OOX
{
	namespace Logic
	{
		class Background : public WritingElement
		{
		public:
			Background();
			virtual ~Background();
			explicit Background(const XML::XNode& node);
			const Background& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:			
			property<Color>			Color;
			nullable_property<std::string>	ThemeColor;
			nullable_property<std::string>	ThemeShade;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BACKGROUND_INCLUDE_H_