#pragma once
#ifndef PPTX_LOGIC_RECT_INCLUDE_H_
#define PPTX_LOGIC_RECT_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class Rect : public WrapperWritingElement
		{
		public:
			Rect();
			virtual ~Rect();
			explicit Rect(const XML::XNode& node);
			const Rect& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> t;
			nullable_property<std::string> b;
			nullable_property<std::string> l;
			nullable_property<std::string> r;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			//std::string GetODString()const;
			void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RECT_INCLUDE_H_