#pragma once
#ifndef PPTX_LOGIC_HSLEFFECT_INCLUDE_H_
#define PPTX_LOGIC_HSLEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class HslEffect : public WrapperWritingElement
		{
		public:
			HslEffect();
			virtual ~HslEffect();
			explicit HslEffect(const XML::XNode& node);
			const HslEffect& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int, setter::between<int, 0, 21600000> > hue;
			nullable_property<int> lum;
			nullable_property<int> sat;			
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_HSLEFFECT_INCLUDE_H_