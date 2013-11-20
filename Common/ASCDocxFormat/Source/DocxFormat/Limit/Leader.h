#pragma once
#ifndef OOX_LIMIT_LEADER_INCLUDE_H_
#define OOX_LIMIT_LEADER_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class Leader : public setter::from<std::string>
		{
		public:
			Leader();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_LEADER_INCLUDE_H_