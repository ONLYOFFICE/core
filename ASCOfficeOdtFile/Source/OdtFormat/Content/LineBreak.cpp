
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "LineBreak.h"


namespace Odt
{
	namespace Content
	{

		LineBreak::LineBreak()
		{
		}


		LineBreak::~LineBreak()
		{
		}


		LineBreak::LineBreak(const XML::XNode& node)
		{
			fromXML(node);
		}


		LineBreak::LineBreak(const std::string& text)
		{
			fromTxt(text);
		}


		const LineBreak& LineBreak::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const LineBreak& LineBreak::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void LineBreak::fromXML(const XML::XNode& node)
		{
		}


		void LineBreak::fromTxt(const std::string& text)
		{
		}


		const XML::XNode LineBreak::toXML() const
		{
			return XML::XElement(ns.text + "line-break");
		}


		const std::string LineBreak::toTxt() const
		{
			return "\n";
		}

	} // namespace Content
} // namespace Odt