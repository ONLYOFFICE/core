#pragma once
#ifndef OOX_LIMIT_CMPD_INCLUDE_H_
#define OOX_LIMIT_CMPD_INCLUDE_H_

#include "setter.h"
#include <string>


namespace OOX
{
	namespace Limit
	{
		class Cmpd : public setter::from<std::string>
		{
		public:
			Cmpd();

		private:
			virtual const std::string no_find() const;
		};
	} // namespace Limit
} // namespace OOX

#endif // OOX_LIMIT_CMPD_INCLUDE_H_