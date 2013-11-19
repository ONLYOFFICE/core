#pragma once
#ifndef ODT_CONTENT_VARIABLE_DECL_INCLUDE_H_
#define ODT_CONTENT_VARIABLE_DECL_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"


namespace Odt
{
	namespace Content
	{
		class VariableDecl : public WritingElement
		{
		public:
			VariableDecl();
			virtual ~VariableDecl();
			explicit VariableDecl(const XML::XNode& node);
			const VariableDecl& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>	Type;
			property<std::string>	Name;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_VARIABLE_DECL_INCLUDE_H_