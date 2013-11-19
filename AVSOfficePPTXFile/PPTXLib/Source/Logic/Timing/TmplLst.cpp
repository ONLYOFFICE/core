#include "TmplLst.h"

namespace PPTX
{
	namespace Logic
	{

		TmplLst::TmplLst()
		{
		}

		TmplLst::~TmplLst()
		{
		}

		TmplLst::TmplLst(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TmplLst& TmplLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TmplLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			XML::Fill(list, element, "tmpl");
			FillParentPointersForChilds();
		}

		const XML::XNode TmplLst::toXML() const
		{
			return XML::XElement(ns.p + "tmplLst",
					XML::Write(list)
				);
		}

		void TmplLst::FillParentPointersForChilds()
		{
			for(std::list<Tmpl>::iterator i = list->begin(); i != list->end(); i++)
				i->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX