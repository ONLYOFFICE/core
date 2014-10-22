#pragma once
#ifndef PPTX_LOGIC_EFFECTLST_INCLUDE_H_
#define PPTX_LOGIC_EFFECTLST_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Effects/Blur.h"
#include "Effects/Glow.h"
#include "Effects/OuterShdw.h"
#include "Effects/PrstShdw.h"
#include "Effects/InnerShdw.h"
#include "Effects/Reflection.h"
#include "Effects/SoftEdge.h"
#include "Effects/FillOverlay.h"

namespace PPTX
{
	namespace Logic
	{

		class EffectLst : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(EffectLst)

            EffectLst& operator=(const EffectLst& oSrc);

		public:
            virtual void fromXML(XmlUtils::CXmlNode& node);

            virtual CString toXML() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECTPROPERTIES_TYPE_LIST);

				pWriter->WriteRecord2(0, blur);
				pWriter->WriteRecord2(1, fillOverlay);
				pWriter->WriteRecord2(2, glow);
				pWriter->WriteRecord2(3, innerShdw);
				pWriter->WriteRecord2(4, outerShdw);
				pWriter->WriteRecord2(5, prstShdw);
				pWriter->WriteRecord2(6, reflection);
				pWriter->WriteRecord2(7, softEdge);

				pWriter->EndRecord();
			}

		public:
			nullable<Blur> blur;
			nullable<FillOverlay> fillOverlay;
			nullable<Glow> glow;
			nullable<InnerShdw> innerShdw;
			nullable<OuterShdw> outerShdw;
			nullable<PrstShdw> prstShdw;
			nullable<Reflection> reflection;
			nullable<SoftEdge> softEdge;
		protected:
            virtual void FillParentPointersForChilds();

		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTLST_INCLUDE_H_
