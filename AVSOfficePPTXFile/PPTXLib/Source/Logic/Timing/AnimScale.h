#pragma once
#ifndef PPTX_LOGIC_ANIMSCALE_INCLUDE_H_
#define PPTX_LOGIC_ANIMSCALE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CBhvr.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimScale : public WrapperWritingElement
		{
		public:
			AnimScale();
			virtual ~AnimScale();			
			explicit AnimScale(const XML::XNode& node);
			const AnimScale& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CBhvr> cBhvr;

			nullable_property<int> byX;
			nullable_property<int> byY;
			nullable_property<int> fromX;
			nullable_property<int> fromY;
			nullable_property<int> toX;
			nullable_property<int> toY;

			nullable_property<bool> zoomContents;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMSCALE_INCLUDE_H