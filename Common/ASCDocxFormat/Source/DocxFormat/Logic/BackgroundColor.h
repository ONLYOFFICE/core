#pragma once
#ifndef OOX_LOGIC_BACKGROUND_COLOR_INCLUDE_H_
#define OOX_LOGIC_BACKGROUND_COLOR_INCLUDE_H_

#include "Common.h"
#include <string>


namespace OOX
{
	namespace Logic
	{
		class BackgroundColor : public Common::Color
		{
		public:
			BackgroundColor();
			BackgroundColor(const Common::Color& color);
			BackgroundColor(const std::string& value);

			const BackgroundColor& operator= (const Common::Color& color);
			const BackgroundColor& operator= (const std::string& value);

		public:
			const std::string ToString() const;
			void fromString(const std::string& value);
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BACKGROUND_COLOR_INCLUDE_H_