#pragma once
#ifndef ODT_CONTENT_EQUATION_INCLUDE_H_
#define ODT_CONTENT_EQUATION_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <string>


namespace Odt
{
	namespace Content
	{
		class Equation : public WritingElement
		{
		public:
			Equation();
			virtual ~Equation();
			explicit Equation(const XML::XNode& node);
			const Equation& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>		Name;
			property<std::string>		Formula;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_EQUATION_INCLUDE_H_