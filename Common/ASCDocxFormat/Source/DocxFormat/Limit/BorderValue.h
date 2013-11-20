#pragma once
#ifndef OOX_LIMIT_BORDER_VALUE_INCLUDE_H_
#define OOX_LIMIT_BORDER_VALUE_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class BorderValueSet
		{
		public:
			BorderValueSet();
			std::set<std::string>	_list;
		};

		class BorderValue : public setter::base_setter<std::string>//from<std::string>
		{
		public:
			BorderValue();

			void operator()(std::string& _value, Parameter value);
		public:
			static BorderValueSet	_set;
		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_BORDER_VALUE_INCLUDE_H_