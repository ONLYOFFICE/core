#pragma once
#ifndef PPTX_LOGIC_ALPHABILEVEL_INCLUDE_H_
#define PPTX_LOGIC_ALPHABILEVEL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class AlphaBiLevel : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AlphaBiLevel)

			AlphaBiLevel& operator=(const AlphaBiLevel& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				thresh = oSrc.thresh;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"thresh", thresh);
				Normalize();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("thresh"), thresh);

				return XmlUtils::CreateNode(_T("a:alphaBiLevel"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_ALPHABILEVEL);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, thresh);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			nullable_int thresh;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				thresh.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ALPHABILEVEL_INCLUDE_H_