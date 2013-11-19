#include "NvPicPr.h"

namespace PPTX
{
	namespace Logic
	{

		NvPicPr::NvPicPr()
		{
		}


		NvPicPr::~NvPicPr()
		{
		}
	

		NvPicPr::NvPicPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NvPicPr& NvPicPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NvPicPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cNvPr = element.element("cNvPr");
			cNvPicPr = element.element("cNvPicPr");
			nvPr = element.element("nvPr");

			FillParentPointersForChilds();
		}


		const XML::XNode NvPicPr::toXML() const
		{
			return XML::XElement(ns.p + "nvPicPr",
					XML::Write(cNvPr) +
					XML::Write(cNvPicPr) +
					XML::Write(nvPr)
				);
		}

		void NvPicPr::FillParentPointersForChilds()
		{
			cNvPr->SetParentPointer(*this);
			cNvPicPr->SetParentPointer(*this);
			nvPr->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX