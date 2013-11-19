#include "TextParagraphPr.h"

namespace PPTX
{
	namespace Logic
	{

		TextParagraphPr::TextParagraphPr()
		{
		}


		TextParagraphPr::~TextParagraphPr()
		{
		}


		TextParagraphPr::TextParagraphPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TextParagraphPr& TextParagraphPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TextParagraphPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			algn = element.attribute("algn").value();
			defTabSz = element.attribute("defTabSz").value();
			eaLnBrk = element.attribute("eaLnBrk").value();
			fontAlgn = element.attribute("fontAlgn").value();
			hangingPunct = element.attribute("hangingPunct").value();
			indent = element.attribute("indent").value();
			latinLnBrk = element.attribute("latinLnBrk").value();
			lvl = element.attribute("lvl").value();
			marL = element.attribute("marL").value();
			marR = element.attribute("marR").value();
			rtl = element.attribute("rtl").value();

			lnSpc = element.element("lnSpc");
			spcAft = element.element("spcAft");
			spcBef = element.element("spcBef");
			buColor.ReadBulletColorFrom(element);
			buSize.ReadBulletSizeFrom(element);
			buTypeface.ReadBulletTypefaceFrom(element);
			ParagraphBullet.ReadBulletFrom(element);

			if(element.element("tabLst").exist())
				XML::Fill(tabLst, element.element("tabLst"), "tab");
			defRPr = element.element("defRPr");

			FillParentPointersForChilds();
		}


		const XML::XNode TextParagraphPr::toXML() const
		{
			return XML::XElement(m_namespace + m_name.get(),
					XML::XAttribute("marL", marL) +
					XML::XAttribute("marR", marR) +
					XML::XAttribute("lvl", lvl) +
					XML::XAttribute("indent", indent) +
					XML::XAttribute("algn", algn) +
					XML::XAttribute("defTabSz", defTabSz) +
					XML::XAttribute("rtl", rtl) +
					XML::XAttribute("eaLnBrk", eaLnBrk) +
					XML::XAttribute("fontAlgn", fontAlgn) +
					XML::XAttribute("latinLnBrk", latinLnBrk) +
					XML::XAttribute("hangingPunct", hangingPunct) +
					XML::Write(lnSpc) +
					XML::Write(spcBef) +
					XML::Write(spcAft) +
					XML::Write(buColor) +
					XML::Write(buSize) +
					XML::Write(buTypeface) +
					XML::Write(ParagraphBullet) +
					XML::WriteIf(XML::XElement(ns.a + "tabLst", XML::Write(tabLst)), !tabLst->empty()) +
					XML::Write(defRPr)
				);
		}

		void TextParagraphPr::FillParentPointersForChilds()
		{
			if(lnSpc.is_init())
				lnSpc->SetParentPointer(*this);
			if(spcAft.is_init())
				spcAft->SetParentPointer(*this);
			if(spcBef.is_init())
				spcBef->SetParentPointer(*this);

			buColor.SetParentPointer(*this);
			buSize.SetParentPointer(*this);
			buTypeface.SetParentPointer(*this);
			ParagraphBullet.SetParentPointer(*this);

			if(defRPr.is_init())
				defRPr->SetParentPointer(*this);
		}

		void TextParagraphPr::Merge(nullable_property<TextParagraphPr>& props)const
		{
			if(!props.is_init())
				props = TextParagraphPr();

			if(lnSpc.is_init())
			{
				props->lnSpc = TextSpacing();
				props->lnSpc->fromXML(lnSpc->toXML());
			}
			if(spcAft.is_init())
			{
				props->spcAft = TextSpacing();
				props->spcAft->fromXML(spcAft->toXML());
			}
			if(spcBef.is_init())
			{
				props->spcBef = TextSpacing();
				props->spcBef->fromXML(spcBef->toXML());
			}

			if(buColor.is_init())
				props->buColor.fromXML(buColor.toXML());
			if(buSize.is_init())
				props->buSize.fromXML(buSize.toXML());
			if(buTypeface.is_init())
				props->buTypeface.fromXML(buTypeface.toXML());
			if(ParagraphBullet.is_init())
				props->ParagraphBullet.fromXML(ParagraphBullet.toXML());

			//property<std::list<Tab> > tabLst;// (Tab List)
			if(defRPr.is_init())
				defRPr->Merge(props->defRPr);

			// Attribs
			if(algn.is_init())			props->algn = *algn;
			if(defTabSz.is_init())		props->defTabSz = *defTabSz;
			if(eaLnBrk.is_init())		props->eaLnBrk = *eaLnBrk;
			if(fontAlgn.is_init())		props->fontAlgn = *fontAlgn;
			if(hangingPunct.is_init())	props->hangingPunct = *hangingPunct;
			if(indent.is_init())		props->indent = *indent;
			if(latinLnBrk.is_init())	props->latinLnBrk = *latinLnBrk;
			if(lvl.is_init())			props->lvl = *lvl;
			if(marL.is_init())			props->marL = *marL;
			if(marR.is_init())			props->marR = *marR;
			if(rtl.is_init())			props->rtl = *rtl;
		}
	} // namespace Logic
} // namespace PPTX