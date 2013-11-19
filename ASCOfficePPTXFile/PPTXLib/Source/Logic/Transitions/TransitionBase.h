#pragma once
#ifndef PPTX_LOGIC_TRANSITIONBASE_INCLUDE_H_
#define PPTX_LOGIC_TRANSITIONBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{
		class TransitionBase : public WrapperWritingElement
		{
		public:
			TransitionBase();
			virtual ~TransitionBase();			
			explicit TransitionBase(const XML::XNode& node);
			const TransitionBase& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void GetTransitionTypeFrom(const XML::XElement& element);
			virtual bool is_init()const{return (base != 0);};
			
			template<class T> const bool is() const { return (base == 0)?false:(typeid(*base) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*base);}
			template<class T> const T& as() const {return static_cast<const T&>(*base);}

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> base;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					base->SetParentPointer(pParent);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TRANSITIONBASE_INCLUDE_H