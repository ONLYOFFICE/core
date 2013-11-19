#pragma once
#ifndef PPTX_LOGIC_CXNSP_INCLUDE_H_
#define PPTX_LOGIC_CXNSP_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "NvCxnSpPr.h"
#include "SpPr.h"
#include "ShapeStyle.h"


namespace PPTX
{
	namespace Logic
	{
		class CxnSp : public WrapperWritingElement
		{
		public:
			CxnSp();
			virtual ~CxnSp();			
			explicit CxnSp(const XML::XNode& node);
			const CxnSp& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual void GetRect(RECT& pRect)const;

			DWORD GetLine(Ln& line)const;
			DWORD GetFill(UniFill& fill)const;
		public:
			property<NvCxnSpPr> nvCxnSpPr;
			property<SpPr> spPr;
			nullable_property<ShapeStyle> style;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CXNSP_INCLUDE_H