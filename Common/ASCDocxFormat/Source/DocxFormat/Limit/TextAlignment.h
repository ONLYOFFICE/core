#pragma once
#ifndef OOX_LIMIT_TEXT_ALIGNMENT_INCLUDE_H_
#define OOX_LIMIT_TEXT_ALIGNMENT_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class TextAlignment : public setter::from<std::string>
		{
		public:
			TextAlignment();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_TEXT_ALIGNMENT_INCLUDE_H_