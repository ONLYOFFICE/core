#pragma once
#ifndef ODT_LOGIC_BORDER_INCLUDE_H_
#define ODT_LOGIC_BORDER_INCLUDE_H_

#include "Color.h"
#include "./../Unit.h"
#include <string>
#include "nullable_property.h"
#include "./../Limit/BorderType.h"


namespace Odt
{
	namespace Logic
	{
		class Border
		{
		public:
			Border();
			Border(const std::string& value);

		public:
			const std::string ToString() const;
			const bool IsNone() const;

		public:
			nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	Width;
			nullable_property<std::string, Limit::BorderType>			BorderType;
			nullable_property<Color>									Color;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_BORDER_INCLUDE_H_