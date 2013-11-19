#pragma once
#ifndef PPTX_LOGIC_GLOW_INCLUDE_H_
#define PPTX_LOGIC_GLOW_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class Glow : public WrapperWritingElement
		{
		public:
			Glow();
			virtual ~Glow();
			explicit Glow(const XML::XNode& node);
			const Glow& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniColor Color;

			nullable_property<int, setter::only_positive<int> > rad;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GLOW_INCLUDE_H_