#pragma once
#ifndef PPTX_LOGIC_SEQ_INCLUDE_H_
#define PPTX_LOGIC_SEQ_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CTn.h"
#include "CondLst.h"
#include "./../../Limit/TLPrevAc.h"
#include "./../../Limit/TLNextAc.h"

namespace PPTX
{
	namespace Logic
	{
		class Seq : public WrapperWritingElement
		{
		public:
			Seq();
			virtual ~Seq();			
			explicit Seq(const XML::XNode& node);
			const Seq& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CTn> cTn;
			nullable_property<CondLst> nextCondLst;
			nullable_property<CondLst> prevCondLst;

			nullable_property<bool> concurrent;
			nullable_property<std::string, Limit::TLNextAc> nextAc;
			nullable_property<std::string, Limit::TLPrevAc> prevAc;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SEQ_INCLUDE_H