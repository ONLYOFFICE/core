#include "Converter.h"
#include <boost/foreach.hpp>
#include "AbstractConverter.h"
#include "Txt.h"
#include "Docx.h"
#include "TxtEvent.h"

namespace Txt2Docx
{
	class Converter_Impl : public AbstractConverter<Txt::File, Docx::Folder>
	{
	public:
		Converter_Impl(const boost::filesystem::wpath& originPath, int encoding);
		void convert(Txt::ITxtEvent& Event);
	};

    Converter::Converter(const boost::filesystem::wpath& originPath, int encoding) : converter_( new Converter_Impl(originPath, encoding) )
    {    
    }
    
    Converter::~Converter()
    {
        delete converter_;
    }
    
    void Converter::convert(Txt::ITxtEvent& Event)
    {
        return converter_->convert(Event);    
    }

    void Converter::read(const boost::filesystem::wpath& path)
    {
        return converter_->read(path);
    }

    void Converter::write(const boost::filesystem::wpath& path) const
    {
        return converter_->write(path);
    }

	Converter_Impl::Converter_Impl(const boost::filesystem::wpath& originPath, int encoding)
		: base(originPath)
	{
		((Txt::File*)(&m_input))->m_nEncoding = encoding;
	}

	void Converter_Impl::convert(Txt::ITxtEvent& Event)
	{
		m_output = m_origin;
		OOX::Document& document = m_output.find<OOX::Document>();

		if (!m_input.Content->empty())
		{
			document.Items->clear();

			int percent = 100000;
			int step = 800000 / m_input.Content->size();
			bool cancel = Event.Progress(0, 100000);
			if(cancel)
				return;
/*
			OOX::Logic::ParagraphProperty pPr;
			OOX::Logic::Spacing space;
			space.After = 0;
			space.Line = 240;
			space.LineRule = "auto";			
			pPr.Spacing = space;

			OOX::Logic::RFonts rFont;
			rFont.Ascii = "Courier New";
			rFont.HAnsi = "Courier New";
			rFont.Cs = "Courier New";

			OOX::Logic::RunProperty rPr;
			rPr.RFonts = rFont;

			pPr.RunProperty = rPr;

			OOX::Logic::Paragraph paragraph;
			paragraph.Property = pPr;
*/
			BOOST_FOREACH(const std::string& line, *m_input.Content)
			{
				//OOX::Logic::ParagraphProperty pPr;
				//OOX::Logic::Spacing space;
				//space.After = 0;
				//space.Line = 240;
				//space.LineRule = "auto";			
				//pPr.Spacing = space;

				//OOX::Logic::RFonts rFont;
				//rFont.Ascii = "Courier New";
				//rFont.HAnsi = "Courier New";
				//rFont.Cs = "Courier New";

				//OOX::Logic::RunProperty rPr;
				//rPr.RFonts = rFont;

				//pPr.RunProperty = rPr;

				//OOX::Logic::Paragraph paragraph;
				//paragraph.Property = pPr;

				OOX::Logic::Paragraph temp;// = paragraph;
				std::string str = line;
				while(str.find("\x08") != str.npos)
				{
					str.erase(str.find("\x08"), 1);//, "");
				}

				if(str != "")
					temp.AddText(str);//, rPr);
				document.Items->push_back(temp);

				percent += step;
				cancel = Event.Progress(0, percent);
				if(cancel)
					return;
			}
		}
		Event.Progress(0, 900000);
	}
} // namespace Txt2Docx