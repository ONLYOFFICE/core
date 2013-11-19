#pragma once
#ifndef OOX_LIMIT_BORDER_NAME_INCLUDE_H_
#define OOX_LIMIT_BORDER_NAME_INCLUDE_H_

#include "setter.h"
#include <string>

namespace OOX
{
	namespace Limit
	{
		class BorderNameSet
		{
		public:
			BorderNameSet();
			std::set<std::string>	_list;
		};

		class BorderName : public setter::base_setter<std::string>//from<std::string>
		{
		public:
			BorderName();

			void operator()(std::string& _value, Parameter value);
		public:
			static BorderNameSet	_set;
		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_BORDER_NAME_INCLUDE_H_