#pragma once
#ifndef OOX_LIMIT_RELATIVE_FROM_INCLUDE_H_
#define OOX_LIMIT_RELATIVE_FROM_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class RelativeFrom : public setter::from<std::string>
		{
		public:
			RelativeFrom();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_RELATIVE_FROM_INCLUDE_H_