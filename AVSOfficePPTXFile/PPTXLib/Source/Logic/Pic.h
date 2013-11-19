#pragma once
#ifndef PPTX_LOGIC_SLIDE_PIC_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_PIC_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "NvPicPr.h"
#include "Fills/BlipFill.h"
#include "SpPr.h"
#include "ShapeStyle.h"

namespace PPTX
{
	namespace Logic
	{

		class Pic : public WrapperWritingElement
		{
		public:
			Pic();
			virtual ~Pic();
			explicit Pic(const XML::XNode& node);
			const Pic& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual void GetRect(RECT& pRect)const;
			virtual std::wstring GetFullPicName()const;
			virtual std::wstring GetVideoLink()const;
			virtual std::wstring GetAudioLink()const;
			DWORD GetFill(UniFill& fill)const;
			DWORD GetLine(Ln& line)const;

		public:
			property<NvPicPr> nvPicPr;
			property<BlipFill> blipFill;
			property<SpPr> spPr;
			nullable_property<ShapeStyle> style;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_PIC_INCLUDE_H_
