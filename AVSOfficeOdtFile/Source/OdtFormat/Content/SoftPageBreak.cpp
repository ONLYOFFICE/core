
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "SoftPageBreak.h"


namespace Odt
{
	namespace Content
	{

		SoftPageBreak::SoftPageBreak()
		{
		}


		SoftPageBreak::~SoftPageBreak()
		{
		}


		SoftPageBreak::SoftPageBreak(const XML::XNode& node)
		{
			fromXML(node);
		}


		SoftPageBreak::SoftPageBreak(const std::string& text)
		{
			fromTxt(text);
		}


		const SoftPageBreak& SoftPageBreak::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const SoftPageBreak& SoftPageBreak::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void SoftPageBreak::fromXML(const XML::XNode& node)
		{
		}


		void SoftPageBreak::fromTxt(const std::string& text)
		{
		}


		const XML::XNode SoftPageBreak::toXML() const
		{
			return XML::XElement(ns.text + "soft-page-break");
		}


		const std::string SoftPageBreak::toTxt() const
		{
			return "";
		}

	} // namespace Content
} // namespace Odt