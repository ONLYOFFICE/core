#pragma once
#ifndef PPTX_VIEWPROPS_ORIGIN_INCLUDE_H_
#define PPTX_VIEWPROPS_ORIGIN_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Origin : public WrapperWritingElement
		{
		public:
			Origin();
			virtual ~Origin();
			explicit Origin(const XML::XNode& node);
			const Origin& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int> x;
			property<int> y;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_ORIGIN_INCLUDE_H_