#pragma once
#ifndef PPTX_LOGIC_EFFECTDAG_INCLUDE_H_
#define PPTX_LOGIC_EFFECTDAG_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "UniEffect.h"
#include "./../Limit/EffectContainerType.h"

namespace PPTX
{
	namespace Logic
	{

		class EffectDag : public WrapperWritingElement
		{
		public:
			
			PPTX_LOGIC_BASE(EffectDag)

			EffectDag& operator=(const EffectDag& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Effects = oSrc.Effects;
				name = oSrc.name;
				type = oSrc.type;
				m_name = oSrc.m_name;
				return *this;
			}

		public:
            virtual void fromXML(XmlUtils::CXmlNode& node);


            virtual CString toXML() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_DAG);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, name);
				pWriter->WriteLimit2(1, type);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->StartRecord(0);
				ULONG len = (ULONG)Effects.size();
				pWriter->WriteULONG(len);
				
				for (ULONG i = 0; i < len; ++i)
				{
					pWriter->WriteRecord1(1, Effects[i]);
				}

				pWriter->EndRecord();

				pWriter->EndRecord();
			}

		public:
			std::vector<UniEffect>						Effects;
			nullable_string								name;
			nullable_limit<Limit::EffectContainerType>	type;
			CString										m_name;
		protected:
            virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTDAG_INCLUDE_H_
