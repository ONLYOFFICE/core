
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Span.h"
#include <boost/foreach.hpp>
#include "LineBreak.h"
#include "SimpleText.h"
#include "Space.h"
#include "Tab.h"


namespace Odt
{
	namespace Content
	{

		Span::Span()
		{
		}


		Span::~Span()
		{
		}


		Span::Span(const XML::XNode& node)
		{
			fromXML(node);
		}


		Span::Span(const std::string& text)
		{
			fromTxt(text);
		}


		Span::Span(const std::string& style, const std::string& text)
			: Style(style)	
		{
			fromTxt(text);
		}


		const Span& Span::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const Span& Span::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void Span::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Style = element.attribute("style-name").value();

			for (XML::const_node_iterator i = element.Nodes.begin(); i != element.Nodes.end(); ++i)
			{
				ParagraphItem run(*i);
				Items->push_back(run);
			}
		}


		void Span::fromTxt(const std::string& text)
		{
			fromTxt("", text);
		}


		void Span::fromTxt(const std::string& style, std::string text)
		{
			if (style != "")
				Style = style;

			if (text.empty())
				return;

			// Убираем ведущии переносы строки
			while (text[0] == '\n')
			{
				add<LineBreak>();
				text = text.substr(1);
				if (text.empty())
					return;
			}

			std::string word;
			size_t space_count = 0;
			enum {Tab, Space, ManySpace, Symbol, Start} symbol_type = Start;

			for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
			{
				if (*it == '\t')
				{
					if (symbol_type == Tab)
					{
					}
					else if (symbol_type == Space)
					{
						add(SimpleText(word));
						add<Odt::Content::Space>();
					}
					else if (symbol_type == ManySpace)
					{
						add(Odt::Content::Space(space_count));
					}
					else if (symbol_type == Symbol)
					{
						add(SimpleText(word));
					}
					else if (symbol_type == Start)
					{
					}
					add<Odt::Content::Tab>();
					word.clear();
					space_count = 0;
					symbol_type = Tab;
				} // *it == tab
				else if (*it == ' ')
				{
					if (symbol_type == Tab)
					{
						symbol_type = Space;
					}
					else if (symbol_type == Space)
					{
						add(SimpleText(word));
						word.clear();
						symbol_type = ManySpace;
					}
					else if (symbol_type == ManySpace)
					{
						symbol_type = ManySpace;
					}
					else if (symbol_type == Symbol)
					{
						symbol_type = Space;
					}
					else if (symbol_type == Start)
					{
						symbol_type = ManySpace;
					}
					++space_count;
				} // *it == space
				else
				{
					if (symbol_type == Tab)
					{
						word += *it;
					}
					else if (symbol_type == Space)
					{
						word += ' ';
						word += *it;
					}
					else if (symbol_type == ManySpace)
					{
						add(Odt::Content::Space(space_count));
						word += *it;
					}
					else if (symbol_type == Symbol)
					{
						word += *it;
					}
					else if (symbol_type == Start)
					{
						word += *it;
					}
					space_count = 0;
					symbol_type = Symbol;
				} // it == symbol
			}


			if (symbol_type == Tab)
			{
			}
			else if (symbol_type == Space)
			{
				word += ' ';
				add(SimpleText(word));
			}
			else if (symbol_type == ManySpace)
			{
				add(Odt::Content::Space(space_count));
			}
			else if (symbol_type == Symbol)
			{
				add(SimpleText(word));
			}
		}


		const XML::XNode Span::toXML() const
		{
			return
				XML::XElement(ns.text + "span",
					XML::XAttribute(ns.text + "style-name", Style) + 
					XML::Write(Items)
				);
		}


		const std::string Span::toTxt() const
		{
			std::string text;
			BOOST_FOREACH(const ParagraphItem& item, *Items)
			{
				text += item.toTxt();
			}
			return text;
		}

	} // namespace Content
} // namespace Odt