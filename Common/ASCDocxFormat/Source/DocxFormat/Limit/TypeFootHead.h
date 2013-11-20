#pragma once
#ifndef OOX_LIMIT_TYPE_FOOT_HEAD_INCLUDE_H_
#define OOX_LIMIT_TYPE_FOOT_HEAD_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class TypeFootHead : public setter::from<std::string>
		{
		public:
			TypeFootHead();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_TYPE_FOOT_HEAD_INCLUDE_H_