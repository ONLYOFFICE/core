#pragma once
#ifndef PPTX_LIMIT_TEXTWRAP_INCLUDE_H_
#define PPTX_LIMIT_TEXTWRAP_INCLUDE_H_

#include "setter.h"
#include <string>

namespace PPTX
{
	namespace Limit
	{
		class TextWrap : public setter::from<std::string>
		{
		public:
			TextWrap();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTWRAP_INCLUDE_H_