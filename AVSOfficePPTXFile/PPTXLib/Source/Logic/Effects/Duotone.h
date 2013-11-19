#pragma once
#ifndef PPTX_LOGIC_DUOTONE_INCLUDE_H_
#define PPTX_LOGIC_DUOTONE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class Duotone : public WrapperWritingElement
		{
		public:
			Duotone();
			virtual ~Duotone();
			explicit Duotone(const XML::XNode& node);
			const Duotone& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::list<UniColor> > Colors;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_DUOTONE_INCLUDE_H_