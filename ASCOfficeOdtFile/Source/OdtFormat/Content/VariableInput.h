#pragma once
#ifndef ODT_CONTENT_VARIABLE_INPUT_INCLUDE_H_
#define ODT_CONTENT_VARIBALE_INPUT_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include <string>


namespace Odt
{
	namespace Content
	{
		class VariableInput : public ParagraphItemBase
		{
		public:
			VariableInput();
			virtual ~VariableInput();
			explicit VariableInput(const XML::XNode& node);
			explicit VariableInput(const std::string& text);
			const VariableInput& operator =(const XML::XNode& node);
			const VariableInput& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			property<std::string>	Name;
			property<std::string>	Description;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_VARIBALE_INPUT_INCLUDE_H_