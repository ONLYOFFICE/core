#pragma once
#ifndef PPTX_LOGIC_PRSTSHDW_INCLUDE_H_
#define PPTX_LOGIC_PRSTSHDW_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"
#include "./../../Limit/PresetShadowVal.h"

namespace PPTX
{
	namespace Logic
	{

		class PrstShdw : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(PrstShdw)

			PrstShdw& operator=(const PrstShdw& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Color	= oSrc.Color;

				prst	= oSrc.prst;
				dir		= oSrc.dir;
				dist	= oSrc.dist;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Color.GetColorFrom(node);
				prst	= node.GetAttribute(L"prst");
				node.ReadAttributeBase(L"dir", dir);
				node.ReadAttributeBase(L"dist", dist);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("prst"), prst.get());
				oAttr.Write(_T("dist"), dist);
				oAttr.Write(_T("dir"), dir);

				XmlUtils::CNodeValue oValue;
				oValue.Write(Color);

				return XmlUtils::CreateNode(_T("a:prstShdw"), oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_PRSTSHDW);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, dir);
				pWriter->WriteInt2(1, dist);
				pWriter->WriteLimit1(2, prst);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, Color);

				pWriter->EndRecord();
			}

		public:
			UniColor Color;

			Limit::PresetShadowVal	prst;
			nullable_int			dir;
			nullable_int			dist;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Color.SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				dir.normalize(0, 21600000);
				dist.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PRSTSHDW_INCLUDE_H_