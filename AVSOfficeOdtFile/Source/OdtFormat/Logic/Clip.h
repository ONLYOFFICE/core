#pragma once
#ifndef ODT_LOGIC_CLIP_INCLUDE_H_
#define ODT_LOGIC_CLIP_INCLUDE_H_

#include "Color.h"
#include <string>
#include "property.h"
#include "Unit.h"
#include "./../Unit.h"
#include "./../Limit/Clip.h"


namespace Odt
{
	namespace Logic
	{
		class Clip
		{
		public:
			Clip(const std::string& value);

		public:
			const std::string ToString() const;

		public:
			property<std::string, Limit::Clip> m_type;
			property<UnitCm, setter::between<UnitCm, 0, 30> > m_top;
			property<UnitCm, setter::between<UnitCm, 0, 30> > m_bottom;
			property<UnitCm, setter::between<UnitCm, 0, 30> > m_right;
			property<UnitCm, setter::between<UnitCm, 0, 30> > m_left;
	};
	} // namespace Logic
} // namespace Odt

#endif // ODT_BORDER_INCLUDE_H_