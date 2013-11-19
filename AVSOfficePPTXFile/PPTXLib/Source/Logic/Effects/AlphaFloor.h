#pragma once
#ifndef PPTX_LOGIC_ALPHAFLOOR_INCLUDE_H_
#define PPTX_LOGIC_ALPHAFLOOR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaFloor : public WrapperWritingElement
		{
		public:
			AlphaFloor();
			virtual ~AlphaFloor();
			explicit AlphaFloor(const XML::XNode& node);
			const AlphaFloor& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHAFLOOR_INCLUDE_H_