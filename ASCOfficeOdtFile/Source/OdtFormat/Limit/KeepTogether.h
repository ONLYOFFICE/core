#pragma once
#ifndef ODT_LIMIT_KEEP_TOGETHER_INCLUDE_H_
#define ODT_LIMIT_KEEP_TOGETHER_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class KeepTogether : public setter::from<std::string>
		{
		public:
			KeepTogether();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_KEEP_TOGETHER_INCLUDE_H_