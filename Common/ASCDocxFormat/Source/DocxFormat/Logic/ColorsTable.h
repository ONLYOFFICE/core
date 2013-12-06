#pragma once
#ifndef OOX_LOGIC_COLORS_TABLE_INCLUDE_H_
#define OOX_LOGIC_COLORS_TABLE_INCLUDE_H_

#include "Common.h"
#include <vector>
#include <utility>

namespace OOX
{
	namespace Logic
	{
		class ColorsTable
		{
		public:
			ColorsTable();

		public:
			const Common::Color fromName(const std::string& name) const;
			const std::string fromColor(const Common::Color& color) const ;

			const bool isFromName(const std::string& name) const;

		private:
			std::vector<std::pair<std::string, Common::Color> >		m_storage;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_COLORS_TABLE_INCLUDE_H_