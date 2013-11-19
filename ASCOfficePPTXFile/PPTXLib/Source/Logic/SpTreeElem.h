#pragma once
#ifndef PPTX_SLIDES_SLIDE_SHAPETREEELEM_INCLUDE_H_
#define PPTX_SLIDES_SLIDE_SHAPETREEELEM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class SpTreeElem : public WrapperWritingElement
		{
		public:
			SpTreeElem();
			virtual ~SpTreeElem();
			explicit SpTreeElem(const XML::XNode& node);
			const SpTreeElem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual bool is_init() const {return (m_elem != 0);};

			template<class T> const bool is() const { return (m_elem == 0)?false:(typeid(*m_elem) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*m_elem);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_elem);}
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> m_elem;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent) {if(is_init()) m_elem->SetParentPointer(pParent);};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_SLIDE_SHAPETREEELEM_INCLUDE_H_
