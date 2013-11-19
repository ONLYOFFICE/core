#pragma once
#ifndef PPTX_LOGIC_SLIDE_GRPSPPR_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_GRPSPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/BWMode.h"
#include "Xfrm.h"
#include "UniFill.h"
#include "EffectProperties.h"
#include "Scene3d.h"

namespace PPTX
{
	namespace Logic
	{

		class GrpSpPr : public WrapperWritingElement
		{
		public:
			GrpSpPr();
			virtual ~GrpSpPr();
			explicit GrpSpPr(const XML::XNode& node);
			const GrpSpPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<Xfrm> xfrm;
			UniFill Fill;
			EffectProperties EffectList;
			nullable_property<Scene3d> scene3d;

			nullable_property<std::string, Limit::BWMode> bwMode;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_GRPSPPR_INCLUDE_H_
