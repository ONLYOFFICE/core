#pragma once
#ifndef PPTX_LOGIC_COLORMODIFIER_INCLUDE_H_
#define PPTX_LOGIC_COLORMODIFIER_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"

namespace PPTX
{
	namespace Logic
	{
		class ColorModifier : public WrapperWritingElement
		{
		public:
			ColorModifier();
			virtual ~ColorModifier();			
			explicit ColorModifier(const XML::XNode& node);
			const ColorModifier& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> name;
			nullable_property<int> val;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_COLORMODIFIER_INCLUDE_H