#pragma once
#ifndef PPTX_LOGIC_LUMEFFECT_INCLUDE_H_
#define PPTX_LOGIC_LUMEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class LumEffect : public WrapperWritingElement
		{
		public:
			LumEffect();
			virtual ~LumEffect();
			explicit LumEffect(const XML::XNode& node);
			const LumEffect& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int> bright;
			nullable_property<int> contrast;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LUMEFFECT_INCLUDE_H_