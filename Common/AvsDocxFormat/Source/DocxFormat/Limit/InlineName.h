#pragma once
#ifndef OOX_LIMIT_INLINE_NAME_INCLUDE_H_
#define OOX_LIMIT_INLINE_NAME_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class InlineName : public setter::from<std::string>
		{
		public:
			InlineName();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_INLINE_NAME_INCLUDE_H_