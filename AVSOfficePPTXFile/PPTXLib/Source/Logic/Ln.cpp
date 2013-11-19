#include "Ln.h"

namespace PPTX
{
	namespace Logic
	{
		Ln::Ln()
		{
		}

		Ln::~Ln()
		{
		}

		Ln::Ln(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Ln& Ln::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Ln::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			algn = element.attribute("algn").value();
			cap = element.attribute("cap").value();
			cmpd = element.attribute("cmpd").value();
			w = element.attribute("w").value();

			Fill.GetFillFrom(element);
			prstDash = element.element("prstDash");
			Join.GetJoinFrom(element);
			headEnd = element.element("headEnd");
			tailEnd = element.element("tailEnd");
			
			FillParentPointersForChilds();
		}

		const XML::XNode Ln::toXML() const
		{
			return XML::XElement(ns.a + "ln",
					XML::XAttribute("w", w) +
					XML::XAttribute("cap", cap) +
					XML::XAttribute("cmpd", cmpd) +
					XML::XAttribute("algn", algn) +
					XML::Write(Fill) +
					XML::Write(prstDash) +
					XML::Write(Join) +
					XML::Write(headEnd) +
					XML::Write(tailEnd)
				);
		}
/*
		const Ln Ln::MergeWith(const Ln& line)const
		{
			Ln result;
			result.Fill = Fill;
			if(line.Fill.is_init())
				result.Fill = line.Fill;

			result.prstDash = prstDash;
			if(line.prstDash.is_init())
				result.prstDash = line.prstDash;

			result.Join = Join;
			if(line.Join.is_init())
				result.Join = line.Join;

			result.headEnd = headEnd;
			if(line.headEnd.is_init())
				result.headEnd = line.headEnd;

			result.tailEnd = tailEnd;
			if(line.tailEnd.is_init())
				result.tailEnd = line.tailEnd;

			result.algn = algn;
			if(line.algn.is_init())
				result.algn = line.algn;

			result.cap = cap;
			if(line.cap.is_init())
				result.cap = line.cap;

			result.cmpd = cmpd;
			if(line.cmpd.is_init())
				result.cmpd = line.cmpd;

			result.w = w;
			if(line.w.is_init())
				result.w = line.w;

			return result;
		}
*/
		void Ln::Merge(nullable_property<Ln>& line)const
		{
			if(!line.is_init())
				line = Ln();

			if(Fill.is_init())
				line->Fill = Fill;//.fromXML(Fill.toXML());
			if(prstDash.is_init())
				line->prstDash = *prstDash;
			if(Join.is_init())
				line->Join = Join;
			if(headEnd.is_init())
				headEnd->Merge(line->headEnd);
			if(tailEnd.is_init())
				tailEnd->Merge(line->tailEnd);

			if(algn.is_init())
				line->algn = *algn;
			if(cap.is_init())
				line->cap = *cap;
			if(cmpd.is_init())
				line->cmpd = *cmpd;
			if(w.is_init())
				line->w = *w;
		}

		void Ln::Merge(Ln& line)const
		{
			if(Fill.is_init())
				line.Fill = Fill;//.fromXML(Fill.toXML());
			if(prstDash.is_init())
				line.prstDash = *prstDash;
			if(Join.is_init())
				line.Join = Join;
			if(headEnd.is_init())
				headEnd->Merge(line.headEnd);
			if(tailEnd.is_init())
				tailEnd->Merge(line.tailEnd);

			if(algn.is_init())
				line.algn = *algn;
			if(cap.is_init())
				line.cap = *cap;
			if(cmpd.is_init())
				line.cmpd = *cmpd;
			if(w.is_init())
				line.w = *w;
		}

		void Ln::FillParentPointersForChilds()
		{
			Fill.SetParentPointer(*this);
			Join.SetParentPointer(*this);
			if(prstDash.is_init())
				prstDash->SetParentPointer(*this);
			if(headEnd.is_init())
				headEnd->SetParentPointer(*this);
			if(tailEnd.is_init())
				tailEnd->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX