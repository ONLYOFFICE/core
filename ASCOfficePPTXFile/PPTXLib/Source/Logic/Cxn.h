#pragma once
#ifndef PPTX_LOGIC_CXN_INCLUDE_H_
#define PPTX_LOGIC_CXN_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class Cxn : public WrapperWritingElement
		{
		public:
			Cxn();
			virtual ~Cxn();
			explicit Cxn(const XML::XNode& node);
			const Cxn& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> x;
			property<std::string> y;
			property<std::string> ang;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			//std::string GetODString()const;
			void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CXN_INCLUDE_H_