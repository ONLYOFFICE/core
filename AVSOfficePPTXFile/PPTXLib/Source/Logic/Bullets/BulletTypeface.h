#pragma once
#ifndef PPTX_LOGIC_BULLETTYPEFACE_INCLUDE_H_
#define PPTX_LOGIC_BULLETTYPEFACE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include <boost/shared_ptr.hpp>
#include "./../TextFont.h"
#include "BuFontTx.h"

namespace PPTX
{
	namespace Logic
	{
		class BulletTypeface : public WrapperWritingElement
		{
		public:
			BulletTypeface();
			virtual ~BulletTypeface();			
			explicit BulletTypeface(const XML::XNode& node);
			const BulletTypeface& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void ReadBulletTypefaceFrom(const XML::XElement& element);
			virtual bool is_init()const{return (m_Typeface != 0);};
			virtual bool has_spec_typeface()const{return is<TextFont>();};
			
			template<class T> const bool is() const { return (m_Typeface == 0)?false:(typeid(*m_Typeface) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*m_Typeface);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_Typeface);}

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> m_Typeface;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					m_Typeface->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BULLETTYPEFACE_INCLUDE_H