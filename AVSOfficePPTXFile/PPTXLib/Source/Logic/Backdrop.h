#pragma once
#ifndef PPTX_LOGIC_BACKDROP_INCLUDE_H_
#define PPTX_LOGIC_BACKDROP_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class Backdrop : public WrapperWritingElement
		{
		public:
			Backdrop();
			virtual ~Backdrop();
			explicit Backdrop(const XML::XNode& node);
			const Backdrop& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int> anchorX;
			property<int> anchorY;
			property<int> anchorZ;

			property<int> normX;
			property<int> normY;
			property<int> normZ;

			property<int> upX;
			property<int> upY;
			property<int> upZ;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BACKDROP_INCLUDE_H_