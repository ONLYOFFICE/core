#pragma once
#ifndef PPTX_LOGIC_RELOFF_INCLUDE_H_
#define PPTX_LOGIC_RELOFF_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class RelOff : public WrapperWritingElement
		{
		public:
			RelOff();
			virtual ~RelOff();
			explicit RelOff(const XML::XNode& node);
			const RelOff& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int> tx;
			nullable_property<int> ty;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RELOFF_INCLUDE_H_