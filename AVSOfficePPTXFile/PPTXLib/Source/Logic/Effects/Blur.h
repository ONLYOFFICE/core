#pragma once
#ifndef PPTX_LOGIC_BLUR_INCLUDE_H_
#define PPTX_LOGIC_BLUR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class Blur : public WrapperWritingElement
		{
		public:
			Blur();
			virtual ~Blur();
			explicit Blur(const XML::XNode& node);
			const Blur& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<size_t> rad;
			nullable_property<bool> grow;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLUR_INCLUDE_H_