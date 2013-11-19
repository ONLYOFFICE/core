#pragma once
#ifndef PPTX_LOGIC_ANIM_INCLUDE_H_
#define PPTX_LOGIC_ANIM_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CBhvr.h"
#include "TavLst.h"
#include "./../../Limit/TLCalcMode.h"
#include "./../../Limit/TLValueType.h"

namespace PPTX
{
	namespace Logic
	{
		class Anim : public WrapperWritingElement
		{
		public:
			Anim();
			virtual ~Anim();			
			explicit Anim(const XML::XNode& node);
			const Anim& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CBhvr> cBhvr;
			nullable_property<TavLst> tavLst;

			nullable_property<std::string> by; //W3C XML Schema string 
			nullable_property<std::string, Limit::TLCalcMode> calcmode; //ST_TLAnimateBehaviorCalcMode
			nullable_property<std::string> from; //W3C XML Schema string 
			nullable_property<std::string> to; //W3C XML Schema string 
			nullable_property<std::string, Limit::TLValueType> valueType; //ST_TLAnimateBehaviorValueType
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIM_INCLUDE_H