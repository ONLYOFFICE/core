#include "NvCxnSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		NvCxnSpPr::NvCxnSpPr()
		{
		}


		NvCxnSpPr::~NvCxnSpPr()
		{
		}
	

		NvCxnSpPr::NvCxnSpPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NvCxnSpPr& NvCxnSpPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NvCxnSpPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cNvPr = element.element("cNvPr");
			cNvCxnSpPr = element.element("cNvCxnSpPr");
			nvPr = element.element("nvPr");

			FillParentPointersForChilds();
		}


		const XML::XNode NvCxnSpPr::toXML() const
		{
			return XML::XElement(ns.p + "nvCxnSpPr",
					XML::Write(cNvPr) +
					XML::Write(cNvCxnSpPr) +
					XML::Write(nvPr)
				);
		}

		void NvCxnSpPr::FillParentPointersForChilds()
		{
			cNvPr->SetParentPointer(*this);
			cNvCxnSpPr->SetParentPointer(*this);
			nvPr->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX