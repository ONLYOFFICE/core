#include "TavLst.h"

namespace PPTX
{
	namespace Logic
	{

		TavLst::TavLst()
		{
		}

		TavLst::~TavLst()
		{
		}

		TavLst::TavLst(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TavLst& TavLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TavLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			XML::Fill(list, element, "tav");
			FillParentPointersForChilds();
		}

		const XML::XNode TavLst::toXML() const
		{
			return XML::XElement(ns.p + "tavLst",
					XML::Write(list)
				);
		}

		void TavLst::FillParentPointersForChilds()
		{
			for(std::list<Tav>::iterator i = list->begin(); i != list->end(); i++)
				i->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX