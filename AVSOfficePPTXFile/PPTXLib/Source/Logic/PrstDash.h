#pragma once
#ifndef PPTX_LOGIC_PRSTDASH_INCLUDE_H_
#define PPTX_LOGIC_PRSTDASH_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/PrstDashVal.h"

namespace PPTX
{
	namespace Logic
	{

		class PrstDash : public WrapperWritingElement
		{
		public:
			PrstDash();
			virtual ~PrstDash();
			explicit PrstDash(const XML::XNode& node);
			const PrstDash& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string, Limit::PrstDashVal> val;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PRSTDASH_INCLUDE_H_