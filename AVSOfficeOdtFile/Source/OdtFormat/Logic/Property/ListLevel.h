#pragma once
#ifndef ODT_LOGIC_PROPERTY_LIST_LEVEL_INCLUDE_H_
#define ODT_LOGIC_PROPERTY_LIST_LEVEL_INCLUDE_H_

#include "./../../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable_property.h"
#include "./../../Unit.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{
			class ListLevel : public WritingElement
			{
			public:
				ListLevel();
				virtual ~ListLevel();
				explicit ListLevel(const XML::XNode& node);
				const ListLevel& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;

			public:
				static const ListLevel merge(const ListLevel& lhs, const ListLevel& rhs);

			public:
				const bool isSimple() const;

			public:
				nullable_property<std::string>		Mode;
				nullable_property<UniversalUnit>	SpaceBefore;				
				nullable_property<UniversalUnit>	MinLabelWidth;			
			};
		} // namespace Property
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTY_LIST_LEVEL_INCLUDE_H_