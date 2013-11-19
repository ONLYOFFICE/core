#pragma once
#ifndef ODT_LOGIC_LIST_LEVEL_STYLE_NUMBER_INCLUDE_H_
#define ODT_LOGIC_LIST_LEVEL_STYLE_NUMBER_INCLUDE_H_

#include "ListLevelStyleItem.h"
#include "property.h"
#include "nullable_property.h"
#include <string>
#include "NumFormat.h"


namespace Odt
{
	namespace Logic
	{
		class ListLevelStyleNumber : public ListLevelStyleItem
		{
		public:
			ListLevelStyleNumber();
			virtual ~ListLevelStyleNumber();
			explicit ListLevelStyleNumber(const XML::XNode& node);
			const ListLevelStyleNumber& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:			
			property<Logic::NumFormat>			NumFormat;
			nullable_property<bool>					NumLetterSync;			
			nullable_property<int>					StartValue;	
			nullable_property<std::string>	Suffix;
			nullable_property<std::string>	Prefix;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_LIST_LEVEL_STYLE_NUMBER_INCLUDE_H_