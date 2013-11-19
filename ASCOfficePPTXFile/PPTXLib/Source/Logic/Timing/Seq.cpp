#include "Seq.h"

namespace PPTX
{
	namespace Logic
	{

		Seq::Seq()
		{
		}

		Seq::~Seq()
		{
		}

		Seq::Seq(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Seq& Seq::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Seq::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cTn = element.element("cTn");
			nextCondLst = element.element("nextCondLst");
			prevCondLst = element.element("prevCondLst");

			concurrent = element.attribute("concurrent").value();
			nextAc = element.attribute("nextAc").value();
			prevAc = element.attribute("prevAc").value();

			FillParentPointersForChilds();
		}

		const XML::XNode Seq::toXML() const
		{
			return XML::XElement(ns.p + "seq",
					XML::XAttribute("concurrent", concurrent) +
					XML::XAttribute("prevAc", prevAc) +
					XML::XAttribute("nextAc", nextAc) +
					XML::Write(cTn) +
					XML::Write(prevCondLst) +
					XML::Write(nextCondLst)
				);
		}

		void Seq::FillParentPointersForChilds()
		{
			cTn->SetParentPointer(*this);
			if(prevCondLst.is_init())
				prevCondLst->SetParentPointer(*this);
			if(nextCondLst.is_init())
				nextCondLst->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX