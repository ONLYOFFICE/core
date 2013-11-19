#pragma once
#ifndef ODT_LOGIC_HYPHENATE_INCLUDE_H_
#define ODT_LOGIC_HYPHENATE_INCLUDE_H_

#include "property.h"


namespace Odt
{
	namespace Logic
	{

		class Hyphenate
		{
		public:
			Hyphenate();

		public:
			property<bool>	Use;
			property<int, setter::between<int, 1, 10> >		Remain;
			property<int, setter::between<int, 1, 10> >		Push;
		};

	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_HYPHENATE_INCLUDE_H_