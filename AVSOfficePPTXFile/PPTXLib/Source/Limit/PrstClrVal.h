#pragma once
#ifndef PPTX_LIMIT_PRSTCLRVAL_INCLUDE_H_
#define PPTX_LIMIT_PRSTCLRVAL_INCLUDE_H_

#include "setter.h"
#include <string>

namespace PPTX
{
	namespace Limit
	{
		class PrstClrVal : public setter::from<std::string>
		{
		public:
			PrstClrVal();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_PRSTCLRVAL_INCLUDE_H_