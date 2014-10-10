#pragma once
#ifndef PPTX_LOGIC_EFFECTLST_INCLUDE_H_
#define PPTX_LOGIC_EFFECTLST_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Effects/Blur.h"
#include "Effects/Glow.h"
#include "Effects/OuterShdw.h"
#include "Effects/PrstShdw.h"
#include "Effects/InnerShdw.h"
#include "Effects/Reflection.h"
#include "Effects/SoftEdge.h"
#include "Effects/FillOverlay.h"

namespace PPTX
{
	namespace Logic
	{

		class EffectLst : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(EffectLst)

			EffectLst& operator=(const EffectLst& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				blur		= oSrc.blur;
				fillOverlay	= oSrc.fillOverlay;
				glow		= oSrc.glow;
				innerShdw	= oSrc.innerShdw;
				outerShdw	= oSrc.outerShdw;
				prstShdw	= oSrc.prstShdw;
				reflection	= oSrc.reflection;
				softEdge	= oSrc.softEdge;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
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

			virtual CString toXML() const
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

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECTPROPERTIES_TYPE_LIST);

				pWriter->WriteRecord2(0, blur);
				pWriter->WriteRecord2(1, fillOverlay);
				pWriter->WriteRecord2(2, glow);
				pWriter->WriteRecord2(3, innerShdw);
				pWriter->WriteRecord2(4, outerShdw);
				pWriter->WriteRecord2(5, prstShdw);
				pWriter->WriteRecord2(6, reflection);
				pWriter->WriteRecord2(7, softEdge);

				pWriter->EndRecord();
			}

		public:
			nullable<Blur> blur;
			nullable<FillOverlay> fillOverlay;
			nullable<Glow> glow;
			nullable<InnerShdw> innerShdw;
			nullable<OuterShdw> outerShdw;
			nullable<PrstShdw> prstShdw;
			nullable<Reflection> reflection;
			nullable<SoftEdge> softEdge;
		protected:
			virtual void EffectLst::FillParentPointersForChilds()
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
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTLST_INCLUDE_H_
