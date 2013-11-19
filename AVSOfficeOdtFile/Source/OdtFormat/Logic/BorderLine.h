#pragma once
#ifndef ODT_LOGIC_BORDER_LINE_INCLUDE_H_
#define ODT_LOGIC_BORDER_LINE_INCLUDE_H_


#include "./../Unit.h"
#include <string>
#include "property.h"


namespace Odt
{
	namespace Logic
	{
		class BorderLine
		{
		public:
			BorderLine();
			BorderLine(const std::string& value);

		public:
			const std::string ToString() const;

		public:
			property<UnitCm, setter::between<UnitCm, 0, 30> >	FirstWidth;
			property<UnitCm, setter::between<UnitCm, 0, 30> >	SecondWidth;
			property<UnitCm, setter::between<UnitCm, 0, 30> >	ThirdWidth;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_BORDER_LINE_INCLUDE_H_