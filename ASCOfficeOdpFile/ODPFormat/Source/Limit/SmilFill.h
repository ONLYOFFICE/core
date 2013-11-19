#pragma once
#ifndef ODP_LIMIT_SMIL_FILL_INCLUDE_H_
#define ODP_LIMIT_SMIL_FILL_INCLUDE_H_

#include "setter.h"
#include <string>

namespace Odp
{
	namespace Limit
	{
		class SmilFill : public setter::from<std::string>
		{
		public:
			SmilFill();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odp

#endif // ODP_LIMIT_SMIL_FILL_INCLUDE_H_