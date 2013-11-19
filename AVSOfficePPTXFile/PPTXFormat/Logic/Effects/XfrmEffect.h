#pragma once
#ifndef PPTX_LOGIC_XFRMEFFECT_INCLUDE_H_
#define PPTX_LOGIC_XFRMEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class XfrmEffect : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(XfrmEffect)

			XfrmEffect& operator=(const XfrmEffect& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				kx = oSrc.kx;
				ky = oSrc.ky;
				sx = oSrc.sx;
				sy = oSrc.sy;
				tx = oSrc.tx;
				ty = oSrc.ty;
				
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"kx", kx);
				node.ReadAttributeBase(L"ky", ky);
				node.ReadAttributeBase(L"sx", sx);
				node.ReadAttributeBase(L"sy", sy);
				node.ReadAttributeBase(L"tx", tx);
				node.ReadAttributeBase(L"ty", ty);

				Normalize();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("sx"), sx);
				oAttr.Write(_T("sy"), sy);
				oAttr.Write(_T("kx"), kx);
				oAttr.Write(_T("ky"), ky);
				oAttr.Write(_T("tx"), tx);
				oAttr.Write(_T("ty"), ty);
				
				return XmlUtils::CreateNode(_T("a:xfrm"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_XFRM);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, kx);
				pWriter->WriteInt2(1, ky);
				pWriter->WriteInt2(2, sx);
				pWriter->WriteInt2(3, sy);
				pWriter->WriteSize_t2(4, tx);
				pWriter->WriteSize_t2(5, tx);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			nullable_int	kx;
			nullable_int	ky;
			nullable_int	sx;
			nullable_int	sy;
			nullable_sizet	tx;
			nullable_sizet	ty;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				kx.normalize(-5400000, 5400000);
				ky.normalize(-5400000, 5400000);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_XFRMEFFECT_INCLUDE_H_