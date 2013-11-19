#include "TnLst.h"

namespace PPTX
{
	namespace Logic
	{

		TnLst::TnLst()
		{
		}

		TnLst::~TnLst()
		{
		}

		TnLst::TnLst(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TnLst& TnLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TnLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			name = element.XName->Name;

			XML::Fill(list, element);
			FillParentPointersForChilds();
		}

		const XML::XNode TnLst::toXML() const
		{
			return XML::XElement(ns.p + name.get(),
					XML::Write(list)
				);
		}

		void TnLst::FillParentPointersForChilds()
		{
			for(std::list<TimeNodeBase>::iterator i = list->begin(); i != list->end(); i++)
				i->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX