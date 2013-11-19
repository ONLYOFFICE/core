#pragma once
#ifndef PPTX_LIMIT_TLTRANSFORM_INCLUDE_H_
#define PPTX_LIMIT_TLTRANSFORM_INCLUDE_H_

#include "setter.h"
#include <string>

namespace PPTX
{
	namespace Limit
	{
		class TLTransform : public setter::from<std::string>
		{
		public:
			TLTransform();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_TLTRANSFORM_INCLUDE_H_