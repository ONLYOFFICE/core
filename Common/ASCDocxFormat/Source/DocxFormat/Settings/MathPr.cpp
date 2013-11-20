
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "MathPr.h"


namespace OOX
{
	namespace Settings
	{

		MathPr::MathPr()
		{
		}


		MathPr::~MathPr()
		{
		}


		MathPr::MathPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const MathPr& MathPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void MathPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_mathFont	=	element.element("mathFont").attribute("val").value();
			m_brkBin		=	element.element("brkBin").attribute("val").value();
			m_brkBinSub	=	element.element("brkBinSub").attribute("val").value(); 
			m_smallFrac	=	element.element("smallFrac").attribute("val").value();
			m_dispDef		=	element.element("dispDef").attribute("val").value();
			m_lMargin		=	element.element("lMargin").attribute("val").value();
			m_rMargin		=	element.element("rMargin").attribute("val").value();
			m_defJc			=	element.element("defJc").attribute("val").value();
			m_wrapIndent= element.element("wrapIndent").attribute("val").value();
			m_intLim		=	element.element("intLim").attribute("val").value();
			m_naryLim		=	element.element("naryLim").attribute("val").value();

		}


		const XML::XNode MathPr::toXML() const
		{
			return 
				XML::XElement(ns.m + "mathPr",
					XML::XElement(ns.m + "mathFont", XML::XAttribute(ns.m + "val", m_mathFont)) +
					XML::XElement(ns.m + "brkBin", XML::XAttribute(ns.m + "val", m_brkBin)) +
					XML::XElement(ns.m + "brkBinSub", XML::XAttribute(ns.m + "val", m_brkBinSub)) +
					XML::XElement(ns.m + "smallFrac", XML::XAttribute(ns.m + "val", m_smallFrac)) + 
					XML::XElement(ns.m + "dispDef", XML::XAttribute(ns.m + "val", m_dispDef)) +
					XML::XElement(ns.m + "lMargin", XML::XAttribute(ns.m + "val", m_lMargin)) +
					XML::XElement(ns.m + "rMargin", XML::XAttribute(ns.m + "val", m_rMargin)) +
					XML::XElement(ns.m + "defJc", XML::XAttribute(ns.m + "val", m_defJc)) +
					XML::XElement(ns.m + "wrapIndent", XML::XAttribute(ns.m + "val", m_wrapIndent)) + 
					XML::XElement(ns.m + "intLim", XML::XAttribute(ns.m + "val", m_intLim)) +
					XML::XElement(ns.m + "naryLim", XML::XAttribute(ns.m + "val", m_naryLim))
				);
		}

	} // namespace Settings
} // namespace OOX