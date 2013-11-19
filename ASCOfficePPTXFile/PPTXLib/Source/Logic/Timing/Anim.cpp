#include "Anim.h"

namespace PPTX
{
	namespace Logic
	{

		Anim::Anim()
		{
		}

		Anim::~Anim()
		{
		}

		Anim::Anim(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Anim& Anim::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Anim::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			by = element.attribute("by").value();
			from = element.attribute("from").value();
			to = element.attribute("to").value();
			calcmode = element.attribute("calcmode").value();
			valueType = element.attribute("valueType").value();

			cBhvr = element.element("cBhvr");
			tavLst = element.element("tavLst");

			FillParentPointersForChilds();
		}

		const XML::XNode Anim::toXML() const
		{
			return XML::XElement(ns.p + "anim",
				XML::XAttribute("by", by) +
				XML::XAttribute("from", from) +
				XML::XAttribute("to", to) +
				XML::XAttribute("calcmode", calcmode) +
				XML::XAttribute("valueType", valueType) +
				XML::Write(cBhvr) +
				XML::Write(tavLst)
				);
		}

		void Anim::FillParentPointersForChilds()
		{
			cBhvr->SetParentPointer(*this);
			if(tavLst.is_init())
				tavLst->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX