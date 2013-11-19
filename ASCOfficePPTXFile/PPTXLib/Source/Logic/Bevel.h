#pragma once
#ifndef PPTX_LOGIC_BEVEL_INCLUDE_H_
#define PPTX_LOGIC_BEVEL_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/BevelType.h"

namespace PPTX
{
	namespace Logic
	{

		class Bevel : public WrapperWritingElement
		{
		public:
			Bevel();
			virtual ~Bevel();
			explicit Bevel(const XML::XNode& node);
			const Bevel& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<int, setter::only_positive<int> > w;
			nullable_property<int, setter::only_positive<int> > h;
			nullable_property<std::string, Limit::BevelType> prst;
		//private:
		public:
			property<std::string> m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BEVEL_INCLUDE_H_