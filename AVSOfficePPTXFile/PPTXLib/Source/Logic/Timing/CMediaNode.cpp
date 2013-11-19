#include "CMediaNode.h"

namespace PPTX
{
	namespace Logic
	{

		CMediaNode::CMediaNode()
		{
		}

		CMediaNode::~CMediaNode()
		{
		}

		CMediaNode::CMediaNode(const XML::XNode& node)
		{
			fromXML(node);
		}

		const CMediaNode& CMediaNode::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void CMediaNode::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cTn = element.element("cTn");
			tgtEl = element.element("tgtEl");

			mute = element.attribute("mute").value();
			numSld = element.attribute("numSld").value();
			showWhenStopped = element.attribute("showWhenStopped").value();
			vol = element.attribute("vol").value();

			FillParentPointersForChilds();
		}

		const XML::XNode CMediaNode::toXML() const
		{
			return XML::XElement(ns.p + "cMediaNode",
				XML::XAttribute("mute", mute) +
				XML::XAttribute("numSld", numSld) +
				XML::XAttribute("showWhenStopped", showWhenStopped) +
				XML::XAttribute("vol", vol) +
				XML::Write(cTn) +
				XML::Write(tgtEl)
			);
		}

		void CMediaNode::FillParentPointersForChilds()
		{
			cTn->SetParentPointer(*this);
			tgtEl->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX