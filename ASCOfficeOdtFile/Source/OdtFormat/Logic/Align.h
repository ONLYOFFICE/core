#pragma once
#ifndef ODT_LOGIC_ALIGN_INCLUDE_H_
#define ODT_LOGIC_ALIGN_INCLUDE_H_

#include "Common.h"
#include <string>


namespace Odt
{
	namespace Logic
	{
		class Align : public Common::Align
		{
		public:
			Align();
			Align(const Common::Align& align);
			Align(const std::string& value);

			const Align& operator= (const Common::Align& align);
			const Align& operator= (const std::string& value);

		public:
			const std::string ToString() const;
			void fromString(const std::string& value);
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_ALIGN_INCLUDE_H_