#pragma once
#ifndef PPTX_LOGIC_RUNELEM_INCLUDE_H_
#define PPTX_LOGIC_RUNELEM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Runs/RunBase.h"
#include "property.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{
		class RunElem : public WrapperWritingElement
		{
		public:
			RunElem();
			virtual ~RunElem();			
			explicit RunElem(const XML::XNode& node);
			const RunElem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual bool is_init() const {return (Elem != 0);};
			virtual std::string GetText()const{return Elem->GetText();};
			virtual std::wstring GetWText()const{return Elem->GetWText();};

			template<class T> const bool is() const { return (Elem == 0)?false:(typeid(*Elem) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*Elem);}
			template<class T> const T& as() const {return static_cast<const T&>(*Elem);}

		//public:
		private:
			boost::shared_ptr<RunBase> Elem;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent){if(is_init()) Elem->SetParentPointer(pParent);};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RUNELEM_INCLUDE_H