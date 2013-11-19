#pragma once
#ifndef PPTX_LOGIC_EFFECTLST_INCLUDE_H_
#define PPTX_LOGIC_EFFECTLST_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "Effects\Blur.h"
#include "Effects\Glow.h"
#include "Effects\OuterShdw.h"
#include "Effects\PrstShdw.h"
#include "Effects\InnerShdw.h"
#include "Effects\Reflection.h"
#include "Effects\SoftEdge.h"
#include "Effects\FillOverlay.h"

namespace PPTX
{
	namespace Logic
	{

		class EffectLst : public WrapperWritingElement
		{
		public:
			EffectLst();
			virtual ~EffectLst();
			explicit EffectLst(const XML::XNode& node);
			const EffectLst& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<Blur> blur;
			nullable_property<FillOverlay> fillOverlay;
			nullable_property<Glow> glow;
			nullable_property<InnerShdw> innerShdw;
			nullable_property<OuterShdw> outerShdw;
			nullable_property<PrstShdw> prstShdw;
			nullable_property<Reflection> reflection;
			nullable_property<SoftEdge> softEdge;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTLST_INCLUDE_H_