#pragma once
#ifndef PPTX_LOGIC_GD_INCLUDE_H_
#define PPTX_LOGIC_GD_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include <list>

namespace PPTX
{
	namespace Logic
	{

		class Gd : public WrapperWritingElement
		{
		public:
			Gd();
			virtual ~Gd();
			explicit Gd(const XML::XNode& node);
			const Gd& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> name;
			nullable_property<std::string> fmla;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			/*std::string */void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GD_INCLUDE_H_