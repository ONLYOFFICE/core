#pragma once
#ifndef ODT_LIMIT_NUMBER_WRAPPED_PARAGRAPHS_INCLUDE_H_
#define ODT_LIMIT_NUMBER_WRAPPED_PARAGRAPHS_INCLUDE_H_

#include "setter.h"
#include <string>


namespace Odt
{
	namespace Limit
	{
		class NumberWrappedParagraphs : public setter::from<std::string>
		{
		public:
			NumberWrappedParagraphs();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace Odt

#endif // ODT_LIMIT_NUMBER_WRAPPED_PARAGRAPHS_INCLUDE_H_