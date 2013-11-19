#pragma once
#ifndef PPTX_LOGIC_NVCXNSPPR_INCLUDE_H_
#define PPTX_LOGIC_NVCXNSPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "CNvPr.h"
#include "CNvCxnSpPr.h"
#include "NvPr.h"


namespace PPTX
{
	namespace Logic
	{
		class NvCxnSpPr : public WrapperWritingElement
		{
		public:
			NvCxnSpPr();
			virtual ~NvCxnSpPr();			
			explicit NvCxnSpPr(const XML::XNode& node);
			const NvCxnSpPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CNvPr> cNvPr;
			property<CNvCxnSpPr> cNvCxnSpPr;
			property<NvPr> nvPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NVCXNSPPR_INCLUDE_H