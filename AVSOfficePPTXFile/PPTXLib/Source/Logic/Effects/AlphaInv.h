#pragma once
#ifndef PPTX_LOGIC_ALPHAINV_INCLUDE_H_
#define PPTX_LOGIC_ALPHAINV_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaInv : public WrapperWritingElement
		{
		public:
			AlphaInv();
			virtual ~AlphaInv();
			explicit AlphaInv(const XML::XNode& node);
			const AlphaInv& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniColor Color;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAINV_INCLUDE_H_