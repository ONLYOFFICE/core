#pragma once
#ifndef PPTX_LOGIC_CLRCHANGE_INCLUDE_H_
#define PPTX_LOGIC_CLRCHANGE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class ClrChange : public WrapperWritingElement
		{
		public:
			ClrChange();
			virtual ~ClrChange();
			explicit ClrChange(const XML::XNode& node);
			const ClrChange& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniColor ClrFrom;
			UniColor ClrTo;
			nullable_property<bool> useA;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CLRCHANGE_INCLUDE_H_