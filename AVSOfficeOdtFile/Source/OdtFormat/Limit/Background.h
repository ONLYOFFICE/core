#pragma once
#ifndef ODT_LIMIT_BACKGROUND_INCLUDE_H_
#define ODT_LIMIT_BACKGROUND_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class Background : public setter::from<std::string>
		{
		public:
			Background();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_BACKGROUND_INCLUDE_H_