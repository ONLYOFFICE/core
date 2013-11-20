
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Tab.h"


namespace OOX
{
	namespace Logic
	{

		Tab::Tab()
		{
		}


		Tab::~Tab()
		{
		}


		Tab::Tab(const std::string& text)
		{
			fromTxt(text);
		}


		Tab::Tab(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Tab& Tab::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const Tab& Tab::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void Tab::fromXML(const XML::XNode& node)
		{
		}


		void Tab::fromTxt(const std::string& text)
		{
		}


		const XML::XNode Tab::toXML() const
		{
			return XML::XElement(ns.w + "tab");
		}


		const std::string Tab::toTxt() const
		{
			return "\t";
		}

	} // namespace Logic
} // namespace OOX