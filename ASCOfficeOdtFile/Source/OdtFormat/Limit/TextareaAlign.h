#pragma once
#ifndef ODT_LIMIT_TEXTAREA_ALIGN_INCLUDE_H_
#define ODT_LIMIT_TEXTAREA_ALIGN_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class TextareaAlign : public setter::from<std::string>
		{
		public:
			TextareaAlign();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_TEXTAREA_ALIGN_INCLUDE_H_