#pragma once
#ifndef OOX_LOGIC_COMPLEX_INCLUDE_H_
#define OOX_LOGIC_COMPLEX_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "./../Limit/ComplexExt.h"


// TODO попробовать перенести в вызываемый класс
namespace OOX
{
	namespace Logic
	{
		class Complex : public WritingElement
		{
		public:
			Complex();
			virtual ~Complex();			
			explicit Complex(const XML::XNode& node);
			const Complex& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::ComplexExt>	Ext;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_COMPLEX_INCLUDE_H_