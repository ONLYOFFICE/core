#pragma once
#ifndef PPTX_LOGIC_NVSPPR_INCLUDE_H_
#define PPTX_LOGIC_NVSPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "CNvPr.h"
#include "CNvSpPr.h"
#include "NvPr.h"


namespace PPTX
{
	namespace Logic
	{
		class NvSpPr : public WrapperWritingElement
		{
		public:
			NvSpPr();
			virtual ~NvSpPr();			
			explicit NvSpPr(const XML::XNode& node);
			const NvSpPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CNvPr> cNvPr;
			property<CNvSpPr> cNvSpPr;
			property<NvPr> nvPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NVSPPR_INCLUDE_H