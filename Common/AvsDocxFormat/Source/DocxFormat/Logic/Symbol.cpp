
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Symbol.h"


namespace OOX
{
	namespace Logic
	{

		Symbol::Symbol()
		{
		}


		Symbol::~Symbol()
		{
		}


		Symbol::Symbol(const std::string& text)
		{
			fromTxt(text);
		}
	

		Symbol::Symbol(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Symbol& Symbol::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const Symbol& Symbol::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void Symbol::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Font = element.attribute("font").value();
			Char = element.attribute("char").value();
		}


		void Symbol::fromTxt(const std::string& text)
		{
			//TODO
		}


		const XML::XNode Symbol::toXML() const
		{
		  return
		    XML::XElement(ns.w + "sym",
			   	          XML::XAttribute(ns.w + "font", Font) +
				          XML::XAttribute(ns.w + "char", Char)
			);
		}


		const std::string Symbol::toTxt() const
		{
			//TODO
			return "";
		}

	} // namespace Logic
} // namespace OOX