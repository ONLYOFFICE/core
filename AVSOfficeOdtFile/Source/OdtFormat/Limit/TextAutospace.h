#pragma once
#ifndef ODT_LIMIT_TEXT_AUTOSPACE_INCLUDE_H_
#define ODT_LIMIT_TEXT_AUTOSPACE_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class TextAutospace : public setter::from<std::string>
		{
		public:
			TextAutospace();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_TEXT_AUTOSPACE_INCLUDE_H_