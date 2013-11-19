#pragma once
#ifndef PPTX_LOGIC_ROT_INCLUDE_H_
#define PPTX_LOGIC_ROT_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class Rot : public WrapperWritingElement
		{
		public:
			Rot();
			virtual ~Rot();
			explicit Rot(const XML::XNode& node);
			const Rot& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int, setter::between<int, 0, 21600000> > lat;
			property<int, setter::between<int, 0, 21600000> > lon;
			property<int, setter::between<int, 0, 21600000> > rev;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ROT_INCLUDE_H_