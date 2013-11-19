#pragma once
#ifndef PPTX_LOGIC_BLEND_INCLUDE_H_
#define PPTX_LOGIC_BLEND_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../EffectDag.h"
#include "./../../Limit/BlendMode.h"

namespace PPTX
{
	namespace Logic
	{

		class Blend : public WrapperWritingElement
		{
		public:
			Blend();
			virtual ~Blend();
			explicit Blend(const XML::XNode& node);
			const Blend& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<EffectDag> cont;
			property<std::string, Limit::BlendMode> blend;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLEND_INCLUDE_H_