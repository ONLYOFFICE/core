#pragma once
#ifndef ODP_LIMIT_RESTART_DEFAULT_INCLUDE_H_
#define ODP_LIMIT_RESTART_DEFAULT_INCLUDE_H_

#include "setter.h"
#include <string>

namespace Odp
{
	namespace Limit
	{
		class RestartDefault : public setter::from<std::string>
		{
		public:
			RestartDefault();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odp

#endif // ODP_LIMIT_RESTART_DEFAULT_INCLUDE_H_