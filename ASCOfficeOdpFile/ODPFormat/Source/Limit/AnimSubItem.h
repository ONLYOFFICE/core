#pragma once
#ifndef ODP_LIMIT_SUBITEM_INCLUDE_H_
#define ODP_LIMIT_SUBITEM_INCLUDE_H_

#include "setter.h"
#include <string>

namespace Odp
{
	namespace Limit
	{
		class AnimSubItem : public setter::from<std::string>
		{
		public:
			AnimSubItem();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odp

#endif // ODP_LIMIT_SUBITEM_INCLUDE_H_