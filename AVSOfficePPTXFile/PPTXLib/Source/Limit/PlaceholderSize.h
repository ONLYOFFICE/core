#pragma once
#ifndef PPTX_LIMIT_PLACEHOLDERSIZE_INCLUDE_H_
#define PPTX_LIMIT_PLACEHOLDERSIZE_INCLUDE_H_

#include "setter.h"
#include <string>

namespace PPTX
{
	namespace Limit
	{
		class PlaceholderSize : public setter::from<std::string>
		{
		public:
			PlaceholderSize();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_PLACEHOLDERSIZE_INCLUDE_H_