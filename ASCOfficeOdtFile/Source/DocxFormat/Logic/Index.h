#pragma once
#ifndef OOX_LOGIC_INDEX_INCLUDE_H_
#define OOX_LOGIC_INDEX_INCLUDE_H_

#include "Common.h"
#include <string>


namespace OOX
{
	namespace Logic
	{
		class Index : public Common::Index
		{
		public:
			Index();
			Index(const Common::Index& index);
			Index(const std::string& value);

			const Index& operator= (const Common::Index& index);
			const Index& operator= (const std::string& value);

		public:
			const std::string ToString() const;
			void fromString(const std::string& value);
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_INDEX_INCLUDE_H_