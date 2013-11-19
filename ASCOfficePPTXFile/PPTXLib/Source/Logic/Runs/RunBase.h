#pragma once
#ifndef PPTX_LOGIC_RUNBASE_INCLUDE_H_
#define PPTX_LOGIC_RUNBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"

namespace PPTX
{
	namespace Logic
	{
		class RunBase : public WrapperWritingElement
		{
		public:
			RunBase();
			virtual ~RunBase();
			virtual std::string GetText()const =0;
			virtual std::wstring GetWText()const =0;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RUNBASE_INCLUDE_H