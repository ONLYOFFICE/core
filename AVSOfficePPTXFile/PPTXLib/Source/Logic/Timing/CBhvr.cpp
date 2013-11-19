#include "CBhvr.h"

namespace PPTX
{
	namespace Logic
	{

		CBhvr::CBhvr()
		{
		}

		CBhvr::~CBhvr()
		{
		}

		CBhvr::CBhvr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const CBhvr& CBhvr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void CBhvr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			accumulate = element.attribute("accumulate").value();
			additive = element.attribute("additive").value();
			by = element.attribute("by").value();
			from = element.attribute("from").value();
			override_ = element.attribute("override").value();
			rctx = element.attribute("rctx").value();
			to = element.attribute("to").value();
			xfrmType = element.attribute("xfrmType").value();


			cTn = element.element("cTn");
			tgtEl = element.element("tgtEl");
			attrNameLst = element.element("attrNameLst");

			FillParentPointersForChilds();
		}

		const XML::XNode CBhvr::toXML() const
		{
			return XML::XElement(ns.p + "cBhvr",
				XML::XAttribute("accumulate", accumulate) +
				XML::XAttribute("additive", additive) +
				XML::XAttribute("by", by) +
				XML::XAttribute("from", from) +
				XML::XAttribute("override", override_) +
				XML::XAttribute("rctx", rctx) +
				XML::XAttribute("to", to) +
				XML::XAttribute("xfrmType", xfrmType) +
				XML::Write(cTn) +
				XML::Write(tgtEl) +
				XML::Write(attrNameLst)
			);
		}

		void CBhvr::FillParentPointersForChilds()
		{
			cTn->SetParentPointer(*this);
			tgtEl->SetParentPointer(*this);
			if(attrNameLst.is_init())
				attrNameLst->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX