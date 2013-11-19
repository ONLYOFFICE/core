#pragma once
#ifndef PPTX_LOGIC_TEXTFIT_INCLUDE_H_
#define PPTX_LOGIC_TEXTFIT_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"

namespace PPTX
{
	namespace Logic
	{
		class TextFit : public WrapperWritingElement
		{
		public:
			enum eFit {FitEmpty, FitNo, FitSpAuto, FitNormAuto};

			TextFit();
			virtual ~TextFit();			
			explicit TextFit(const XML::XNode& node);
			const TextFit& operator =(const XML::XNode& node);
			void GetTextFitFrom(const XML::XElement& element);
			virtual bool is_init(){return (type != FitEmpty);};

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(TextFit& fit)const;

		public:
			property<eFit> type;
			nullable_property<int> fontScale;
			nullable_property<int> lnSpcReduction;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TEXTFIT_INCLUDE_H