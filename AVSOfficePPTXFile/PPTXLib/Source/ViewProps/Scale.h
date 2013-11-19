#pragma once
#ifndef PPTX_VIEWPROPS_SCALE_INCLUDE_H_
#define PPTX_VIEWPROPS_SCALE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Ratio.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Scale : public WrapperWritingElement
		{
		public:
			Scale();
			virtual ~Scale();
			explicit Scale(const XML::XNode& node);
			const Scale& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<nsViewProps::Ratio> sx;
			property<nsViewProps::Ratio> sy;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_SCALE_INCLUDE_H_