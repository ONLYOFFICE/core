#pragma once
#ifndef ODT_LOGIC_NUM_FORMAT_INCLUDE_H_
#define ODT_LOGIC_NUM_FORMAT_INCLUDE_H_

#include "Common.h"
#include <string>


namespace Odt
{
	namespace Logic
	{
		class NumFormat : public Common::NumFormat
		{
		public:
			NumFormat();
			NumFormat(const Common::NumFormat& numFormat);
			NumFormat(const std::string& value);

			const NumFormat& operator= (const Common::NumFormat& numFormat);
			const NumFormat& operator= (const std::string& value);

		public:
			const std::string ToString() const;
			void fromString(const std::string& value);
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_NUM_FORMAT_INCLUDE_H_