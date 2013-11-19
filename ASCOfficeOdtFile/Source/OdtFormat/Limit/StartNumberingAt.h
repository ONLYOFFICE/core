#pragma once
#ifndef ODT_LIMIT_START_NUMBERING_AT_INCLUDE_H_
#define ODT_LIMIT_START_NUMBERING_AT_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class StartNumberingAt : public setter::from<std::string>
		{
		public:
			StartNumberingAt();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_START_NUMBERING_AT_INCLUDE_H_