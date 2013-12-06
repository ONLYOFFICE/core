#pragma once
#ifndef OOX_LOGIC_RUN_ITEM_BASE_INCLUDE_H_
#define OOX_LOGIC_RUN_ITEM_BASE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>

namespace OOX
{
	namespace Logic
	{
		class RunItemBase : public WritingElement
		{
		public:
			virtual const std::string toTxt() const = 0;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_RUN_ITEM_BASE_INCLUDE_H_