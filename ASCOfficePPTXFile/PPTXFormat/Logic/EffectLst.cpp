#include "EffectLst.h"

namespace PPTX
{
	namespace Logic
	{
		EffectLst::EffectLst()
		{
		}

		EffectLst::~EffectLst()
		{
		}

		EffectLst::EffectLst(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const EffectLst& EffectLst::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}

		void EffectLst::fromXML(XmlUtils::CXmlNode& node)
		{
			blur		= node.ReadNode(_T("a:blur"));
			fillOverlay = node.ReadNode(_T("a:fillOverlay"));
			glow		= node.ReadNode(_T("a:glow"));
			innerShdw	= node.ReadNode(_T("a:innerShdw"));
			outerShdw	= node.ReadNode(_T("a:outerShdw"));
			prstShdw	= node.ReadNode(_T("a:prstShdw"));
			reflection	= node.ReadNode(_T("a:reflection"));
			softEdge	= node.ReadNode(_T("a:softEdge"));

			FillParentPointersForChilds();
		}

		CString EffectLst::toXML() const
		{
			CString str = _T("<a:effectLst>");
			if (blur.IsInit())			str += blur->toXML();
			if (fillOverlay.IsInit())	str += fillOverlay->toXML();
			if (glow.IsInit())			str += glow->toXML();
			if (innerShdw.IsInit())		str += innerShdw->toXML();
			if (outerShdw.IsInit())		str += outerShdw->toXML();
			if (prstShdw.IsInit())		str += prstShdw->toXML();
			if (reflection.IsInit())	str += reflection->toXML();
			if (softEdge.IsInit())		str += softEdge->toXML();

			str += _T("</a:effectLst>");
			return str;
		}

		void EffectLst::FillParentPointersForChilds()
		{
			if(blur.IsInit())
				blur->SetParentPointer(this);
			if(fillOverlay.IsInit())
				fillOverlay->SetParentPointer(this);
			if(glow.IsInit())
				glow->SetParentPointer(this);
			if(innerShdw.IsInit())
				innerShdw->SetParentPointer(this);
			if(outerShdw.IsInit())
				outerShdw->SetParentPointer(this);
			if(prstShdw.IsInit())
				prstShdw->SetParentPointer(this);
			if(reflection.IsInit())
				reflection->SetParentPointer(this);
			if(softEdge.IsInit())
				softEdge->SetParentPointer(this);
		}

	} // namespace Logic
} // namespace PPTX