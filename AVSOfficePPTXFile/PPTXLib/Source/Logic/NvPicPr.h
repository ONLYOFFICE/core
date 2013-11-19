#pragma once
#ifndef PPTX_LOGIC_NVPICPR_INCLUDE_H_
#define PPTX_LOGIC_NVPICPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "CNvPr.h"
#include "CNvPicPr.h"
#include "NvPr.h"


namespace PPTX
{
	namespace Logic
	{
		class NvPicPr : public WrapperWritingElement
		{
		public:
			NvPicPr();
			virtual ~NvPicPr();			
			explicit NvPicPr(const XML::XNode& node);
			const NvPicPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CNvPr> cNvPr;
			property<CNvPicPr> cNvPicPr;
			property<NvPr> nvPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NVPICPR_INCLUDE_H