#pragma once
#ifndef OOX_LIMIT_HORIZONTAL_ANCHOR_LOCATION_INCLUDE_H_
#define OOX_LIMIT_HORIZONTAL_ANCHOR_LOCATION_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class HorizontalAnchorLocation : public setter::from<std::string>
		{
		public:
			HorizontalAnchorLocation();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_HORIZONTAL_ANCHOR_LOCATION_INCLUDE_H_