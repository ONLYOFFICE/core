#pragma once
#ifndef ODP_CONTENT_TRANSITIONFILTER_INCLUDE_H_
#define ODP_CONTENT_TRANSITIONFILTER_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "../../Limit/AnimSubItem.h"
#include "../../Limit/Accumulate.h"
#include "../../Limit/Additive.h"
#include "../../Limit/CalcMode.h"
#include "../../Limit/AnimDirection.h"
#include "OdtFormat/Logic/Color.h"
#include "../../Limit/SmilFill.h"
#include "../../Limit/SmilFillDefault.h"
#include "../../Limit/SmilMode.h"
#include "../../Limit/Restart.h"
#include "../../Limit/RestartDefault.h"
#include "OdtFormat/Limit/SmilType.h"

namespace Odp
{
	namespace Content
	{
		class TransitionFilter : public Odt::WritingElement
		{
		public:
			TransitionFilter();
			virtual ~TransitionFilter();
			explicit TransitionFilter(const XML::XNode& node);
			const TransitionFilter& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> 						Formula;//anim:formula
			nullable_property<std::string, Limit::AnimSubItem>  SubItem;//anim:sub-item
			nullable_property<double>							Accelerate;//smil:accelerate
			nullable_property<std::string, Limit::Accumulate>   Accumulate;//smil:accumulate
			nullable_property<std::string, Limit::Additive>  	Additive;//smil:additive
			nullable_property<bool>								AutoReverse;//smil:autoReverse
			nullable_property<std::string> 						Begin;//smil:begin
			nullable_property<std::string>						By;//smil:by
			nullable_property<std::string, Limit::CalcMode>		CalcMode;//smil:calcMode
			nullable_property<double>							Decelerate;//smil:decelerate
			nullable_property<std::string, Limit::AnimDirection> Direction;//smil:direction
			nullable_property<std::string>						Dur;//smil:dur
			nullable_property<std::string>						End;//smil:end
			nullable_property<Odt::Logic::Color>				FadeColor;//smil:fadeColor
			nullable_property<std::string, Limit::SmilFill> 	Fill;//smil:fill
			nullable_property<std::string, Limit::SmilFillDefault> 	FillDefault;//smil:fillDefault
			nullable_property<std::string>						From;//smil:from
			nullable_property<std::string, Limit::SmilMode>		Mode;//smil:mode
			nullable_property<std::string>						RepeatCount;//smil:repeatCount
			nullable_property<std::string>						RepeatDur;//smil:repeatDur
			nullable_property<std::string, Limit::Restart>		Restart;//smil:restart
			nullable_property<std::string, Limit::RestartDefault>	RestartDefault;//smil:restartDefault
			nullable_property<std::string>						SubType;//smil:subtype
			nullable_property<std::string>						TargetElement;//smil:targetElement
			nullable_property<std::string>						To;//smil:to
			nullable_property<std::string, Odt::Limit::SmilType>	Type;//smil:type
			nullable_property<std::string>						Values;//smil:values
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_TRANSITIONFILTER_INCLUDE_H_