#pragma once
#ifndef PPTX_LOGIC_GRAYSCL_INCLUDE_H_
#define PPTX_LOGIC_GRAYSCL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class Grayscl : public WrapperWritingElement
		{
		public:
			Grayscl();
			virtual ~Grayscl();
			explicit Grayscl(const XML::XNode& node);
			const Grayscl& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GRAYSCL_INCLUDE_H_