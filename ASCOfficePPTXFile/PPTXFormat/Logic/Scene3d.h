#pragma once
#ifndef PPTX_LOGIC_SCENE3D_INCLUDE_H_
#define PPTX_LOGIC_SCENE3D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Camera.h"
#include "LightRig.h"
#include "Backdrop.h"

namespace PPTX
{
	namespace Logic
	{

		class Scene3d : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Scene3d)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				camera		= node.ReadNode(_T("a:camera"));
				lightRig	= node.ReadNode(_T("a:lightRig"));
				backdrop	= node.ReadNode(_T("a:backdrop"));
				
				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(camera);
				oValue.Write(lightRig);
				oValue.WriteNullable(backdrop);

				return XmlUtils::CreateNode(_T("a:scene3d"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				// TODO:
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				// TODO:	
			}

		public:
			Camera				camera;
			LightRig			lightRig;
			nullable<Backdrop>	backdrop;
		protected:
			virtual void FillParentPointersForChilds()
			{
				camera.SetParentPointer(this);
				lightRig.SetParentPointer(this);
				if(backdrop.IsInit())
					backdrop->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SCENE3D_INCLUDE_H_