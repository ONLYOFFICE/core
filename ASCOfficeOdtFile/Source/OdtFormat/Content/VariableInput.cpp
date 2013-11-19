
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "VariableInput.h"


namespace Odt
{
	namespace Content
	{

		VariableInput::VariableInput()
		{
		}


		VariableInput::~VariableInput()
		{
		}


		VariableInput::VariableInput(const XML::XNode& node)
		{
			fromXML(node);
		}


		VariableInput::VariableInput(const std::string& text)
		{
			fromTxt(text);
		}


		const VariableInput& VariableInput::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const VariableInput& VariableInput::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void VariableInput::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name = element.attribute("name").value();
			Description = element.attribute("description").value();
		}


		void VariableInput::fromTxt(const std::string& text)
		{
			Name = text;
			Description = text;
		}


		const XML::XNode VariableInput::toXML() const
		{
			return
				XML::XElement(ns.text + "variable-input",
					XML::XAttribute(ns.text + "name", Name) +
					XML::XAttribute(ns.text + "description", Description)
				);
		}


		const std::string VariableInput::toTxt() const
		{
			return Name;
		}

	} // namespace Content
} // namespace Odt