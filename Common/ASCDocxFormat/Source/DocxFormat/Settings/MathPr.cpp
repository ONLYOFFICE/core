
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
		return XML::XElement();
		}

	} // namespace Settings
} // namespace OOX