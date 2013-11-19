#pragma once
#ifndef PPTX_LOGIC_UNIEFFECT_INCLUDE_H_
#define PPTX_LOGIC_UNIEFFECT_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{
		class UniEffect : public WrapperWritingElement
		{
		public:
			UniEffect();
			virtual ~UniEffect();			
			explicit UniEffect(const XML::XNode& node);
			const UniEffect& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void GetEffectFrom(const XML::XElement& element);
			virtual bool is_init()const{return (Effect != 0);};
			
			template<class T> const bool is() const { return (Effect == 0)?false:(typeid(*Effect) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*Effect);}
			template<class T> const T& as() const {return static_cast<const T&>(*Effect);}

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> Effect;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					Effect->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNIEFFECT_INCLUDE_H