#pragma once
#ifndef PPTX_LOGIC_HF_INCLUDE_H_
#define PPTX_LOGIC_HF_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class HF : public WrapperWritingElement
		{
		public:
			HF();
			virtual ~HF();
			explicit HF(const XML::XNode& node);
			const HF& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<bool> dt;
			nullable_property<bool> ftr;
			nullable_property<bool> hdr;
			nullable_property<bool> sldNum;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_HF_INCLUDE_H_