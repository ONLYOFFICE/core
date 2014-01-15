#pragma once
#ifndef OOX_LOGIC_FILL_INCLUDE_H_
#define OOX_LOGIC_FILL_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
#include <string>
#include "./../Limit/FillType.h"
#include "./../Unit.h"


namespace OOX
{
	namespace Logic
	{
		class Fill : public WritingElement
		{
		public:
			Fill();
			virtual ~Fill();			
			explicit Fill(const XML::XNode& node);
			const Fill& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>								Color;
			nullable_property<std::string>						Method;			
			nullable_property<std::string>						Colors;			
			nullable_property<std::string>						Rotate;
			nullable_property<UnitPercent>						Focus;
			nullable_property<std::string, Limit::FillType>		Type; 
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_FILL_INCLUDE_H_