#pragma once
#ifndef ODT_LIMIT_TAB_STOP_TYPE_INCLUDE_H_
#define ODT_LIMIT_TAB_STOP_TYPE_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class TabStopType : public setter::from<std::string>
		{
		public:
			TabStopType();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_TAB_STOP_TYPE_INCLUDE_H_