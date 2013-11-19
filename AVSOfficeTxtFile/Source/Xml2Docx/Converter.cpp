#include "Converter.h"
#include <boost/foreach.hpp>
#include "UniversalUnit.h"


namespace Xml2Docx
{
	static const int dx = 280;

	static const std::string SSS	= "SpecialSymbolStyle";
	static const std::string ES		= "ElementStyle";
	static const std::string NSS	= "NamespaceStyle";
	static const std::string TS		= "TextStyle";
	static const std::string CS		= "CommentStyle";
	static const std::string XMLNS	= "xmlnsStyle";

	Converter::Converter(const boost::filesystem::wpath& originPath)
		: base(originPath)
	{
		ppr.Ind = OOX::Logic::Ind();

		rprSSS.RStyle	= SSS;
		rprES.RStyle	= ES;
		rprTS.RStyle	= TS;
		rprCS.RStyle	= CS;
		rprNSS.RStyle	= NSS;
		rprXMLNS.RStyle	= XMLNS;
	}

	void Converter::convert(XmlFormat::IXmlEvent& Event)
	{
		m_output = m_origin;
		OOX::Document& document = m_output.find<OOX::Document>();

		if (m_input.Document != 0)
		{
			document.Items->clear();

			//int percent = 100000;
			//int step = 800000 / m_input.Document->Root.Elements.size();//.Content->size();
			bool cancel = Event.Progress(0, 100000);
			if(cancel)
				return;

			OOX::Logic::Paragraph paragraph;
			paragraph.AddText(m_input.Document->Declaration->ToString(), rprSSS);
			document.Items->push_back(paragraph);
			cancel = AddElement(m_input.Document->Root, *(document.Items), 0, 100000, 800000, Event);
			if(cancel)
				return;

			cancel = Event.Progress(0, 900000);
			if(cancel)
				return;
		}
	}

	bool Converter::AddElement(const XML::XElement& element, std::vector<OOX::Logic::TextItem>& document, int level, int percent, int step, XmlFormat::IXmlEvent& Event)const
	{
		bool cancel = Event.Progress(0, percent);
		if(cancel)
			return true;

		OOX::Logic::Paragraph paragraph;
		//for(int i = 0; i < level; i++)
		//	paragraph.AddTab();
		paragraph.Property = ppr;
		paragraph.Property->Ind->Left = UniversalUnit(level * dx, UniversalUnit::Dx);
		

		paragraph.AddText("<", rprSSS);
		if(element.XName->Ns.is_init())
		{
			paragraph.AddText(element.XName->Ns->GetPrefix(), rprES);
			paragraph.AddText(":", rprES);
		}
		paragraph.AddText((*element.XName->Name), rprES);

		if(element.Space->isPreserve())
		{
			paragraph.AddText(" xml:space", rprXMLNS);
			paragraph.AddText("=\"", rprSSS);
			paragraph.AddText("preserve", rprNSS);
			paragraph.AddText("\"", rprSSS);
		}

		for(XML::const_namespace_iterator aIter = element.Namespaces.begin(); aIter != element.Namespaces.end(); aIter++)
		{
			AddNamespace(*aIter, paragraph);
		}

		for(XML::const_attribute_iterator aIter = element.Attributes.begin(); aIter != element.Attributes.end(); aIter++)
		{
			AddAttribute(*aIter, paragraph);
		}

		if(element.Nodes.size() == 0)
		{
			paragraph.AddText("/>", rprSSS);
			document.push_back(paragraph);
		}
		else
		{
			paragraph.AddText(">", rprSSS);
			document.push_back(paragraph);

			int lPercent = percent;
			int lStep = step / element.Nodes.size();
			for(XML::const_node_iterator iter = element.Nodes.begin(); iter != element.Nodes.end(); iter++)
			{
				if(iter->isElement())
				{
					cancel = AddElement(XML::XElement(*iter), document, level + 1, lPercent, lStep, Event);
					if(cancel)
						return true;
				}
				else if(iter->isText())
				{
					AddText(XML::XText(*iter), document, level);
				}
				else if(iter->isComment())
				{
					AddComment(XML::XComment(*iter), document, level);
				}
				lPercent += lStep;
			}

			OOX::Logic::Paragraph endPara;
			//for(int i = 0; i < level; i++)
			//	endPara.AddTab();
			endPara.Property = ppr;
			endPara.Property->Ind->Left = UniversalUnit(level * dx, UniversalUnit::Dx);

			endPara.AddText("</", rprSSS);
			if(element.XName->Ns.is_init())
			{
				endPara.AddText(element.XName->Ns->GetPrefix(), rprES);
				endPara.AddText(":", rprES);
			}
			endPara.AddText((*element.XName->Name), rprES);

			endPara.AddText(">", rprSSS);
			document.push_back(endPara);
		}

		cancel = Event.Progress(0, percent + step);
		if(cancel)
			return true;
		return false;
	}

	void Converter::AddAttribute(const XML::XAttribute& attribute, OOX::Logic::Paragraph& paragraph)const
	{
		paragraph.AddText(" ", rprSSS);
		if(attribute->XName->Ns.is_init())
		{
			paragraph.AddText(attribute->XName->Ns->GetPrefix(), rprES);
			paragraph.AddText(":", rprES);
		}
		paragraph.AddText((*attribute->XName->Name), rprES);
		paragraph.AddText("=\"", rprSSS);
		paragraph.AddText(attribute.value().ToString(), rprTS);
		paragraph.AddText("\"", rprSSS);
	}

	void Converter::AddNamespace(const XML::XNamespace& NameSpace, OOX::Logic::Paragraph& paragraph)const
	{
		paragraph.AddText(" ", rprSSS);
		if(NameSpace.GetPrefix() != "xmlns")
		{
			//paragraph.AddText(attribute->XName->Ns->GetPrefix(), rprES);
			paragraph.AddText("xmlns:", rprXMLNS);
		}
		paragraph.AddText(NameSpace.GetPrefix(), rprXMLNS);
		paragraph.AddText("=\"", rprSSS);
		paragraph.AddText(NameSpace.GetName(), rprNSS);
		paragraph.AddText("\"", rprSSS);
	}

	void Converter::AddText(const XML::XText& text, std::vector<OOX::Logic::TextItem>& document, int level)const
	{
		OOX::Logic::Paragraph paragraph;
		//for(int i = 0; i < level; i++)
		//	paragraph.AddTab();
		paragraph.Property = ppr;
		paragraph.Property->Ind->Left = UniversalUnit(level * dx, UniversalUnit::Dx);

		paragraph.AddText(text.Value.ToString(), rprTS);
		document.push_back(paragraph);
	}

	void Converter::AddComment(const XML::XComment& comment, std::vector<OOX::Logic::TextItem>& document, int level)const
	{
		OOX::Logic::Paragraph paragraph;
		//for(int i = 0; i < level; i++)
		//	paragraph.AddTab();
		paragraph.Property = ppr;
		paragraph.Property->Ind->Left = ::UniversalUnit(level * dx, UniversalUnit::Dx);

		paragraph.AddText("<!--", rprSSS);
		paragraph.AddText(comment.Value.ToString(), rprCS);
		paragraph.AddText("-->", rprSSS);
		document.push_back(paragraph);

	}

	void Converter::read(const boost::filesystem::wpath& path, bool space)
	{
		m_input.read(path, space);
	}

	void Converter::write(const boost::filesystem::wpath& path) const
	{
		m_output.write(path);
	}

} // namespace Xml2Docx