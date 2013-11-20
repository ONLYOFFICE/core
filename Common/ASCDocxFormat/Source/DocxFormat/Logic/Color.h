#pragma once
#ifndef OOX_LOGIC_COLOR_INCLUDE_H_
#define OOX_LOGIC_COLOR_INCLUDE_H_

#include <string>
#include "Common.h"

namespace OOX
{
	namespace Logic
	{
		class Color : public Common::Color
		{
		public:
			Color();
			Color(const Common::Color& color);
			Color(const std::string& value);

			const Color& operator= (const Color& color);
			const Color& operator= (const Common::Color& color);
			const Color& operator= (const std::string& value);

			bool isAuto() const { return m_Auto; }

		public:
			const std::string ToString() const;
			void fromString(const std::string& value);
		private:
			bool m_Auto;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_COLOR_INCLUDE_H_