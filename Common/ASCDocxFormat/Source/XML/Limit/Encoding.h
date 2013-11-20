#pragma once
#ifndef XML_LIMIT_ENCODING_INCLUDE_H_
#define XML_LIMIT_ENCODING_INCLUDE_H_

#include <string>
#include "setter.h"


namespace XML
{
	namespace Limit
	{
		class Encoding : public setter::from<std::string>
		{
		public:
			Encoding();
			const std::string no_find() const;
		};
	} // namespace Limit
} // namespace XML

#endif // XML_LIMIT_ENCODING_INCLUDE_H_