#pragma once
#ifndef PPTX_LIMIT_SPLITTER_BAR_STATE_INCLUDE_H_
#define PPTX_LIMIT_SPLITTER_BAR_STATE_INCLUDE_H_

#include "setter.h"
#include <string>

namespace PPTX
{
	namespace Limit
	{
		class SplitterBarState : public setter::from<std::string>
		{
		public:
			SplitterBarState();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_SPLITTER_BAR_STATE_INCLUDE_H_