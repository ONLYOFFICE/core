#pragma once
#ifndef PPTX_LOGIC_EFFECTELEMENT_INCLUDE_H_
#define PPTX_LOGIC_EFFECTELEMENT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class EffectElement : public WrapperWritingElement
		{
		public:
			EffectElement();
			virtual ~EffectElement();
			explicit EffectElement(const XML::XNode& node);
			const EffectElement& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> ref;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTELEMENT_INCLUDE_H_