#pragma once
#ifndef PPTX_LOGIC_UNICOLOR_INCLUDE_H_
#define PPTX_LOGIC_UNICOLOR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <boost/shared_ptr.hpp>
#include "Colors/ColorBase.h"

namespace PPTX
{
	namespace Logic
	{
		class UniColor : public WrapperWritingElement
		{
		public:
			UniColor();
			virtual ~UniColor();			
			explicit UniColor(const XML::XNode& node);
			const UniColor& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void GetColorFrom(const XML::XElement& element);
			virtual bool is_init()const{return (Color != 0);};
			
			template<class T> const bool is() const { return (Color == 0)?false:(typeid(*Color) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*Color);}
			template<class T> const T& as() const {return static_cast<const T&>(*Color);}

			virtual DWORD GetRGBA(DWORD RGBA = 0)const;
			virtual DWORD GetARGB(DWORD ARGB = 0)const;
			virtual DWORD GetBGRA(DWORD BGRA = 0)const;
			virtual DWORD GetABGR(DWORD ABGR = 0)const;

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<ColorBase> Color;
//hslClr (Hue, Saturation, Luminance Color Model)  §20.1.2.3.13 
//scrgbClr (RGB Color Model - Percentage Variant)  §20.1.2.3.30 
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					Color->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNICOLOR_INCLUDE_H