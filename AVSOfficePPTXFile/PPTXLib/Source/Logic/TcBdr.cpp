#include "TcBdr.h"

namespace PPTX
{
	namespace Logic
	{

		TcBdr::TcBdr()
		{
		}


		TcBdr::~TcBdr()
		{
		}
	

		TcBdr::TcBdr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TcBdr& TcBdr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TcBdr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			left = element.element("left");
			right = element.element("right");
			top = element.element("top");
			bottom = element.element("bottom");
			insideH = element.element("insideH");
			insideV = element.element("insideV");
			tl2br = element.element("tl2br");
			tr2bl = element.element("tr2bl");

			FillParentPointersForChilds();
		}


		const XML::XNode TcBdr::toXML() const
		{
			return XML::XElement(ns.a + "tcBdr",
					XML::Write(left) +
					XML::Write(right) +
					XML::Write(top) +
					XML::Write(bottom) +
					XML::Write(insideH) +
					XML::Write(insideV) +
					XML::Write(tl2br) +
					XML::Write(tr2bl)
				);
		}

		void TcBdr::FillParentPointersForChilds()
		{
			if(left.is_init())
				left->SetParentPointer(*this);
			if(right.is_init())
				right->SetParentPointer(*this);
			if(top.is_init())
				top->SetParentPointer(*this);
			if(bottom.is_init())
				bottom->SetParentPointer(*this);
			if(insideH.is_init())
				insideH->SetParentPointer(*this);
			if(insideV.is_init())
				insideV->SetParentPointer(*this);
			if(tl2br.is_init())
				tl2br->SetParentPointer(*this);
			if(tr2bl.is_init())
				tr2bl->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX