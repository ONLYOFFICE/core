#pragma once
#ifndef PPTX_LOGIC_UNICOLOR_INCLUDE_H_
#define PPTX_LOGIC_UNICOLOR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
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
			explicit UniColor(XmlUtils::CXmlNode& node);
			const UniColor& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void GetColorFrom(XmlUtils::CXmlNode& element);
			virtual bool is_init()const{return (Color.IsInit());};

			template<class T> AVSINLINE const bool	is() const	{ return Color.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return Color.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return Color.as<T>(); }
			
			virtual DWORD GetRGBA(DWORD RGBA = 0)const;
			virtual DWORD GetARGB(DWORD ARGB = 0)const;
			virtual DWORD GetBGRA(DWORD BGRA = 0)const;
			virtual DWORD GetABGR(DWORD ABGR = 0)const;

			virtual DWORD GetRGBColor(NSCommon::smart_ptr<PPTX::WrapperFile>& _oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& _oClrMap, DWORD ARGB = 0)
			{
				if (Color.is_init())
					return Color->GetRGBColor(_oTheme, _oClrMap, ARGB);
				return 0;
			}

			virtual CString toXML() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (Color.is_init())
					Color->toPPTY(pWriter);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (Color.is_init())
					Color->toXmlWriter(pWriter);
			}

		//public:
		public:
			smart_ptr<ColorBase> Color;
//hslClr (Hue, Saturation, Luminance Color Model)  §20.1.2.3.13 
//scrgbClr (RGB Color Model - Percentage Variant)  §20.1.2.3.30 
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					Color->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNICOLOR_INCLUDE_H