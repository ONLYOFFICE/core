#pragma once
#ifndef OOX_LOGIC_SHADOW_INCLUDE_H_
#define OOX_LOGIC_SHADOW_INCLUDE_H_

#include "./../WritingElement.h"
#include "./../Limit/TrueFalse.h"
#include <string>
#include "nullable_property.h"

namespace OOX
{
	namespace Logic
	{
		class Shadow : public WritingElement
		{
		public:
			Shadow();
			virtual ~Shadow();			
			explicit Shadow(const XML::XNode& node);
			const Shadow& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			
			nullable_property<std::string>	On;
			nullable_property<std::string>	Type;
			nullable_property<std::string>	Opacity;
			nullable_property<std::string>	Origin;
			nullable_property<std::string>	Matrix;
			nullable_property<std::string>	Color;
			nullable_property<std::string>	Color2;
			nullable_property<std::string>	Offset;
			nullable_property<std::string>	Offset2;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SHADOW_INCLUDE_H_