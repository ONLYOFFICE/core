#pragma once
#ifndef PPTX_VIEWPROPS_SLIDE_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_SLIDE_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CSldViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class SlideViewPr : public WrapperWritingElement
		{
		public:
			SlideViewPr();
			virtual ~SlideViewPr();
			explicit SlideViewPr(const XML::XNode& node);
			const SlideViewPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<nsViewProps::CSldViewPr> CSldViewPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_SLIDE_VIEW_PROPERTIES_INCLUDE_H_