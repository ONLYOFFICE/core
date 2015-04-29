#include "Converter.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "../TxtFormat/TxtFormat.h"
#include "../TxtXmlEvent.h"

namespace Txt2Docx
{
	class Converter_Impl
	{
	public:
		Converter_Impl(int encoding);

		void convert(TxtXml::ITxtXmlEvent& Event);

		Txt::File		m_inputFile;
		OOX::CDocument	m_outputFile;

	};

    Converter::Converter(int encoding) : converter_( new Converter_Impl(encoding) )
    {    
    }
    
    Converter::~Converter()
    {
        delete converter_;
    }
    
    void Converter::convert(TxtXml::ITxtXmlEvent& Event)
    {
        return converter_->convert(Event);    
    }

    void Converter::read(const std::wstring& path)
    {
        return converter_->m_inputFile.read(path);
    }

    void Converter::write(/*const std::wstring& path*/XmlUtils::CStringWriter & stringWriter)
    {
		for (long i=0;i < converter_->m_outputFile.m_arrItems.size(); i++)
		{
			if (converter_->m_outputFile.m_arrItems[i] != NULL)
				stringWriter.WriteString(converter_->m_outputFile.m_arrItems[i]->toXML());
		}
		//BOOL res = converter_->m_outputFile.Write(std_string2string(path.string()));
		return;
    }

	Converter_Impl::Converter_Impl(int encoding)
	{
		m_inputFile.m_nEncoding = encoding;
		
	}

	void Converter_Impl::convert(TxtXml::ITxtXmlEvent& Event)
	{
		//smart_ptr<OOX::File> pFile = m_outputFile.Find(OOX::FileTypes::Document);
		
		OOX::CDocument *pDocument = &m_outputFile;//NULL;

		if (!m_inputFile.m_listContent.empty() /*&& pFile.IsInit() && OOX::FileTypes::Document == pFile->type()*/)
		{
			//pDocument = (OOX::CDocument*)pFile.operator->();
			//pDocument->ClearItems();

			int percent = 100000;
			int step = 800000 / m_inputFile.m_listContentSize; // !!!!!
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
			for (std::list<std::wstring>::iterator line = m_inputFile.m_listContent.begin(); line != m_inputFile.m_listContent.end(); line++)
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

				OOX::Logic::CParagraph *temp = new OOX::Logic::CParagraph();
				
				while(line->find(_T("\x08")) != line->npos)
				{
					line->erase(line->find(_T("\x08")), 1);//, "");
				}

				if(line->length() > 0)
                {
                    CString s = std_string2string(*line);
                    temp->AddText(s);//, rPr);
                }
				
				pDocument->m_arrItems.push_back(temp);

				percent += step;
				cancel = Event.Progress(0, percent);
				if(cancel)
					return;
			}
		}
		Event.Progress(0, 900000);
	}
} // namespace Txt2Docx
