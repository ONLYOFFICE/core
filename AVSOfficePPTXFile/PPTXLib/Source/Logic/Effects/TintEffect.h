#pragma once
#ifndef PPTX_LOGIC_TINTEFFECT_INCLUDE_H_
#define PPTX_LOGIC_TINTEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class TintEffect : public WrapperWritingElement
		{
		public:
			TintEffect();
			virtual ~TintEffect();
			explicit TintEffect(const XML::XNode& node);
			const TintEffect& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int> amt;
			nullable_property<int, setter::between<int, 0, 21600000> > hue;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TINTEFFECT_INCLUDE_H_