#pragma once
#ifndef OOX_LOGIC_BOTTOM_INCLUDE_H_
#define OOX_LOGIC_BOTTOM_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "Color.h"
#include "nullable.h"


namespace OOX
{
	namespace Logic
	{
		class Bottom : public WritingElement
		{
		public:
			Bottom();
			virtual ~Bottom();
			explicit Bottom(const XML::XNode& node);
			const Bottom& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;	

		public:
			nullable_property<std::string>		Value;
			nullable_property<int>				Space;
			nullable_property<int>				Size;
			nullable_property<Color>			Color;
			nullable_property<std::string>		ThemeColor;
			nullable_property<std::string>		ThemeShade;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BOTTOM_INCLUDE_H_