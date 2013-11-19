#pragma once
#ifndef PPTX_LOGIC_GS_INCLUDE_H_
#define PPTX_LOGIC_GS_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class Gs : public WrapperWritingElement
		{
		public:
			Gs();
			virtual ~Gs();			
			explicit Gs(const XML::XNode& node);
			const Gs& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			UniColor color;
			property<int> pos;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GS_INCLUDE_H