#pragma once
#ifndef PPTX_LOGIC_CAMERA_INCLUDE_H_
#define PPTX_LOGIC_CAMERA_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Rot.h"
#include "./../Limit/CameraType.h"

namespace PPTX
{
	namespace Logic
	{

		class Camera : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Camera)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				prst	= node.GetAttribute(_T("prst"));
				node.ReadAttributeBase(L"fov", fov);
				node.ReadAttributeBase(L"zoom", zoom);

				rot		= node.ReadNode(_T("a:rot"));
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("prst"), prst.get());
				oAttr.Write(_T("fov"), fov);
				oAttr.Write(_T("zoom"), zoom);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(rot);

				return XmlUtils::CreateNode(_T("a:camera"), oAttr, oValue);
			}
		public:
			nullable<Rot>		rot;

			Limit::CameraType	prst;
			nullable_int		fov;
			nullable_int		zoom;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(rot.IsInit())
					rot->SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				fov.normalize(0, 10800000);
				zoom.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CAMERA_INCLUDE_H_