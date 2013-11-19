#pragma once
#ifndef PPTX_VIEWPROPS_GUIDE_INCLUDE_H_
#define PPTX_VIEWPROPS_GUIDE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/Orient.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Guide : public WrapperWritingElement
		{
		public:
			Guide();
			virtual ~Guide();
			explicit Guide(const XML::XNode& node);
			const Guide& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int, setter::only_positive<int> > pos;
			nullable_property<std::string, Limit::Orient> orient;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_GUIDE_INCLUDE_H_