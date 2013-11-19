#pragma once
#ifndef PPTX_PRESENTATION_SLDSZ_INCLUDE_H_
#define PPTX_PRESENTATION_SLDSZ_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/SlideSize.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class SldSz : public WrapperWritingElement
		{
		public:
			SldSz();
			virtual ~SldSz();
			explicit SldSz(const XML::XNode& node);
			const SldSz& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<int, setter::between<int, 914400, 51206400> > cx;
			property<int, setter::between<int, 914400, 51206400> > cy;
			nullable_property<std::string, Limit::SlideSize> type;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_SLDSZ_INCLUDE_H_