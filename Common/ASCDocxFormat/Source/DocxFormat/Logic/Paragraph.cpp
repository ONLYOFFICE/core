
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Paragraph.h"
#include "RunProperty.h"
#include "Exception/log_runtime_error.h"
#include "Run.h"
#include "Drawing.h"
#include "Tab.h"
#include "Break.h"
#include "Text.h"
#include "Symbol.h"


namespace OOX
{
	namespace Logic
	{

		Paragraph::Paragraph()
//			: m_inList(false)
		{
		}


		Paragraph::~Paragraph()
		{
		}


		Paragraph::Paragraph(const XML::XNode& node)
		{
			fromXML(node);
		}


		Paragraph::Paragraph(const RId& rId, const boost::filesystem::wpath& filename, const long width, const long height)
//			: m_inList(false)
		{
			RunItemBase* drawing = new Drawing(rId, filename, width, height);
			Run run;
			run.add(drawing);
			Items->push_back(run);
		}

		Paragraph::Paragraph(const RId& rId, const boost::filesystem::wpath& filename, const long xEmu, const std::string& hRelativeFrom, const long yEmu, const std::string& vRelativeFrom, const long widthEmu, const long heightEmu)
//			: m_inList(false)
		{
			RunItemBase* drawing = new Drawing(rId, filename, xEmu, hRelativeFrom, yEmu, vRelativeFrom, widthEmu, heightEmu);
			Run run;
			run.add(drawing);
			Items->push_back(run);
		}


		const Paragraph& Paragraph::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Paragraph::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
//			m_inList = false;
			Property = element.element("pPr");
			for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
			{
				const XML::XElement element(*i);
				if (element.XName == "r" || element.XName == "hyperlink" || element.XName == "bookmarkStart" ||
					element.XName == "bookmarkEnd" || element.XName == "fldSimple" || element.XName == "ins" ||
					element.XName == "del")
				{
					Items->push_back(ParagraphItem(*i));
				}
			}

			//if(Property.is_init() && Property->NumPr.is_init() && Property->NumPr->NumId.is_init())
			//	m_nList = true;
		}


		const XML::XNode Paragraph::toXML() const
		{
			return 
				XML::XElement(ns.w + "p",
					XML::Write(Property) + 
					XML::Write(Items)
			);
		}


		void Paragraph::Add(const Run& run)
		{
			AddRun(run);
		}


		void Paragraph::AddRun(const Run& run)
		{
			Items->push_back(run);
		}


		void Paragraph::AddText(const std::string& text)
		{
			Run run(text);
			Items->push_back(run);
		}


		void Paragraph::AddText(const std::string& text, const nullable<Logic::RunProperty>& property)
		{
			Run run(text);
			run.Property = property;
			Items->push_back(run);
		}


		void Paragraph::AddTab()
		{
			Run run;
			run.add<Tab>();
			Items->push_back(run);
		}


		void Paragraph::AddTab(const nullable<Logic::RunProperty>& property)
		{
			Run run;
			run.add<Tab>();
			run.Property = property;
			Items->push_back(run);
		}


		void Paragraph::AddLineBreak()
		{
			Run run;
			run.add<Break>();
			Items->push_back(run);
		}


		void Paragraph::AddBreak(const std::string& type)
		{
			if (type == "page" || type == "line" || type == "column")
			{
				Run run;
				Break* br = new Break();
				br->Type = type;
				run.add(br);
				Items->push_back(run);
			}
		}


		void Paragraph::AddSpace(const size_t count)
		{
			Run run;
			RunItemBase* text = new Text(std::string(count, ' '));
			run.add(text);
			Items->push_back(run);
		}


		void Paragraph::AddSpace(const size_t count, const nullable<Logic::RunProperty>& property)
		{
			Run run;
			RunItemBase* text = new Text(std::string(count, ' '));
			run.Property = property;
			run.add(text);
			Items->push_back(run);
		}


		void Paragraph::AddHyperlink(const RId& rId, const std::string& text)
		{
			RunProperty runPr;
			runPr.Under = true;
			runPr.UnderType = "single";
			runPr.FontColor = "0000FF";
			Run run(text, runPr);
			Hyperlink hyperlink;
			hyperlink.rId = rId;
			hyperlink.Runs->push_back(run);
			Items->push_back(hyperlink);
		}


		void Paragraph::AddHyperlink(const std::string& nameHref, const std::string& text)
		{
			RunProperty runPr;
			runPr.RStyle = "Hyperlink";
			Run run(text, runPr);
			Hyperlink hyperlink;
			hyperlink.Anchor  = nameHref;
			hyperlink.History = 1; // TODO еще одна магическая цифра, не счетчик
			hyperlink.Runs->push_back(run);
			Items->push_back(hyperlink);
		}


		void Paragraph::AddBookmarkStart(const std::string& name)
		{
			BookmarkStart bookmark(name);
			Items->push_back(bookmark);
		}

	
		void Paragraph::AddBookmarkEnd(const std::string& name)
		{
			BookmarkEnd bookmark;
			Items->push_back(bookmark);
		}


		void Paragraph::AddDrawing(const Drawing& drawing)
		{
			// TODO text
//			Run run;
//			run.createDrawing(drawing);
//			Items->push_back(run);
		}


		void Paragraph::setRunProperty(const OOX::Logic::RunProperty& property)
		{
			if (!Property.is_init())
				Property = ParagraphProperty();
			Property->RunProperty = property;
		}	

		
		const bool Paragraph::isInList() const
		{
			if(!Property.is_init())
				return false;
			if(!Property->NumPr.is_init())
				return false;
			if(!Property->NumPr->NumId.is_init())
				return false;
			return true;
			//return m_inList;
		}


		const int Paragraph::GetListNum() const
		{
			if(!Property.is_init())
				return 0;
			if(!Property->NumPr.is_init())
				return 0;
			return Property->NumPr->NumId.get_value_or(0);
			//m_inList = true;
		}


		const int Paragraph::GetLevel() const
		{
			if(!Property.is_init())
				return 0;
			if(!Property->NumPr.is_init())
				return 0;
			if(!Property->NumPr->Ilvl.is_init())
				return 0;
			return Property->NumPr->Ilvl.get() + 1;
		}

	} // namespace Logic
} // namespace OOX