#pragma once
#ifndef ODP_LIMIT_DIRECTION_INCLUDE_H_
#define ODP_LIMIT_DIRECTION_INCLUDE_H_

#include "setter.h"
#include <string>

namespace Odp
{
	namespace Limit
	{
		class AnimDirection : public setter::from<std::string>
		{
		public:
			AnimDirection();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odp

#endif // ODP_LIMIT_DIRECTION_INCLUDE_H_