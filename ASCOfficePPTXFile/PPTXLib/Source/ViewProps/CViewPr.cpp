#include "CViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		CViewPr::CViewPr()
		{
		}

		CViewPr::~CViewPr()
		{
		}

		CViewPr::CViewPr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const CViewPr& CViewPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void CViewPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			attrVarScale = element.attribute("varScale").value();

			Scale = element.element("scale");
			Origin = element.element("origin");

			FillParentPointersForChilds();
		}

		const XML::XNode CViewPr::toXML() const
		{
			return	XML::XElement(ns.p + "cViewPr",
				XML::XAttribute("varScale", attrVarScale) +
				XML::Write(Scale) +
				XML::Write(Origin)
			);
		}

		void CViewPr::FillParentPointersForChilds()
		{
			Scale->SetParentPointer(*this);
			Origin->SetParentPointer(*this);
		}
	} // namespace nsViewProps
} // namespace PPTX