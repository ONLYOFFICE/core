#include "RunProperties.h"

namespace PPTX
{
	namespace Logic
	{

		RunProperties::RunProperties()
		{
		}


		RunProperties::~RunProperties()
		{
		}
	

		RunProperties::RunProperties(const XML::XNode& node)
		{
			fromXML(node);
		}


		const RunProperties& RunProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void RunProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_namespace = element.XName.get().Ns;
			m_name = element.XName.get().Name;

			altLang = element.attribute("altLang").value();
			b = element.attribute("b").value();
			bmk = element.attribute("bmk").value();
			cap = element.attribute("cap").value();
			dirty = element.attribute("dirty").value();
			err = element.attribute("err").value();
			i = element.attribute("i").value();
			kern = element.attribute("kern").value();
			kumimoji = element.attribute("kumimoji").value();
			lang = element.attribute("lang").value();
			noProof = element.attribute("noProof").value();
			normalizeH = element.attribute("normalizeH").value();
			smtClean = element.attribute("smtClean").value();
			smtId = element.attribute("smtId").value();
			strike = element.attribute("strike").value();
			sz = element.attribute("sz").value();
			u = element.attribute("u").value();
			baseline = element.attribute("baseline").value();
			spc = element.attribute("spc").value();

			ln = element.element("ln");
			Fill.GetFillFrom(element);
			EffectList.GetEffectListFrom(element);
			latin = element.element("latin");
			ea = element.element("ea");
			cs = element.element("cs");
			sym = element.element("sym");
			hlinkClick = element.element("hlinkClick");
			hlinkMouseOver = element.element("hlinkMouseOver");

			FillParentPointersForChilds();
		}


		const XML::XNode RunProperties::toXML() const
		{
			return XML::XElement(m_namespace + m_name.get(),
				XML::XAttribute("kumimoji", kumimoji) +
				XML::XAttribute("lang", lang) +
				XML::XAttribute("altLang", altLang) +
				XML::XAttribute("sz", sz) +
				XML::XAttribute("b", b) +
				XML::XAttribute("i", i) +
				XML::XAttribute("u", u) +
				XML::XAttribute("strike", strike) +
				XML::XAttribute("kern", kern) +
				XML::XAttribute("cap", cap) +
				XML::XAttribute("spc", spc) +
				XML::XAttribute("normalizeH", normalizeH) +
				XML::XAttribute("baseline", baseline) +
				XML::XAttribute("noProof", noProof) +
				XML::XAttribute("dirty", dirty) +
				XML::XAttribute("err", err) +
				XML::XAttribute("smtClean", smtClean) +
				XML::XAttribute("smtId", smtId) +
				XML::XAttribute("bmk", bmk) +
				XML::Write(ln) +
				XML::Write(Fill) +
				XML::Write(EffectList) +
				XML::Write(latin) +
				XML::Write(ea) +
				XML::Write(cs) +
				XML::Write(sym) +
				XML::Write(hlinkClick) +
				XML::Write(hlinkMouseOver)
			);
		}

		void RunProperties::FillParentPointersForChilds()
		{
			if(ln.is_init())
				ln->SetParentPointer(*this);
			Fill.SetParentPointer(*this);
			EffectList.SetParentPointer(*this);
			if(latin.is_init())
				latin->SetParentPointer(*this);
			if(ea.is_init())
				ea->SetParentPointer(*this);
			if(cs.is_init())
				cs->SetParentPointer(*this);
			if(sym.is_init())
				sym->SetParentPointer(*this);
			if(hlinkClick.is_init())
				hlinkClick->SetParentPointer(*this);
			if(hlinkMouseOver.is_init())
				hlinkMouseOver->SetParentPointer(*this);
		}

		void RunProperties::Merge(nullable_property<RunProperties>& props)const
		{
			if(!props.is_init())
				props = RunProperties();

			if(ln.is_init())
				ln->Merge(props->ln);
			if(Fill.is_init())
				props->Fill = Fill;//.fromXML(Fill.toXML());
//			EffectProperties EffectList;
			//highlight (Highlight Color)  §21.1.2.3.4 
			//uLn (Underline Stroke)  §21.1.2.3.14 
			//uLnTx (Underline Follows Text)  §21.1.2.3.15 
			//uFill (Underline Fill)  §21.1.2.3.12 
			//uFillTx (Underline Fill Properties Follow Text)  §21.1.2.3.13 

			if(latin.is_init())	latin->Merge(props->latin);
			if(ea.is_init())	ea->Merge(props->ea);
			if(cs.is_init())	cs->Merge(props->cs);
			if(sym.is_init())	sym->Merge(props->sym);
/*
			nullable_property<Hyperlink> hlinkClick;
			nullable_property<Hyperlink> hlinkMouseOver;
			//rtl (Right to Left Run)  §21.1.2.2.8 
*/
			// Attributes
			if(altLang.is_init()) props->altLang = *altLang;
			if(b.is_init()) props->b = *b;
			if(baseline.is_init()) props->baseline = *baseline;
			if(bmk.is_init()) props->bmk = *bmk;
			if(cap.is_init()) props->cap = *cap;
			if(dirty.is_init()) props->dirty = *dirty;
			if(err.is_init()) props->err = *err;
			if(i.is_init()) props->i = *i;
			if(kern.is_init()) props->kern = *kern;
			if(kumimoji.is_init()) props->kumimoji = *kumimoji;
			if(lang.is_init()) props->lang = *lang;
			if(noProof.is_init()) props->noProof = *noProof;
			if(normalizeH.is_init()) props->normalizeH = *normalizeH;
			if(smtClean.is_init()) props->smtClean = *smtClean;
			if(smtId.is_init()) props->smtId = *smtId;
			if(spc.is_init()) props->spc = *spc;
			if(strike.is_init()) props->strike = *strike;
			if(sz.is_init()) props->sz = *sz;
			if(u.is_init()) props->u = *u;
		}
	} // namespace Logic
} // namespace PPTX