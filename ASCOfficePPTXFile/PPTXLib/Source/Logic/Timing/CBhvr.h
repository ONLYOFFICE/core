#pragma once
#ifndef PPTX_LOGIC_CBHVR_INCLUDE_H_
#define PPTX_LOGIC_CBHVR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CTn.h"
#include "AttrNameLst.h"
#include "TgtEl.h"
#include "./../../Limit/TLAccumulate.h"
#include "./../../Limit/TLAdditive.h"
#include "./../../Limit/TLOverride.h"
#include "./../../Limit/TLTransform.h"

namespace PPTX
{
	namespace Logic
	{
		class CBhvr : public WrapperWritingElement
		{
		public:
			CBhvr();
			virtual ~CBhvr();			
			explicit CBhvr(const XML::XNode& node);
			const CBhvr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CTn> cTn;
			property<TgtEl> tgtEl;
			nullable_property<AttrNameLst> attrNameLst;

			nullable_property<std::string, Limit::TLAccumulate> accumulate; //ST_TLBehaviorAccumulateType 
			nullable_property<std::string, Limit::TLAdditive> additive; //ST_TLBehaviorAdditiveType 
			nullable_property<std::string> by; //W3C XML Schema string 
			nullable_property<std::string> from; //W3C XML Schema string 
			nullable_property<std::string, Limit::TLOverride> override_; //ST_TLBehaviorOverrideType 
			nullable_property<std::string> rctx; //"PPT"/"IE" W3C XML Schema string 
			nullable_property<std::string> to; //W3C XML Schema string 
			nullable_property<std::string, Limit::TLTransform> xfrmType; //ST_TLBehaviorTransformType 
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CBHVR_INCLUDE_H