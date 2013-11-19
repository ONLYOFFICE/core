#pragma once
#ifndef PPTX_LOGIC_TILE_INCLUDE_H_
#define PPTX_LOGIC_TILE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/RectAlign.h"
#include "./../../Limit/Flip.h"

namespace PPTX
{
	namespace Logic
	{

		class Tile : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Tile)

			Tile& operator=(const Tile& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				algn	= oSrc.algn;
				flip	= oSrc.flip;
				sx		= oSrc.sx;
				sy		= oSrc.sy;
				tx		= oSrc.tx;
				ty		= oSrc.ty;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"algn", algn);
				node.ReadAttributeBase(L"flip", flip);
				node.ReadAttributeBase(L"sx", sx);
				node.ReadAttributeBase(L"sy", sy);
				node.ReadAttributeBase(L"tx", tx);
				node.ReadAttributeBase(L"ty", ty);
			}
			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("algn"), algn);
				oAttr.WriteLimitNullable(_T("flip"), flip);
				oAttr.Write(_T("sx"), sx);
				oAttr.Write(_T("sy"), sy);
				oAttr.Write(_T("tx"), tx);
				oAttr.Write(_T("ty"), ty);

				return XmlUtils::CreateNode(_T("a:tile"), oAttr);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tile"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("algn"), algn);
				pWriter->WriteAttribute(_T("flip"), flip);
				pWriter->WriteAttribute(_T("sx"), sx);
				pWriter->WriteAttribute(_T("sy"), sy);
				pWriter->WriteAttribute(_T("tx"), tx);
				pWriter->WriteAttribute(_T("ty"), ty);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:tile"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, sx);
				pWriter->WriteInt2(1, sy);
				pWriter->WriteInt2(2, tx);
				pWriter->WriteInt2(3, ty);
				pWriter->WriteLimit2(4, algn);
				pWriter->WriteLimit2(5, flip);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

		public:
			nullable_limit<Limit::RectAlign>	algn;
			nullable_limit<Limit::Flip>			flip;
			nullable_int						sx;
			nullable_int						sy;
			nullable_int						tx;
			nullable_int						ty;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TILE_INCLUDE_H_