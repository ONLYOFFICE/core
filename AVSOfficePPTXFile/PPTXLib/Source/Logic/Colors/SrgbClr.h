#pragma once
#ifndef PPTX_LOGIC_SRGBCLR_INCLUDE_H_
#define PPTX_LOGIC_SRGBCLR_INCLUDE_H_

#include "property.h"
#include "ColorBase.h"

namespace PPTX
{
	namespace Logic
	{
		class SrgbClr : public ColorBase
		{
		public:
			SrgbClr();
			virtual ~SrgbClr();			
			explicit SrgbClr(const XML::XNode& node);
			const SrgbClr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SRGBCLR_INCLUDE_H