#pragma once
#ifndef PPTX_LOGIC_BUSZPTS_INCLUDE_H_
#define PPTX_LOGIC_BUSZPTS_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuSzPts : public WrapperWritingElement
		{
		public:
			BuSzPts();
			virtual ~BuSzPts();			
			explicit BuSzPts(const XML::XNode& node);
			const BuSzPts& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int, setter::between<int, 100, 400000> > val;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUSZPTS_INCLUDE_H