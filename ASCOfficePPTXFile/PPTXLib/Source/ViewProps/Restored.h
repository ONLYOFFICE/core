#pragma once
#ifndef PPTX_VIEWPROPS_RESTORED_INCLUDE_H_
#define PPTX_VIEWPROPS_RESTORED_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class Restored : public WrapperWritingElement
		{
		public:
			Restored();
			virtual ~Restored();
			explicit Restored(const XML::XNode& node);
			const Restored& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int, setter::only_positive<int> > sz;
			nullable_property<bool> autoAdjust;
		//private:
		public:
			property<std::string> name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_RESTORED_INCLUDE_H_