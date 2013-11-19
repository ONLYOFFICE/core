#pragma once
#ifndef PPTX_LIMIT_TEXTUNDERLINE_INCLUDE_H_
#define PPTX_LIMIT_TEXTUNDERLINE_INCLUDE_H_

#include "setter.h"
#include <string>

namespace PPTX
{
	namespace Limit
	{
		class TextUnderline : public setter::from<std::string>
		{
		public:
			TextUnderline();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TEXTUNDERLINE_INCLUDE_H_