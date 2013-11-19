#pragma once
#ifndef PPTX_LOGIC_ANIMROT_INCLUDE_H_
#define PPTX_LOGIC_ANIMROT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CBhvr.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimRot : public WrapperWritingElement
		{
		public:
			AnimRot();
			virtual ~AnimRot();			
			explicit AnimRot(const XML::XNode& node);
			const AnimRot& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CBhvr> cBhvr;

			nullable_property<int> by;
			nullable_property<int> from;
			nullable_property<int> to;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMROT_INCLUDE_H