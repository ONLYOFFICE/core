#pragma once
#ifndef PPTX_LOGIC_AHBASE_INCLUDE_H_
#define PPTX_LOGIC_AHBASE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Ah.h"
#include "property.h"
#include <boost/shared_ptr.hpp>

namespace PPTX
{
	namespace Logic
	{
		class AhBase : public WrapperWritingElement
		{
		public:
			AhBase();
			virtual ~AhBase();			
			explicit AhBase(const XML::XNode& node);
			const AhBase& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void GetAdjustHandleFrom(const XML::XElement& element);
			virtual bool is_init()const{return (ah != 0);};
			
			template<class T> const bool is() const { return (ah == 0)?false:(typeid(*ah) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*ah);}
			template<class T> const T& as() const {return static_cast<const T&>(*ah);}

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<Ah> ah;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					ah->SetParentPointer(pParent);
			};
			//std::string GetODString()const
			void GetODString(std::list<std::string>& strList)const
			{
				//return ah->GetODString();
				ah->GetODString(strList);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_AHBASE_INCLUDE_H_