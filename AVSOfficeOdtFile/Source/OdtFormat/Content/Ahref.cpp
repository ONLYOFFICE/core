
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Ahref.h"


namespace Odt
{
	namespace Content
	{

		Ahref::Ahref()
		{
		}


		Ahref::~Ahref()
		{
		}


		Ahref::Ahref(const XML::XNode& node)
		{
			fromXML(node);
		}

		
		Ahref::Ahref(const std::string& text)
		{
			fromTxt(text);
		}


		const Ahref& Ahref::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const Ahref& Ahref::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void Ahref::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Type = element.attribute("type").value();
			Href = element.attribute("href").value();
			m_isInner =((*Href)[0] == L'#');

			for (XML::const_node_iterator i = element.Nodes.begin(); i != element.Nodes.end(); ++i)
			{
				ParagraphItem run(*i);
				Items->push_back(run);
			}
		}


		void Ahref::fromTxt(const std::string& text)
		{
			ParagraphItem item(text);
			Items->push_back(item);
		}


		const XML::XNode Ahref::toXML() const
		{
			return
				XML::XElement(ns.text + "a",
					XML::XAttribute(ns.xlink + "type", Type) +
					XML::XAttribute(ns.xlink + "href", Href) +
					XML::Write(Items)
				);
		}


		const std::string Ahref::toTxt() const
		{
			std::string text;
			BOOST_FOREACH(const ParagraphItem& item, *Items)
			{
				text += item.toTxt();
			}
			return text;
		}


		const bool Ahref::isInner() const
		{
			return m_isInner;
		}

	} // namespace Content
} // namespace Odt