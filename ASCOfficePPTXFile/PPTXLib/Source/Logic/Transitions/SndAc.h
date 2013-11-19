#pragma once
#ifndef PPTX_SLIDES_SNDAC_INCLUDE_H_
#define PPTX_SLIDES_SNDAC_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "XML.h"
#include "StSnd.h"

namespace PPTX
{
	namespace Logic
	{

		class SndAc : public WrapperWritingElement
		{
		public:
			SndAc();
			virtual ~SndAc();
			explicit SndAc(const XML::XNode& node);
			const SndAc& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<StSnd> stSnd;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_SNDAC_INCLUDE_H_