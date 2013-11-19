#include "TxBody.h"

namespace PPTX
{
	namespace Logic
	{

		TxBody::TxBody()
		{
		}


		TxBody::~TxBody()
		{
		}
	

		TxBody::TxBody(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TxBody& TxBody::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TxBody::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Paragrs->clear();

			bodyPr = element.element("bodyPr");
			lstStyle = element.element("lstStyle");
			XML::Fill(Paragrs,element,"p");

			FillParentPointersForChilds();
		}


		const XML::XNode TxBody::toXML() const
		{
			return XML::XElement(ns.p + "txBody",
					XML::Write(bodyPr) +
					XML::Write(lstStyle) +
					XML::Write(Paragrs)
				);
		}

		std::string TxBody::GetText()const
		{
			std::string result = "";
			for(std::list<Paragraph>::const_iterator i = Paragrs->begin(); i != Paragrs->end(); i++)
			{
				result = result + i->GetText();
			}
			return result;
		}

		std::wstring TxBody::GetWText()const
		{
			std::wstring result = L"";
			for(std::list<Paragraph>::const_iterator i = Paragrs->begin(); i != Paragrs->end(); i++)
			{
				result = result + i->GetWText();
			}
			return result;
		}

		void TxBody::FillParentPointersForChilds()
		{
			bodyPr->SetParentPointer(*this);
			if(lstStyle.is_init())
				lstStyle->SetParentPointer(*this);
			for(std::list<Paragraph>::iterator i = Paragrs->begin(); i != Paragrs->end(); i++)
				i->SetParentPointer(*this);
		}

		void TxBody::Merge(nullable_property<TxBody>& txBody)const
		{
			if(!txBody.is_init())
				txBody = TxBody();
			bodyPr->Merge(*txBody->bodyPr);
			if(lstStyle.is_init())
				lstStyle->Merge(txBody->lstStyle);
		}
	} // namespace Logic
} // namespace PPTX