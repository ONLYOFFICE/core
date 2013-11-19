#pragma once
#ifndef PPTX_LOGIC_EFFECTPROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_EFFECTPROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{
		class EffectProperties : public WrapperWritingElement
		{
		public:
			EffectProperties();
			virtual ~EffectProperties();			
			explicit EffectProperties(const XML::XNode& node);
			const EffectProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void GetEffectListFrom(const XML::XElement& element);
			virtual bool is_init() const {return (List != 0);};

			template<class T> const bool is() const { return (List == 0)?false:(typeid(*List) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*List);}
			template<class T> const T& as() const {return static_cast<const T&>(*List);}

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<WrapperWritingElement> List;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					List->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTPROPERTIES_INCLUDE_H