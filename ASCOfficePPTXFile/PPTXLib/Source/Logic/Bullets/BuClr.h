#pragma once
#ifndef PPTX_LOGIC_BUCLR_INCLUDE_H_
#define PPTX_LOGIC_BUCLR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class BuClr : public WrapperWritingElement
		{
		public:
			BuClr();
			virtual ~BuClr();			
			explicit BuClr(const XML::XNode& node);
			const BuClr& operator =(const XML::XNode& node);

			virtual DWORD GetRGBA()const{return Color.GetRGBA();};
			virtual DWORD GetARGB()const{return Color.GetARGB();};
			virtual DWORD GetBGRA()const{return Color.GetBGRA();};
			virtual DWORD GetABGR()const{return Color.GetABGR();};

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			UniColor Color;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUCLR_INCLUDE_H