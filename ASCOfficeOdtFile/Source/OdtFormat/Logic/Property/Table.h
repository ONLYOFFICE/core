#pragma once
#ifndef ODT_LOGIC_PROPERTY_TABLE_INCLUDE_H_
#define ODT_LOGIC_PROPERTY_TABLE_INCLUDE_H_

#include "./../../WritingElement.h"
//#include "./../../Unit.h"
#include <string>
#include "nullable_property.h"
#include "./../../Limit/Align.h"
#include "./../../Limit/BorderModel.h"
#include "./../../Limit/KeepWithNext.h"
#include "./../../Limit/WritingMode.h"
#include "UniversalUnit.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{
			class Table : public WritingElement
			{
			public:
				Table();
				virtual ~Table();
				explicit Table(const XML::XNode& node);
				const Table& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;

			public:
				static const Table merge(const Table& lhs, const Table& rhs);

			public:
				const bool isSimple() const;

			public:
				nullable_property<std::string, Limit::BorderModel>		BorderModel;
				nullable_property<UniversalUnit>						Width;
				nullable_property<std::string, Limit::Align>			Align;
				nullable_property<int>									PageNumber;
				nullable_property<UniversalUnit>						MarginLeft;
				nullable_property<UniversalUnit>						MarginRight;
				nullable_property<std::string, Limit::KeepWithNext>		KeepWithNext;
				nullable_property<std::string, Limit::WritingMode>		WritingMode;
			};
		} // namespace Property
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTY_TABLE_INCLUDE_H_