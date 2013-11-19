#pragma once
#ifndef PPTX_LOGIC_UNIMEDIA_INCLUDE_H_
#define PPTX_LOGIC_UNIMEDIA_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{
		class UniMedia : public WrapperWritingElement
		{
		public:
			UniMedia();
			virtual ~UniMedia();			
			explicit UniMedia(const XML::XNode& node);
			const UniMedia& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void GetMediaFrom(const XML::XElement& element);
			virtual bool is_init()const{return (Media != 0);};
			
			template<class T> const bool is() const { return (Media == 0)?false:(typeid(*Media) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*Media);}
			template<class T> const T& as() const {return static_cast<const T&>(*Media);}

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> Media;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					Media->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNIMEDIA_INCLUDE_H