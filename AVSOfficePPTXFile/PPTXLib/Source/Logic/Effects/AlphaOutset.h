#pragma once
#ifndef PPTX_LOGIC_ALPHAOUTSET_INCLUDE_H_
#define PPTX_LOGIC_ALPHAOUTSET_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaOutset : public WrapperWritingElement
		{
		public:
			AlphaOutset();
			virtual ~AlphaOutset();
			explicit AlphaOutset(const XML::XNode& node);
			const AlphaOutset& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<size_t> rad;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAOUTSET_INCLUDE_H_