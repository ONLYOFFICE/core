#pragma once
#ifndef ODT_LOGIC_TAB_STOP_INCLUDE_H_
#define ODT_LOGIC_TAB_STOP_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "nullable_property.h"
//#include "./../Unit.h"
#include "./../Limit/TabStopType.h"
#include "./../Limit/LeaderStyle.h"
#include "./../Limit/LeaderText.h"
#include "Utility.h"


namespace Odt
{
	namespace Logic
	{
		class TabStop : public WritingElement
		{
		public:
			TabStop();
			virtual ~TabStop();
			explicit TabStop(const XML::XNode& node);
			const TabStop& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<UniversalUnit> Position;
			nullable_property<std::string, Limit::TabStopType> Type;
			nullable_property<std::string, Limit::LeaderStyle> LeaderStyle;
			nullable_property<std::string, Limit::LeaderText>  LeaderText;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_TAB_STOP_INCLUDE_H_