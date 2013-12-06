
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "NodeContainer.h"
#include "./../XElement.h"
#include "./../XComment.h"
#include "./../XText.h"
#include "XList.h"
#include "XNamespaceContainer.h"
#include "Exception/not_implement.h"
#include "Utility.h"


namespace XML
{
	namespace Private
	{

		NodeContainer::NodeContainer()
			: Nodes(),
				Elements(Nodes),
				Texts(Nodes)
		{
		}


		NodeContainer::NodeContainer(const XNodeContainer& nodes)
			: Nodes(nodes),
				Elements(Nodes),
				Texts(Nodes)
		{
		}


		NodeContainer::~NodeContainer()
		{
		}


		NodeContainer::NodeContainer(const XElement& element)
			: Nodes(),
				Elements(Nodes),
				Texts(Nodes)
		{
			Elements.Add(element);
		}


		NodeContainer::NodeContainer(const XText& text)
			: Nodes(),
				Elements(Nodes),
				Texts(Nodes)
		{
			Texts.Add(text);
		}


		NodeContainer::NodeContainer(const XComment& comment)
			: Nodes(),
				Elements(Nodes),
				Texts(Nodes)
		{
			Nodes.Add(comment);
		}


		NodeContainer::NodeContainer(const XList& list)
			: Nodes(),
				Elements(Nodes),
				Texts(Nodes)
		{
			Nodes.merge(list.Nodes);
		}


		const bool NodeContainer::isElement() const
		{
			return false;
		}


		const bool NodeContainer::isText() const
		{
			return false;
		}


		const bool NodeContainer::isComment() const
		{
			return false;
		}


		void NodeContainer::fromSource(NSCommon::smart_ptr<XSingleSource> source, const XNamespaceContainer& ns, const XSpace& space)
		{
			if (space.isDefault())
				source->skipSpace();

			while (true)
			{
				if (source->get() == '<')
				{
					source->next();
					if (source->get() == '/')
						break;
					if (isComment(source))
						insertComment(source);	
					else
						Nodes.push_back(XElement(source, ns, space));
				}
				else
				{
					Nodes.push_back(XText(source->getString('<')));
				}
				if (space.isDefault())
					source->skipSpace();
			}
			source->next();
		}


		void NodeContainer::fromSource(NSCommon::smart_ptr<XWideSource> source, const XNamespaceContainer& ns, const XSpace& space)
		{
			if (space.isDefault())
				source->skipSpace();

			while (true)
			{
				if (source->get() == L'<')
				{
					source->next();
					if (source->get() == L'/')
						break;
					if (isComment(source))
						insertComment(source);	
					else
						Nodes.push_back(XElement(source, ns, space));
				}
				else
				{
					Nodes.push_back(XText(Encoding::unicode2utf8(source->getString(L'<'))));
				}
				if (space.isDefault())
					source->skipSpace();
			}
			source->next();
		}


		const std::string NodeContainer::ToString() const
		{
			return Nodes.ToString();
		}


		const std::wstring NodeContainer::ToWString() const
		{
			// TODO not implement
			throw not_implement();
		}


		void NodeContainer::SaveToStringList(std::list<std::string>& strList) const
		{
			Nodes.SaveToStringList(strList);
		}


		void NodeContainer::SaveToWStringList(std::list<std::wstring>& strList) const
		{
			Nodes.SaveToWStringList(strList);
		}


		void NodeContainer::Add(const XNode& node)
		{
			Nodes.Add(node);
		}


		void NodeContainer::Add(const XElement& element)
		{
			Elements.Add(element);
		}


		void NodeContainer::Add(const XText& text)
		{
			Texts.Add(text);
		}


		void NodeContainer::Add(const XComment& comment)
		{
			Nodes.Add(comment);
		}


		void NodeContainer::Add(const XList& list)
		{
			Nodes.merge(list.Nodes);
		}


		const bool NodeContainer::isComment(NSCommon::smart_ptr<XSingleSource> source)
		{
			if (source->get() == '!')
			{
				source->next();
				if (source->get() == '-')
				{
					source->next();
					if (source->get() == '-')
					{
						source->next();
						return true;															
					}
				}
			}
			return false;
		}


		void NodeContainer::insertComment(NSCommon::smart_ptr<XSingleSource> source)
		{
			std::string comment;
			while(true)
			{
				comment += source->getString('-');									
				source->next();
				if (source->get() == '-')
				{										
					source->next();
					if (source->get() == '>')
					{
						source->next();
						break;		
					}
					else
					{
						comment += "--";						
					}
				}
				else
				{
					comment += "-";
				}
			}
			Nodes.push_back(XComment(comment));
		}


		const bool NodeContainer::isComment(NSCommon::smart_ptr<XWideSource> source)
		{
			if (source->get() == L'!')
			{
				source->next();
				if (source->get() == L'-')
				{
					source->next();
					if (source->get() == L'-')
					{
						source->next();
						return true;															
					}
				}
			}
			return false;
		}


		void NodeContainer::insertComment(NSCommon::smart_ptr<XWideSource> source)
		{
			std::wstring comment;
			while(true)
			{
				comment += source->getString(L'-');									
				source->next();
				if (source->get() == L'-')
				{										
					source->next();
					if (source->get() == L'>')
					{
						source->next();
						break;		
					}
					else
					{
						comment += L"--";						
					}
				}
				else
				{
					comment += L"-";
				}
			}
			Nodes.push_back(XComment(Encoding::unicode2utf8(comment)));
		}

	} // namespace Private
} // namespace XML