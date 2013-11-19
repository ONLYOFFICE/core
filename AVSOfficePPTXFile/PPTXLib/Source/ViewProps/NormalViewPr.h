#pragma once
#ifndef PPTX_VIEWPROPS_NORMAL_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_NORMAL_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Restored.h"
#include "./../Limit/SplitterBarState.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class NormalViewPr : public WrapperWritingElement
		{
		public:
			NormalViewPr();
			virtual ~NormalViewPr();
			explicit NormalViewPr(const XML::XNode& node);
			const NormalViewPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<nsViewProps::Restored> restoredLeft;
			property<nsViewProps::Restored> restoredTop;
			nullable_property<std::string, Limit::SplitterBarState> attrHorzBarState;
			nullable_property<std::string, Limit::SplitterBarState> attrVertBarState;
			nullable_property<bool> attrPreferSingleView;
			nullable_property<bool> attrShowOutlineIcons;
			nullable_property<bool> attrSnapVertSplitter;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_NORMAL_VIEW_PROPERTIES_INCLUDE_H_