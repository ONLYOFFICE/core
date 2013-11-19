#pragma once
#ifndef PPTX_LIMIT_SYSCLRVAL_INCLUDE_H_
#define PPTX_LIMIT_SYSCLRVAL_INCLUDE_H_

#include "setter.h"
#include <string>

namespace PPTX
{
	namespace Limit
	{
		class SysClrVal : public setter::from<std::string>
		{
		public:
			SysClrVal();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_SYSCLRVAL_INCLUDE_H_