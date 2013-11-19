#pragma once
#ifndef ODT_LOGIC_BACKGROUND_COLOR_INCLUDE_H_
#define ODT_LOGIC_BACKGROUND_COLOR_INCLUDE_H_

#include "Common.h"
#include <string>


namespace Odt
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
			const bool isTransparent() const;

		private:
			bool	m_transparent;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_BACKGROUND_COLOR_INCLUDE_H_