#pragma once
#ifndef ODT_LOGIC_COLUMNS_INCLUDE_H_
#define ODT_LOGIC_COLUMNS_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "Utility.h"


namespace Odt
{
	namespace Logic
	{
		class Columns : public WritingElement
		{
		public:
			Columns();
			virtual ~Columns();
			explicit Columns(const XML::XNode& node);
			const Columns& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int>		 ColumnCount;
			property<UniversalUnit> ColumnGap;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_COLUMNS_INCLUDE_H_