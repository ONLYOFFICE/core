#pragma once
#ifndef PPTX_LOGIC_ALPHAMOD_INCLUDE_H_
#define PPTX_LOGIC_ALPHAMOD_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../EffectDag.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaMod : public WrapperWritingElement
		{
		public:
			AlphaMod();
			virtual ~AlphaMod();
			explicit AlphaMod(const XML::XNode& node);
			const AlphaMod& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<EffectDag> cont;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAMOD_INCLUDE_H_