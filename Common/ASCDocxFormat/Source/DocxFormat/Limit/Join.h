#pragma once
#ifndef OOX_LIMIT_JOIN_INCLUDE_H_
#define OOX_LIMIT_JOIN_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class Join : public setter::from<std::string>
		{
		public:
			Join();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_JOIN_INCLUDE_H_