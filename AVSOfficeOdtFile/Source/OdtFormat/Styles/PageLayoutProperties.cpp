
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "PageLayoutProperties.h"


namespace Odt
{
	namespace Styles
	{

		PageLayoutProperties::PageLayoutProperties()
		{
			PageWidth	= "210mm";
			PageHeight	= "297mm";

			MarginTop		= "20mm";
			MarginBottom	= "20mm";
			MarginLeft		= "30mm";
			MarginRight		= "15mm";
		}


		PageLayoutProperties::~PageLayoutProperties()
		{
		}


		PageLayoutProperties::PageLayoutProperties(const XML::XNode& node)
		{
			fromXML(node);
		}


		const PageLayoutProperties& PageLayoutProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void PageLayoutProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			std::string strPageW = element.attribute("page-width").value().ToString();
			if ("" == strPageW)
				strPageW = "210mm";

			std::string strPageH = element.attribute("page-height").value().ToString();
			if ("" == strPageH)
				strPageH = "297mm";

			std::string strMarginL = element.attribute("margin-left").value().ToString();
			if ("" == strMarginL)
				strMarginL = "30mm";

			std::string strMarginT = element.attribute("margin-top").value().ToString();
			if ("" == strMarginT)
				strMarginT = "20mm";

			std::string strMarginR = element.attribute("margin-right").value().ToString();
			if ("" == strMarginR)
				strMarginR = "15mm";

			std::string strMarginB = element.attribute("margin-bottom").value().ToString();
			if ("" == strMarginB)
				strMarginB = "20mm";

			PageWidth		=	strPageW;
			PageHeight		=	strPageH;
			m_numFormat		=	element.attribute("num-format").value();
			PrintOrientation	= element.attribute("print-orientation").value();
			MarginTop		=	strMarginT;
			MarginBottom	=	strMarginB;
			MarginLeft		=	strMarginL;
			MarginRight		=	strMarginR;
			Border			=	element.attribute("border").value();
			BorderLeft		=	element.attribute("border-left").value();
			BorderRight		=	element.attribute("border-right").value();
			BorderTop		=	element.attribute("border-top").value();
			BorderBottom	=	element.attribute("border-bottom").value();
			BackgroundColor	= element.attribute("background-color").value();
			m_writingMode	=	element.attribute("writing-mode").value();
			m_footnoteMaxHeight = element.attribute("footnote-max-height").value().ToString();
			m_footnoteSep = element.element("footnote-sep");
			Columns		    = element.element("columns");
			BackgroundImage = element.element("background-image");
		}


		const XML::XNode PageLayoutProperties::toXML() const
		{
			return 
				XML::XElement(ns.style + "page-layout-properties",
					XML::XAttribute(ns.fo + "page-width", PageWidth) +
					XML::XAttribute(ns.fo + "page-height", PageHeight) +
					XML::XAttribute(ns.style + "num-format", m_numFormat) +
					XML::XAttribute(ns.style + "print-orientation", PrintOrientation) +
					XML::XAttribute(ns.fo + "margin-top", MarginTop) +
					XML::XAttribute(ns.fo + "margin-bottom", MarginBottom) +
					XML::XAttribute(ns.fo + "margin-left", MarginLeft) +
					XML::XAttribute(ns.fo + "margin-right", MarginRight) +
					XML::XAttribute(ns.fo + "border", Border) +
					XML::XAttribute(ns.fo + "border-left", BorderLeft) +
					XML::XAttribute(ns.fo + "border-right", BorderRight) +
					XML::XAttribute(ns.fo + "border-top", BorderTop) + 
					XML::XAttribute(ns.fo + "border-bottom", BorderBottom) +
					XML::XAttribute(ns.fo + "background-color", BackgroundColor) +
					XML::XAttribute(ns.style + "writing-mode", m_writingMode) +
					XML::XAttribute(ns.style + "footnote-max-height", m_footnoteMaxHeight) +
					XML::Write(m_footnoteSep) +
					XML::Write(Columns) +
					XML::Write(BackgroundImage)
				);
		}

		const bool PageLayoutProperties::onlyBorder() const
		{
			return !(BorderLeft.is_init() || BorderRight.is_init() || BorderTop.is_init() || BorderBottom.is_init());
		}

		const bool PageLayoutProperties::hasBorders() const
		{
			return BorderLeft.is_init() || BorderRight.is_init() || BorderTop.is_init() || BorderBottom.is_init() || Border.is_init();
		}
	} // namespace Styles
} // namespace Odt