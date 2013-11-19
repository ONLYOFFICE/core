
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "SimpleText.h"


namespace Odt
{
	namespace Content
	{

		SimpleText::SimpleText()
		{
		}


		SimpleText::~SimpleText()
		{
		}


		SimpleText::SimpleText(const XML::XNode& node)
		{
			fromXML(node);
		}


		SimpleText::SimpleText(const std::string& text)
		{
			fromTxt(text);
		}


		const SimpleText& SimpleText::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const SimpleText& SimpleText::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void SimpleText::fromXML(const XML::XNode& node)
		{
			const XML::XText text(node);
			Text = text.Value;
		}


		void SimpleText::fromTxt(const std::string& text)
		{
			Text = text;
		}
		
		
		const XML::XNode SimpleText::toXML() const
		{
			return XML::XText(Text);
		}


		const std::string SimpleText::toTxt() const
		{
			return Text;
		}

	} // namespace Content
} // namespace Odt