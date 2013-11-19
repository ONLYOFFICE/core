#pragma once
#ifndef PPTX_LOGIC_UNIPATH2D_INCLUDE_H_
#define PPTX_LOGIC_UNIPATH2D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <boost/shared_ptr.hpp>
#include "Path2D/PathBase.h"

namespace PPTX
{
	namespace Logic
	{
		class UniPath2D : public WrapperWritingElement
		{
		public:
			UniPath2D();
			virtual ~UniPath2D();			
			explicit UniPath2D(const XML::XNode& node);
			const UniPath2D& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void GetPath2DFrom(const XML::XElement& element);
			virtual bool is_init()const{return (Path2D != 0);};
			
			template<class T> const bool is() const { return (Path2D == 0)?false:(typeid(*Path2D) == typeid(T));}
			template<class T> T& as() {return static_cast<T&>(*Path2D);}
			template<class T> const T& as() const {return static_cast<const T&>(*Path2D);}

			virtual const XML::XNode toXML() const;
		//public:
		private:
			boost::shared_ptr<PathBase> Path2D;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement& pParent)
			{
				if(is_init())
					Path2D->SetParentPointer(pParent);
			};

			//std::string GetODString()const
			void GetODString(std::list<std::string>& strList)const
			{
				//return Path2D->GetODString();
				Path2D->GetODString(strList);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNIPATH2D_INCLUDE_H