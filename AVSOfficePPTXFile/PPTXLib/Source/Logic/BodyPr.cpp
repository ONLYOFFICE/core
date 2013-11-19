#include "BodyPr.h"

namespace PPTX
{
	namespace Logic
	{

		BodyPr::BodyPr()
		{
		}


		BodyPr::~BodyPr()
		{
		}
	

		BodyPr::BodyPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BodyPr& BodyPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BodyPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_namespace = element.XName.get().Ns;

			anchor = element.attribute("anchor").value();
			anchorCtr = element.attribute("anchorCtr").value();
			bIns = element.attribute("bIns").value();
			compatLnSpc = element.attribute("compatLnSpc").value();
			forceAA = element.attribute("forceAA").value();
			fromWordArt = element.attribute("fromWordArt").value();
			horzOverflow = element.attribute("horzOverflow").value();
			lIns = element.attribute("lIns").value();
			numCol = element.attribute("numCol").value();
			rIns = element.attribute("rIns").value();
			rot = element.attribute("rot").value();
			rtlCol = element.attribute("rtlCol").value();
			spcCol = element.attribute("spcCol").value();
			spcFirstLastPara = element.attribute("spcFirstLastPara").value();
			tIns = element.attribute("tIns").value();
			upright = element.attribute("upright").value();
			vert = element.attribute("vert").value();
			vertOverflow = element.attribute("vertOverflow").value();
			wrap = element.attribute("wrap").value();

			Fit.GetTextFitFrom(element);
			prstTxWarp = element.element("prstTxWarp");
			scene3d = element.element("scene3d");
			if(element.element("flatTx").exist())
				flatTx = element.element("flatTx").attribute("z").value();
			sp3d = element.element("sp3d");

			FillParentPointersForChilds();
		}


		const XML::XNode BodyPr::toXML() const
		{
			return XML::XElement(m_namespace + "bodyPr",
				XML::XAttribute("rot", rot) +
				XML::XAttribute("spcFirstLastPara", spcFirstLastPara) +
				XML::XAttribute("vertOverflow", vertOverflow) +
				XML::XAttribute("horzOverflow", horzOverflow) +
				XML::XAttribute("vert", vert) +
				XML::XAttribute("wrap", wrap) +
				XML::XAttribute("lIns", lIns) +
				XML::XAttribute("tIns", tIns) +
				XML::XAttribute("rIns", rIns) +
				XML::XAttribute("bIns", bIns) +
				XML::XAttribute("numCol", numCol) +
				XML::XAttribute("spcCol", spcCol) +
				XML::XAttribute("rtlCol", rtlCol) +
				XML::XAttribute("fromWordArt", fromWordArt) +
				XML::XAttribute("anchor", anchor) +
				XML::XAttribute("anchorCtr", anchorCtr) +
				XML::XAttribute("forceAA", forceAA) +
				XML::XAttribute("upright", upright) +
				XML::XAttribute("compatLnSpc", compatLnSpc) +
				XML::Write(prstTxWarp) +
				XML::Write(Fit) +
				XML::Write(scene3d) +
				XML::WriteIf(XML::XElement(m_namespace + "flatTx", XML::XAttribute("z", flatTx)), flatTx.is_init()) +
				XML::Write(sp3d)
			);
		}

		void BodyPr::FillParentPointersForChilds()
		{
			if(prstTxWarp.is_init())
				prstTxWarp->SetParentPointer(*this);
			Fit.SetParentPointer(*this);
			if(scene3d.is_init())
				scene3d->SetParentPointer(*this);
		}

		void BodyPr::Merge(BodyPr& bodyPr)const
		{
			if(*Fit.type != TextFit::FitEmpty)
				Fit.Merge(bodyPr.Fit);
/*
			nullable_property<PrstTxWarp> prstTxWarp;
			nullable_property<Scene3d> scene3d;
			nullable_property<int> flatTx;
			nullable_property<Sp3d> sp3d;
*/
			// Attributes
			if(anchor.is_init())
				bodyPr.anchor = *anchor;
			if(anchorCtr.is_init())
				bodyPr.anchorCtr = *anchorCtr;
			if(bIns.is_init())
				bodyPr.bIns = *bIns;
			if(compatLnSpc.is_init())
				bodyPr.compatLnSpc = *compatLnSpc;
			if(forceAA.is_init())
				bodyPr.forceAA = *forceAA;
			if(fromWordArt.is_init())
				bodyPr.fromWordArt = *fromWordArt;
			if(horzOverflow.is_init())
				bodyPr.horzOverflow = *horzOverflow;
			if(lIns.is_init())
				bodyPr.lIns = *lIns;
			if(numCol.is_init())
				bodyPr.numCol = *numCol;
			if(rIns.is_init())
				bodyPr.rIns = *rIns;
			if(rot.is_init())
				bodyPr.rot = *rot;
			if(rtlCol.is_init())
				bodyPr.rtlCol = *rtlCol;
			if(spcCol.is_init())
				bodyPr.spcCol = *spcCol;
			if(spcFirstLastPara.is_init())
				bodyPr.spcFirstLastPara = *spcFirstLastPara;
			if(tIns.is_init())
				bodyPr.tIns = *tIns;
			if(upright.is_init())
				bodyPr.upright = *upright;
			if(vert.is_init())
				bodyPr.vert = *vert;
			if(vertOverflow.is_init())
				bodyPr.vertOverflow = *vertOverflow;
			if(wrap.is_init())
				bodyPr.wrap = *wrap;
		}
	} // namespace Logic
} // namespace PPTX