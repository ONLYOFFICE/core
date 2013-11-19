#pragma once
#ifndef PPTX_LOGIC_NVGRAPHICFRAMEPR_INCLUDE_H_
#define PPTX_LOGIC_NVGRAPHICFRAMEPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "CNvPr.h"
#include "CNvGraphicFramePr.h"
#include "NvPr.h"


namespace PPTX
{
	namespace Logic
	{
		class NvGraphicFramePr : public WrapperWritingElement
		{
		public:
			NvGraphicFramePr();
			virtual ~NvGraphicFramePr();			
			explicit NvGraphicFramePr(const XML::XNode& node);
			const NvGraphicFramePr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CNvPr> cNvPr;
			property<CNvGraphicFramePr> cNvGraphicFramePr;
			property<NvPr> nvPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NVGRAPHICFRAMEPR_INCLUDE_H