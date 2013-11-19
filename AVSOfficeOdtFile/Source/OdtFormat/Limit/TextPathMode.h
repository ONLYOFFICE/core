#pragma once
#ifndef ODT_LIMIT_TEXT_PATH_MODE_INCLUDE_H_
#define ODT_LIMIT_TEXT_PATH_MODE_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class TextPathMode: public setter::from<std::string>
		{
		public:
			TextPathMode();

		private:
			virtual const std::string no_find() const;		
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_TEXT_PATH_MODE_INCLUDE_H_