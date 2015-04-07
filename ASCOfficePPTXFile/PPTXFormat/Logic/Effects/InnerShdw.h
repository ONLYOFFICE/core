#pragma once
#ifndef PPTX_LOGIC_INNERSHDW_INCLUDE_H_
#define PPTX_LOGIC_INNERSHDW_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class InnerShdw : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(InnerShdw)

			InnerShdw& operator=(const InnerShdw& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Color	= oSrc.Color;
				blurRad	= oSrc.blurRad;
				dir		= oSrc.dir;
				dist	= oSrc.dist;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Color.GetColorFrom(node);
				node.ReadAttributeBase(L"blurRad", blurRad);
				node.ReadAttributeBase(L"dir", dir);
				node.ReadAttributeBase(L"dist", dist);

				FillParentPointersForChilds();
				Normalize();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("blurRad"), blurRad);
				oAttr.Write(_T("dist"), dist);
				oAttr.Write(_T("dir"), dir);

				return XmlUtils::CreateNode(_T("a:innerShdw"), oAttr, Color.toXML());
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_INNERSHDW);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, dir);
				pWriter->WriteInt2(1, dist);
				pWriter->WriteInt2(2, blurRad);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, Color);

				pWriter->EndRecord();
			}

		public:
			UniColor Color;

			nullable_int blurRad;
			nullable_int dir;
			nullable_int dist;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Color.SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				blurRad.normalize_positive();
				dist.normalize_positive();

				dir.normalize(0, 21600000);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_INNERSHDW_INCLUDE_H_