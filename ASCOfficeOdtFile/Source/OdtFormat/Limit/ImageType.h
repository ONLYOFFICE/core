#pragma once
#ifndef ODT_LIMIT_IMAGE_TYPE_INCLUDE_H_
#define ODT_LIMIT_IMAGE_TYPE_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class ImageType : public setter::from<std::string>
		{
		public:
			ImageType();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_IMAGE_TYPE_INCLUDE_H_