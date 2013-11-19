#pragma once
#ifndef PPTX_VIEWPROPS_OUTLINE_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_OUTLINE_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CViewPr.h"
#include "Sld.h"
#include <list>

namespace PPTX
{
	namespace nsViewProps
	{
		class OutlineViewPr : public WrapperWritingElement
		{
		public:
			OutlineViewPr();
			virtual ~OutlineViewPr();
			explicit OutlineViewPr(const XML::XNode& node);
			const OutlineViewPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<nsViewProps::CViewPr> CViewPr;
			property<std::list<nsViewProps::Sld> > SldLst;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_OUTLINE_VIEW_PROPERTIES_INCLUDE_H_