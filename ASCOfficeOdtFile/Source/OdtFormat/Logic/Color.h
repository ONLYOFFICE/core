#pragma once
#ifndef ODT_LOGIC_COLOR_INCLUDE_H_
#define ODT_LOGIC_COLOR_INCLUDE_H_

#include "Common.h"
#include <string>


namespace Odt
{
	namespace Logic
	{
		class Color : public Common::Color
		{
		public:
			Color();
			Color(const Common::Color& color);
			Color(const std::string& value);

			const Color& operator= (const Common::Color& color);
			const Color& operator= (const std::string& value);

		public:
			const std::string ToString() const;
			void fromString(const std::string& value);
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_COLOR_INCLUDE_H_