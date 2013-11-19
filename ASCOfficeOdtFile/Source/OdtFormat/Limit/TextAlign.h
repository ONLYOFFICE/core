#pragma once
#ifndef ODT_LIMIT_TEXT_ALIGN_INCLUDE_H_
#define ODT_LIMIT_TEXT_ALIGN_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class TextAlign : public setter::from<std::string>
		{
		public:
			TextAlign();

		private:
			virtual const std::string no_find() const;		
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_TEXT_ALIGN_INCLUDE_H_