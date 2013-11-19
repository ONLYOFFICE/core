#pragma once
#ifndef PPTX_VIEWPROPS_COMMON_SLIDE_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_COMMON_SLIDE_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CViewPr.h"
#include "Guide.h"
#include <list>

namespace PPTX
{
	namespace nsViewProps
	{
		class CSldViewPr : public WrapperWritingElement
		{
		public:
			CSldViewPr();
			virtual ~CSldViewPr();
			explicit CSldViewPr(const XML::XNode& node);
			const CSldViewPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<nsViewProps::CViewPr> CViewPr;
			property<std::list<Guide> > GuideLst;

			nullable_property<bool> attrShowGuides;
			nullable_property<bool> attrSnapToGrid;
			nullable_property<bool> attrSnapToObjects;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_COMMON_SLIDE_VIEW_PROPERTIES_INCLUDE_H_