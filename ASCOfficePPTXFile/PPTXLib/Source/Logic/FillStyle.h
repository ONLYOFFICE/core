#pragma once
#ifndef PPTX_LOGIC_FILLSTYLE_INCLUDE_H_
#define PPTX_LOGIC_FILLSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "UniFill.h"

namespace PPTX
{
	namespace Logic
	{
		class FillStyle: public WrapperWritingElement
		{
		public:
			FillStyle();
			virtual ~FillStyle();			
			explicit FillStyle(const XML::XNode& node);
			const FillStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			UniFill Fill;
		protected:
			virtual void FillParentPointersForChilds();
		};

	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FILLSTYLE_INCLUDE_H