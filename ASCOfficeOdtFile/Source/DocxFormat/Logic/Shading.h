#pragma once
#ifndef OOX_LOGIC_SHADING_INCLUDE_H_
#define OOX_LOGIC_SHADING_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "nullable_property.h"

namespace OOX
{
	namespace Logic
	{
		class Shading : public WritingElement
		{
		public:
			Shading();
			virtual ~Shading();
			explicit Shading(const XML::XNode& node);
			const Shading& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:

			nullable_property<std::string>	value;
			nullable_property<std::string>	color;
			nullable_property<std::string>	fill;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SHADING_INCLUDE_H_