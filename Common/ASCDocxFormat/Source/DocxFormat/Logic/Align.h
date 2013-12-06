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
			Align(const std::wstring& value);

			const Align& operator= (const Common::Align& align);
			const Align& operator= (const std::string& value);
			const Align& operator= (const std::wstring& value);

		public:
			const std::string ToString() const;
			const std::wstring ToStringW() const;
			void fromString(const std::string& value);
			void fromString(const std::wstring& value);
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_ALIGN_INCLUDE_H_