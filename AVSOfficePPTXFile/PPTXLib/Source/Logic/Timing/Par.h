#pragma once
#ifndef PPTX_LOGIC_PAR_INCLUDE_H_
#define PPTX_LOGIC_PAR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CTn.h"

namespace PPTX
{
	namespace Logic
	{
		class Par : public WrapperWritingElement
		{
		public:
			Par();
			virtual ~Par();			
			explicit Par(const XML::XNode& node);
			const Par& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<CTn> cTn;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PAR_INCLUDE_H