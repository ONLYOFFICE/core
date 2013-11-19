#pragma once
#ifndef PPTX_LIMIT_FLIP_INCLUDE_H_
#define PPTX_LIMIT_FLIP_INCLUDE_H_

#include "setter.h"
#include <string>

namespace PPTX
{
	namespace Limit
	{
		class Flip : public setter::from<std::string>
		{
		public:
			Flip();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_FLIP_INCLUDE_H_