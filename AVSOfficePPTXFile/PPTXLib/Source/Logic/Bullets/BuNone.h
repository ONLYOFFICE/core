#pragma once
#ifndef PPTX_LOGIC_BUNONE_INCLUDE_H_
#define PPTX_LOGIC_BUNONE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuNone : public WrapperWritingElement
		{
		public:
			BuNone();
			virtual ~BuNone();			
			explicit BuNone(const XML::XNode& node);
			const BuNone& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUNONE_INCLUDE_H