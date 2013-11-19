#pragma once
#ifndef PPTX_LOGIC_BUCLRTX_INCLUDE_H_
#define PPTX_LOGIC_BUCLRTX_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuClrTx : public WrapperWritingElement
		{
		public:
			BuClrTx();
			virtual ~BuClrTx();			
			explicit BuClrTx(const XML::XNode& node);
			const BuClrTx& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUCLRTX_INCLUDE_H