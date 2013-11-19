#pragma once
#ifndef PPTX_LOGIC_CELL3D_INCLUDE_H_
#define PPTX_LOGIC_CELL3D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/Material.h"
#include "Bevel.h"
#include "LightRig.h"

namespace PPTX
{
	namespace Logic
	{
		class Cell3D : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Cell3D)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"prstMaterial", prstMaterial);

				bevel		= node.ReadNodeNoNS(_T("bevel"));
				lightRig	= node.ReadNode(_T("a:lightRig"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("prstMaterial"), prstMaterial);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(bevel);
				oValue.WriteNullable(lightRig);

				return XmlUtils::CreateNode(_T("a:cell3D"), oAttr, oValue);
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
			nullable_limit<Limit::Material>		prstMaterial;

			nullable<Bevel>						bevel;
			nullable<LightRig>					lightRig;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(bevel.IsInit())
					bevel->SetParentPointer(this);
				if(lightRig.IsInit())
					lightRig->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CELL3D_INCLUDE_H_