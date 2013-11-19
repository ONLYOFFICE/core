#pragma once
#ifndef OOX_LOGIC_STROKE_INCLUDE_H_
#define OOX_LOGIC_STROKE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "./../Limit/Join.h"

namespace OOX
{
	namespace Logic
	{
		class Stroke : public WritingElement
		{
		public:
			Stroke();
			virtual ~Stroke();			
			explicit Stroke(const XML::XNode& node);
			const Stroke& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::Join>	Join;
			nullable_property<std::string>		DashStyle;
			nullable_property<std::string>		EndCap;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_STROKE_INCLUDE_H_