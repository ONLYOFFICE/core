#pragma once
#ifndef PPTX_LOGIC_FILLEFFECT_INCLUDE_H_
#define PPTX_LOGIC_FILLEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniFill.h"

namespace PPTX
{
	namespace Logic
	{

		class FillEffect : public WrapperWritingElement
		{
		public:
			FillEffect();
			virtual ~FillEffect();
			explicit FillEffect(const XML::XNode& node);
			const FillEffect& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniFill Fill;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FILLEFFECT_INCLUDE_H_