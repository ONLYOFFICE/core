#pragma once
#ifndef PPTX_LOGIC_TIMING_INCLUDE_H_
#define PPTX_LOGIC_TIMING_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "TnLst.h"
#include "BldLst.h"

namespace PPTX
{
	namespace Logic
	{
		class Timing : public WrapperWritingElement
		{
		public:
			Timing();
			virtual ~Timing();
			explicit Timing(const XML::XNode& node);
			const Timing& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<TnLst> tnLst;
			nullable_property<BldLst> bldLst;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TIMING_INCLUDE_H_