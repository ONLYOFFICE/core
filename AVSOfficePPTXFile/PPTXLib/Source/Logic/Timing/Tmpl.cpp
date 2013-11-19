#include "Tmpl.h"

namespace PPTX
{
	namespace Logic
	{

		Tmpl::Tmpl()
		{
		}

		Tmpl::~Tmpl()
		{
		}

		Tmpl::Tmpl(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Tmpl& Tmpl::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Tmpl::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			tnLst = element.element("tnLst");
			lvl = element.attribute("lvl").value();
			FillParentPointersForChilds();
		}

		const XML::XNode Tmpl::toXML() const
		{
			return XML::XElement(ns.p + "tmpl",
					XML::Write(tnLst) +
					XML::XAttribute("lvl", lvl)
				);
		}

		void Tmpl::FillParentPointersForChilds()
		{
			tnLst->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX