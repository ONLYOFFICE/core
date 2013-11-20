
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Break.h"


namespace OOX
{
	namespace Logic
	{

		Break::Break()
		{
		}


		Break::~Break()
		{
		}


		Break::Break(const std::string& text)
		{
			fromTxt(text);
		}


		Break::Break(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Break& Break::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const Break& Break::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void Break::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Type = element.attribute("type").value();
			Clear = element.attribute("clear").value();
		}


		void Break::fromTxt(const std::string& text)
		{
			Type = text;
		}


		const XML::XNode Break::toXML() const
		{
			return 
				XML::XElement(ns.w + "br", 
					XML::XAttribute(ns.w + "type", Type) + 
					XML::XAttribute(ns.w + "clear", Clear)
				);
		}


		const std::string Break::toTxt() const
		{
			if (!Type.is_init())
				return "\n";
			return "";
		}

	} // namespace Logic
} // namespace OOX