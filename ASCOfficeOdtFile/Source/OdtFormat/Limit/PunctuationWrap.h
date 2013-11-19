#pragma once
#ifndef ODT_LIMIT_PUNCTUATION_WRAP_INCLUDE_H_
#define ODT_LIMIT_PUNCTUATION_WRAP_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class PunctuationWrap : public setter::from<std::string>
		{
		public:
			PunctuationWrap();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_PUNCTUATION_WRAP_INCLUDE_H_