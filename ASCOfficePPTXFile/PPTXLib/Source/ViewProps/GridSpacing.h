#pragma once
#ifndef PPTX_VIEWPROPS_GRIDSPACING_INCLUDE_H_
#define PPTX_VIEWPROPS_GRIDSPACING_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class GridSpacing : public WrapperWritingElement
		{
		public:
			GridSpacing();
			virtual ~GridSpacing();
			explicit GridSpacing(const XML::XNode& node);
			const GridSpacing& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int, setter::only_positive<int> > cx;
			property<int, setter::only_positive<int> > cy;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_GRIDSPACING_INCLUDE_H_