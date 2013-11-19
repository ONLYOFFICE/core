#pragma once
#ifndef PPTX_LOGIC_ATTRNAME_INCLUDE_H_
#define PPTX_LOGIC_ATTRNAME_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{
		class AttrName : public WrapperWritingElement
		{
		public:
			AttrName();
			virtual ~AttrName();
			explicit AttrName(const XML::XNode& node);
			const AttrName& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> text;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ATTRNAME_INCLUDE_H_