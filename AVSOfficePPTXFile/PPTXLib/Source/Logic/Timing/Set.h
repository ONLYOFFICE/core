#pragma once
#ifndef PPTX_LOGIC_SET_INCLUDE_H_
#define PPTX_LOGIC_SET_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CBhvr.h"
#include "AnimVariant.h"

namespace PPTX
{
	namespace Logic
	{
		class Set : public WrapperWritingElement
		{
		public:
			Set();
			virtual ~Set();			
			explicit Set(const XML::XNode& node);
			const Set& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CBhvr> cBhvr;
			nullable_property<AnimVariant> to;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SET_INCLUDE_H