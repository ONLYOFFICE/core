#pragma once
#ifndef PPTX_LOGIC_SLIDE_BGPR_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_BGPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "UniFill.h"
#include "EffectProperties.h"

namespace PPTX
{
	namespace Logic
	{

		class BgPr : public WrapperWritingElement
		{
		public:
			BgPr();
			virtual ~BgPr();
			explicit BgPr(const XML::XNode& node);
			const BgPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniFill Fill;
			EffectProperties EffectList;
 
			nullable_property<bool> shadeToTitle;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_BGPR_INCLUDE_H_