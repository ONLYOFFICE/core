#pragma once
#ifndef PPTX_LOGIC_UNIFILL_INCLUDE_H_
#define PPTX_LOGIC_UNIFILL_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{
		class UniFill : public WrapperWritingElement
		{
		public:
			UniFill();
			virtual ~UniFill();			
			explicit UniFill(const XML::XNode& node);
			const UniFill& operator =(const XML::XNode& node);
		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void GetFillFrom(const XML::XElement& element);
			virtual bool is_init()const{return (Fill != 0);};
			
			template<class T> const bool is() const { return (Fill == 0)?false:(typeid(*Fill) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*Fill);}
			template<class T> const T& as() const {return static_cast<const T&>(*Fill);}

			const UniFill& operator+=(const UniFill& fill);
			void Merge(UniFill& fill)const;

			virtual const XML::XNode toXML() const;
		private:
			boost::shared_ptr<WrapperWritingElement> Fill;
			enum Type {notInit, noFill, solidFill, gradFill, blipFill, pattFill, grpFill};
			property<Type> m_type;
//grpFill (Group Fill)  §20.1.8.35 
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNIFILL_INCLUDE_H