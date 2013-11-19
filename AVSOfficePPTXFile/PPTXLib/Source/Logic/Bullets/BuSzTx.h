#pragma once
#ifndef PPTX_LOGIC_BUSZTX_INCLUDE_H_
#define PPTX_LOGIC_BUSZTX_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuSzTx : public WrapperWritingElement
		{
		public:
			BuSzTx();
			virtual ~BuSzTx();			
			explicit BuSzTx(const XML::XNode& node);
			const BuSzTx& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUSZTX_INCLUDE_H