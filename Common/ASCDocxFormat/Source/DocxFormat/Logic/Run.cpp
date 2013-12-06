
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Run.h"
#include "./../FormatError.h"
#include "Utility.h"
#include <utility>
#include "Text.h"
#include "Symbol.h"

namespace OOX
{
	namespace Logic
	{

		Run::Run()
		{
		}


		Run::~Run()
		{
		}


		Run::Run(const XML::XNode& node)
		{
			fromXML(node);
		}


		Run::Run(const std::string& text)
		{
			fromTxt(text);
		}


		Run::Run(const std::string& text, const RunProperty& runProperty)
		{
			fromTxt(text);
			Property = runProperty;
		}


		const Run& Run::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const Run& Run::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void Run::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Property = element.element("rPr");

			for (XML::const_node_iterator i = element.Nodes.begin(); i != element.Nodes.end(); ++i)
			{
				if (i->isElement())
				{
					const XML::XElement element(*i);
					
					if (element.XName == "AlternateContent")
					{
						for (XML::const_element_iterator j = element.Elements.begin(); j != element.Elements.end(); ++j)
						{
							const XML::XElement oElement(*j);
							if (oElement.XName == "Fallback")
							{
								for (XML::const_element_iterator j = element.Elements.begin(); j != element.Elements.end(); ++j)
									fromXML (*j);

								break;
							}
						}
					}
					
					if (element.XName == "t" ||element.XName == "tab" || element.XName == "br" || 
							element.XName == "drawing" || element.XName == "pict" || element.XName == "footnoteRef" ||
							element.XName == "endnoteRef" || element.XName == "continuationSeparator" || 
							element.XName == "separator" || element.XName == "footnoteReference" || 
							element.XName == "endnoteReference" || element.XName == "object" ||
							element.XName == "fldChar" || element.XName == "instrText" || element.XName == "sym")
						{
							Items->push_back(RunItem(*i));
						}
				}
				else if (i->isText())
				{
					XML::XText text(*i);
					Items->push_back(RunItem(text.Value));
				}
			}
		}


		void Run::fromTxt(const std::string& text)
		{
			add(text);
		}


		const XML::XNode Run::toXML() const
		{
		return XML::XElement();
		}


		const std::string Run::toTxt() const
		{
			std::string text;

			const std::vector<RunItem>& runs = Items.get();

			for (std::vector<RunItem>::const_iterator iter = runs.begin(); iter != runs.end(); ++iter)
			{
				text += (*iter).toTxt();
			}

			return text;
		}


		const bool Run::empty() const
		{
			return Items->empty();
		}


		void Run::clear()
		{
			Items->clear();
		}


		void Run::add(RunItemBase* item)
		{
			Items->push_back(RunItem(item));
		}


		void Run::add(const std::string& text)
		{
			Items->push_back(RunItem(new Text(text)));
		}

	} // namespace Logic
} // namespace OOX