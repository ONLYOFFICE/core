#pragma once
#ifndef PPTX_LIMIT_PENALIGN_INCLUDE_H_
#define PPTX_LIMIT_PENALIGN_INCLUDE_H_

#include "setter.h"
#include <string>

namespace PPTX
{
	namespace Limit
	{
		class PenAlign : public setter::from<std::string>
		{
		public:
			PenAlign();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_PENALIGN_INCLUDE_H_