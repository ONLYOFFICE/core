#pragma once
#ifndef PPTX_LOGIC_TXEL_INCLUDE_H_
#define PPTX_LOGIC_TXEL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{
		class TxEl : public WrapperWritingElement
		{
		public:
			TxEl();
			virtual ~TxEl();
			explicit TxEl(const XML::XNode& node);
			const TxEl& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<size_t> st;
			nullable_property<size_t> end;
			nullable_property<bool> charRg;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TXEL_INCLUDE_H_