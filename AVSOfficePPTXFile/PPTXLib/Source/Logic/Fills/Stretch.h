#pragma once
#ifndef PPTX_LOGIC_STRETCH_INCLUDE_H_
#define PPTX_LOGIC_STRETCH_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Rect.h"

namespace PPTX
{
	namespace Logic
	{

		class Stretch : public WrapperWritingElement
		{
		public:
			Stretch();
			virtual ~Stretch();
			explicit Stretch(const XML::XNode& node);
			const Stretch& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<Rect> fillRect;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_STRETCH_INCLUDE_H_