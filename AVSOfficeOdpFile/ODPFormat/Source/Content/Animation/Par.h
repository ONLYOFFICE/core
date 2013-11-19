#pragma once
#ifndef ODP_CONTENT_PAR_INCLUDE_H_
#define ODP_CONTENT_PAR_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "../../Limit/NodeType.h"
#include "../../Limit/PresetClass.h"
#include "../../Limit/SmilFill.h"
#include "../../Limit/SmilFillDefault.h"
#include "../../Limit/Restart.h"
#include "../../Limit/RestartDefault.h"
#include <vector>
#include "AnimationElement.h"

namespace Odp
{
	namespace Content
	{
		class Par : public Odt::WritingElement
		{
		public:
			Par();
			virtual ~Par();
			explicit Par(const XML::XNode& node);
			const Par& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> 						AnimId;//anim:id
			nullable_property<std::string> 						GroupId;//presentation:group-id
			nullable_property<std::string> 						MasterElement;//presentation:master-element
			nullable_property<std::string, Limit::NodeType> 	NodeType;//presentation:node-type
			nullable_property<std::string, Limit::PresetClass> 	PresetClass;//presentation:preset-class
			nullable_property<std::string> 						PresetId;//presentation:preset-id
			nullable_property<std::string> 						PresetSubType;//presentation:preset-sub-type
			nullable_property<double>							Accelerate;//smil:accelerate
			nullable_property<bool>								AutoReverse;//smil:autoReverse
			nullable_property<std::string> 						Begin;//smil:begin
			nullable_property<double>							Decelerate;//smil:decelerate
			nullable_property<std::string>						Dur;//smil:dur
			nullable_property<std::string>						End;//smil:end
			nullable_property<std::string>						EndSync;//smil:endsync
			nullable_property<std::string, Limit::SmilFill> 	Fill;//smil:fill
			nullable_property<std::string, Limit::SmilFillDefault> 	FillDefault;//smil:fillDefault
			nullable_property<std::string>						RepeatCount;//smil:repeatCount
			nullable_property<std::string>						RepeatDur;//smil:repeatDur
			nullable_property<std::string, Limit::Restart>		Restart;//smil:restart
			nullable_property<std::string, Limit::RestartDefault>	RestartDefault;//smil:restartDefault
			//nullable_property<std::string> 						XmlId;//xml:id

			property<std::vector<AnimationElement> > Childs;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_PAR_INCLUDE_H_