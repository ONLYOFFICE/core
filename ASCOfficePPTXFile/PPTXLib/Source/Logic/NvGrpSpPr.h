#pragma once
#ifndef PPTX_LOGIC_SLIDE_NVGRPSPPR_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_NVGRPSPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "CNvPr.h"
#include "CNvGrpSpPr.h"
#include "NvPr.h"

namespace PPTX
{
	namespace Logic
	{

		class NvGrpSpPr : public WrapperWritingElement
		{
		public:
			NvGrpSpPr();
			virtual ~NvGrpSpPr();
			explicit NvGrpSpPr(const XML::XNode& node);
			const NvGrpSpPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<CNvPr> cNvPr;
			property<CNvGrpSpPr> cNvGrpSpPr;
			property<NvPr> nvPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_NVGRPSPPR_INCLUDE_H_