#pragma once
#ifndef ODT_LIMIT_STYLE_FAMILY_INCLUDE_H_
#define ODT_LIMIT_STYLE_FAMILY_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class StyleFamily : public setter::from<std::string>
		{
		public:
			StyleFamily();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_STYLE_FAMILY_INCLUDE_H_