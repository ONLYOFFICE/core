#pragma once
#ifndef OOX_LIMIT_CONNECT_TYPE_INCLUDE_H_
#define OOX_LIMIT_CONNECT_TYPE_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class ConnectType : public setter::from<std::string>
		{
		public:
			ConnectType();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_CONNECT_TYPE_INCLUDE_H_