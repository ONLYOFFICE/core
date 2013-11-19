#include "NvSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		NvSpPr::NvSpPr()
		{
		}


		NvSpPr::~NvSpPr()
		{
		}
	

		NvSpPr::NvSpPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NvSpPr& NvSpPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NvSpPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cNvPr = element.element("cNvPr");
			cNvSpPr = element.element("cNvSpPr");
			nvPr = element.element("nvPr");

			FillParentPointersForChilds();
		}


		const XML::XNode NvSpPr::toXML() const
		{
			return XML::XElement(ns.p + "nvSpPr",
					XML::Write(cNvPr) +
					XML::Write(cNvSpPr) +
					XML::Write(nvPr)
				);
		}

		void NvSpPr::FillParentPointersForChilds()
		{
			cNvPr->SetParentPointer(*this);
			cNvSpPr->SetParentPointer(*this);
			nvPr->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX