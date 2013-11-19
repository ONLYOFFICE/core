#pragma once
#ifndef PPTX_LOGIC_BUSZPCT_INCLUDE_H_
#define PPTX_LOGIC_BUSZPCT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuSzPct : public WrapperWritingElement
		{
		public:
			BuSzPct();
			virtual ~BuSzPct();			
			explicit BuSzPct(const XML::XNode& node);
			const BuSzPct& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int, setter::between<int, 25000, 400000> > val;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUSZPCT_INCLUDE_H