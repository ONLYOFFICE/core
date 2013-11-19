#pragma once
#ifndef PPTX_VIEWPROPS_RATIO_INCLUDE_H_
#define PPTX_VIEWPROPS_RATIO_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Ratio : public WrapperWritingElement
		{
		public:
			Ratio();
			virtual ~Ratio();
			explicit Ratio(const XML::XNode& node);
			const Ratio& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int, setter::only_positive<int> > d;
			property<int, setter::only_positive<int> > n;
		//private:
		public:
			property<std::string> name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_RATIO_INCLUDE_H_