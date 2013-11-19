#pragma once
#ifndef PPTX_VIEWPROPS_SORTER_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_SORTER_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class SorterViewPr : public WrapperWritingElement
		{
		public:
			SorterViewPr();
			virtual ~SorterViewPr();
			explicit SorterViewPr(const XML::XNode& node);
			const SorterViewPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<nsViewProps::CViewPr> CViewPr;
			nullable_property<bool> attrShowFormatting;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_SORTER_VIEW_PROPERTIES_INCLUDE_H_