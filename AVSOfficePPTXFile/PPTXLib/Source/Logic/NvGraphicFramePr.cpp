#include "NvGraphicFramePr.h"

namespace PPTX
{
	namespace Logic
	{

		NvGraphicFramePr::NvGraphicFramePr()
		{
		}


		NvGraphicFramePr::~NvGraphicFramePr()
		{
		}
	

		NvGraphicFramePr::NvGraphicFramePr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NvGraphicFramePr& NvGraphicFramePr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NvGraphicFramePr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cNvPr = element.element("cNvPr");
			cNvGraphicFramePr = element.element("cNvGraphicFramePr");
			nvPr = element.element("nvPr");

			FillParentPointersForChilds();
		}


		const XML::XNode NvGraphicFramePr::toXML() const
		{
			return XML::XElement(ns.p + "nvGraphicFramePr",
					XML::Write(cNvPr) +
					XML::Write(cNvGraphicFramePr) +
					XML::Write(nvPr)
				);
		}

		void NvGraphicFramePr::FillParentPointersForChilds()
		{
			cNvPr->SetParentPointer(*this);
			cNvGraphicFramePr->SetParentPointer(*this);
			nvPr->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX