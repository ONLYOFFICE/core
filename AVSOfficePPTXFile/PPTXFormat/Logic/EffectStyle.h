#pragma once
#ifndef PPTX_LOGIC_EFFECTSTYLE_INCLUDE_H_
#define PPTX_LOGIC_EFFECTSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "EffectProperties.h"
#include "Scene3d.h"
#include "Sp3d.h"

namespace PPTX
{
	namespace Logic
	{

		class EffectStyle : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(EffectStyle)

			EffectStyle& operator=(const EffectStyle& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				EffectList = oSrc.EffectList;
				scene3d = oSrc.scene3d;
				sp3d = oSrc.sp3d;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				EffectList.GetEffectListFrom(node);
				scene3d = node.ReadNode(_T("a:scene3d"));
				sp3d	= node.ReadNode(_T("a:sp3d"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(EffectList);
				oValue.WriteNullable(scene3d);
				oValue.WriteNullable(sp3d);

				return XmlUtils::CreateNode(_T("a:effectStyle"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:effectStyle"));
				pWriter->EndAttributes();

				EffectList.toXmlWriter(pWriter);
				pWriter->Write(scene3d);
				pWriter->Write(sp3d);

				pWriter->EndNode(_T("a:effectStyle"));
			}

		public:
			EffectProperties	EffectList;
			nullable<Scene3d>	scene3d;
			nullable<Sp3d>		sp3d;
		protected:
			virtual void FillParentPointersForChilds()
			{
				EffectList.SetParentPointer(this);
				if(scene3d.IsInit())
					scene3d->SetParentPointer(this);
				if(sp3d.IsInit())
					sp3d->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTSTYLE_INCLUDE_H_