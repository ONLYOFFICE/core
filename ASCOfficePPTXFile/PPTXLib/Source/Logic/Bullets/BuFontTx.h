#pragma once
#ifndef PPTX_LOGIC_BUFONTTX_INCLUDE_H_
#define PPTX_LOGIC_BUFONTTX_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuFontTx : public WrapperWritingElement
		{
		public:
			BuFontTx();
			virtual ~BuFontTx();			
			explicit BuFontTx(const XML::XNode& node);
			const BuFontTx& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUFONTTX_INCLUDE_H