#pragma once
#ifndef PPTX_VIEWPROPS_COMMON_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_COMMON_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Origin.h"
#include "Scale.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class CViewPr : public WrapperWritingElement
		{
		public:
			CViewPr();
			virtual ~CViewPr();
			explicit CViewPr(const XML::XNode& node);
			const CViewPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<bool> attrVarScale;
			property<nsViewProps::Origin> Origin;
			property<nsViewProps::Scale> Scale;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_COMMON_VIEW_PROPERTIES_INCLUDE_H_