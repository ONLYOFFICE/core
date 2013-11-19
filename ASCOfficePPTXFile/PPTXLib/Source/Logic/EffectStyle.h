#pragma once
#ifndef PPTX_LOGIC_EFFECTSTYLE_INCLUDE_H_
#define PPTX_LOGIC_EFFECTSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "EffectProperties.h"
#include "Scene3d.h"
#include "Sp3d.h"

namespace PPTX
{
	namespace Logic
	{

		class EffectStyle : public WrapperWritingElement
		{
		public:
			EffectStyle();
			virtual ~EffectStyle();
			explicit EffectStyle(const XML::XNode& node);
			const EffectStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			EffectProperties EffectList;
			nullable_property<Scene3d> scene3d;
			nullable_property<Sp3d> sp3d;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTSTYLE_INCLUDE_H_