#pragma once
#ifndef PPTX_LOGIC_ANIMEFFECT_INCLUDE_H_
#define PPTX_LOGIC_ANIMEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CBhvr.h"
#include "AnimVariant.h"
#include "./../../Limit/TLTransition.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimEffect : public WrapperWritingElement
		{
		public:
			AnimEffect();
			virtual ~AnimEffect();			
			explicit AnimEffect(const XML::XNode& node);
			const AnimEffect& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CBhvr> cBhvr;
			nullable_property<AnimVariant> progress;

			nullable_property<std::string> filter;
			nullable_property<std::string> prLst;
			nullable_property<std::string, Limit::TLTransition> transition;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMEFFECT_INCLUDE_H