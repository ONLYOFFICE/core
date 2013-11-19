#pragma once
#ifndef PPTX_LOGIC_PATHBASE_INCLUDE_H_
#define PPTX_LOGIC_PATHBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class PathBase : public WrapperWritingElement
		{
		public:
			PathBase();
			virtual ~PathBase();
			//virtual std::string GetODString() const =0;
			virtual void GetODString(std::list<std::string>& strList)const = 0;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PATHBASE_INCLUDE_H_