#include "EffectStyle.h"

namespace PPTX
{
	namespace Logic
	{

		void EffectStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			EffectList.GetEffectListFrom(node);
			scene3d = node.ReadNode(_T("a:scene3d"));
			sp3d	= node.ReadNode(_T("a:sp3d"));

			FillParentPointersForChilds();
		}


		CString EffectStyle::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(EffectList);
			oValue.WriteNullable(scene3d);
			oValue.WriteNullable(sp3d);

			return XmlUtils::CreateNode(_T("a:effectStyle"), oValue);
		}

		void EffectStyle::FillParentPointersForChilds()
		{
			EffectList.SetParentPointer(this);
			if(scene3d.IsInit())
				scene3d->SetParentPointer(this);
			if(sp3d.IsInit())
				sp3d->SetParentPointer(this);
		}

	} // namespace Logic
} // namespace PPTX
