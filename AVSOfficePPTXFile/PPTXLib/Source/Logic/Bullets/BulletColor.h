#pragma once
#ifndef PPTX_LOGIC_BULLETCOLOR_INCLUDE_H_
#define PPTX_LOGIC_BULLETCOLOR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include <boost/shared_ptr.hpp>
#include "BuClrTx.h"
#include "BuClr.h"

namespace PPTX
{
	namespace Logic
	{
		class BulletColor : public WrapperWritingElement
		{
		public:
			BulletColor();
			virtual ~BulletColor();			
			explicit BulletColor(const XML::XNode& node);
			const BulletColor& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void ReadBulletColorFrom(const XML::XElement& element);
			virtual bool is_init()const{return (m_Color != 0);};
			virtual bool has_spec_color()const{return is<BuClr>();};
			
			template<class T> const bool is() const { return (m_Color == 0)?false:(typeid(*m_Color) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*m_Color);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_Color);}

			virtual DWORD GetRGBA()const;
			virtual DWORD GetARGB()const;
			virtual DWORD GetBGRA()const;
			virtual DWORD GetABGR()const;

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> m_Color;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					m_Color->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BULLETCOLOR_INCLUDE_H