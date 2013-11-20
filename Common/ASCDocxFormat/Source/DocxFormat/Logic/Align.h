#pragma once
#ifndef OOX_LOGIC_ALIGN_INCLUDE_H_
#define OOX_LOGIC_ALIGN_INCLUDE_H_

#include <string>
#include "Common.h"


namespace OOX
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
} // namespace OOX

#endif // OOX_LOGIC_ALIGN_INCLUDE_H_