#include "Paragraph.h"

namespace PPTX
{
	namespace Logic
	{

		Paragraph::Paragraph()
		{
		}


		Paragraph::~Paragraph()
		{
		}
	

		Paragraph::Paragraph(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Paragraph& Paragraph::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Paragraph::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			RunElems->clear();

			pPr = element.element("pPr");
			XML::Fill(RunElems, element, "r", "fld", "br");
			endParaRPr = element.element("endParaRPr");
			
			FillParentPointersForChilds();
		}


		const XML::XNode Paragraph::toXML() const
		{
			return XML::XElement(ns.a + "p",
					XML::Write(pPr) +
					XML::Write(RunElems) +
					XML::Write(endParaRPr)
				);
		}

		void Paragraph::FillParentPointersForChilds()
		{
			if(pPr.is_init())
				pPr->SetParentPointer(*this);
			if(endParaRPr.is_init())
				endParaRPr->SetParentPointer(*this);
			for(std::list<RunElem>::iterator i = RunElems->begin(); i != RunElems->end(); i++)
				i->SetParentPointer(*this);
		}

		std::string Paragraph::GetText()const
		{
			std::string result = "";
			for(std::list<RunElem>::const_iterator i = RunElems->begin(); i != RunElems->end(); i++)
			{
				result = result + i->GetText();
			}
			result = result + "\n";
			return result;
		}

		std::wstring Paragraph::GetWText()const
		{
			std::wstring result = L"";
			for(std::list<RunElem>::const_iterator i = RunElems->begin(); i != RunElems->end(); i++)
			{
				result = result + i->GetWText();
			}
			result = result + L"\n";
			return result;
		}

	} // namespace Logic
} // namespace PPTX