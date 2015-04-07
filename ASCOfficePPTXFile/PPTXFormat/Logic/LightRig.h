#pragma once
#ifndef PPTX_LOGIC_LIGHTRIG_INCLUDE_H_
#define PPTX_LOGIC_LIGHTRIG_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Rot.h"
#include "./../Limit/LightRigType.h"
#include "./../Limit/RectAlign.h"

namespace PPTX
{
	namespace Logic
	{

		class LightRig : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(LightRig)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				dir = node.GetAttribute(_T("dir"));
				rig = node.GetAttribute(_T("rig"));

                CString sRotNodeName = _T("a:rot");
                rot = node.ReadNode(sRotNodeName);
				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("dir"), dir.get());
				oAttr.Write(_T("rig"), rig.get());

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(rot);

				return XmlUtils::CreateNode(_T("a:lightRig"), oAttr, oValue);
			}
		public:
			nullable<Rot>		rot;

			Limit::RectAlign	dir;
			Limit::LightRigType rig;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(rot.IsInit())
					rot->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LIGHTRIG_INCLUDE_H_
