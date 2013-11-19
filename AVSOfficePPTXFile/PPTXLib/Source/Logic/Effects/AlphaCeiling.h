#pragma once
#ifndef PPTX_LOGIC_ALPHACEILING_INCLUDE_H_
#define PPTX_LOGIC_ALPHACEILING_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaCeiling : public WrapperWritingElement
		{
		public:
			AlphaCeiling();
			virtual ~AlphaCeiling();
			explicit AlphaCeiling(const XML::XNode& node);
			const AlphaCeiling& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHACEILING_INCLUDE_H_