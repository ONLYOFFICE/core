#pragma once
#ifndef PPTX_LOGIC_BULLET_INCLUDE_H_
#define PPTX_LOGIC_BULLET_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include <boost/shared_ptr.hpp>
#include "BuNone.h"
#include "BuChar.h"
#include "BuAutoNum.h"
#include "BuBlip.h"

namespace PPTX
{
	namespace Logic
	{
		class Bullet : public WrapperWritingElement
		{
		public:
			Bullet();
			virtual ~Bullet();			
			explicit Bullet(const XML::XNode& node);
			const Bullet& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void ReadBulletFrom(const XML::XElement& element);
			virtual bool is_init()const{return (m_Bullet != 0);};
			virtual bool has_bullet()const{return ((is_init()) && (!is<BuNone>()));};
			
			template<class T> const bool is() const { return (m_Bullet == 0)?false:(typeid(*m_Bullet) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*m_Bullet);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_Bullet);}

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> m_Bullet;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					m_Bullet->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BULLET_INCLUDE_H