#pragma once
#ifndef PPTX_LOGIC_RUNBASE_INCLUDE_H_
#define PPTX_LOGIC_RUNBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class RunBase : public WrapperWritingElement
		{
		public:
			RunBase(){}
			virtual ~RunBase(){}
			virtual CString GetText()const =0;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RUNBASE_INCLUDE_H