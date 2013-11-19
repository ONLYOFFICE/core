#pragma once
#ifndef ODT_LIMIT_WRITING_MODE_INCLUDE_H_
#define ODT_LIMIT_WRITING_MODE_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class WritingMode : public setter::from<std::string>
		{
		public:
			WritingMode();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_WRITING_MODE_INCLUDE_H_