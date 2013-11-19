#include "NvGrpSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		NvGrpSpPr::NvGrpSpPr()
		{
		}


		NvGrpSpPr::~NvGrpSpPr()
		{
		}
	

		NvGrpSpPr::NvGrpSpPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NvGrpSpPr& NvGrpSpPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NvGrpSpPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cNvPr = element.element("cNvPr");
			cNvGrpSpPr = element.element("cNvGrpSpPr");
			nvPr = element.element("nvPr");

			FillParentPointersForChilds();
		}


		const XML::XNode NvGrpSpPr::toXML() const
		{
			return
				XML::XElement(ns.p + "nvGrpSpPr",
					XML::Write(cNvPr) +
					XML::Write(cNvGrpSpPr) +
					XML::Write(nvPr)
				);
		}

		void NvGrpSpPr::FillParentPointersForChilds()
		{
			cNvPr->SetParentPointer(*this);
			cNvGrpSpPr->SetParentPointer(*this);
			nvPr->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX