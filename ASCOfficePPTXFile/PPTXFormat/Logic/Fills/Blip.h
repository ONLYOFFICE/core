#pragma once
#ifndef PPTX_LOGIC_BLIP_INCLUDE_H_
#define PPTX_LOGIC_BLIP_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/BlipCompression.h"
#include "./../UniEffect.h"
#include "../../DocxFormat/RId.h"

namespace PPTX
{
	namespace Logic
	{
		class Blip : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Blip)

			Blip& operator=(const Blip& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Effects = oSrc.Effects;

				cstate	= oSrc.cstate;
				embed	= oSrc.embed;
				link	= oSrc.link;

				m_namespace	= oSrc.m_namespace;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual CString toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				CString strName = (_T("") == m_namespace) ? _T("blip") : (m_namespace + _T(":blip"));
				pWriter->StartNode(strName);

				pWriter->StartAttributes();
				if (embed.IsInit())
					pWriter->WriteAttribute(_T("r:embed"), embed->ToString());
				if (link.IsInit())
					pWriter->WriteAttribute(_T("r:link"), link->ToString());
				pWriter->WriteAttribute(_T("cstate"), cstate);
				pWriter->EndAttributes();

				// TODO:
				size_t nCount = Effects.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					Effects[i].toXmlWriter(pWriter);
				}

				pWriter->EndNode(strName);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, cstate);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				/*
				old version
				if (embed.is_init())
					embed->toPPTY(0, pWriter);
				if (link.is_init())
					link->toPPTY(1, pWriter);
				*/

				// new version
				if (embed.is_init())
					pWriter->WriteString1(10, embed->get());
				if (link.is_init())
					pWriter->WriteString1(11, link->get());

				pWriter->StartRecord(2);
				ULONG len = (ULONG)Effects.size();
				pWriter->WriteULONG(len);

				for (ULONG i = 0; i < len; ++i)
				{
					pWriter->WriteRecord1(3, Effects[i]);
				}

				pWriter->EndRecord();

				double dX = 0;
				double dY = 0;
				double dW = pWriter->GetWidthMM();
				double dH = pWriter->GetHeightMM();

				FileContainer* pRels = NULL;
				if (pWriter->m_pCommonRels->is_init())
					pRels = pWriter->m_pCommonRels->operator ->();

				NSShapeImageGen::COleInfo* pOleInfo = NULL;
				if(oleInfo.IsInit())
				{
					pOleInfo = oleInfo.GetPointer();
					pOleInfo->m_sFilename = this->GetFullOleName(PPTX::RId(oleInfo->m_sRid), pRels);
				}

				NSShapeImageGen::CImageInfo oId = pWriter->m_pCommon->m_pImageManager->WriteImage(this->GetFullPicName(pRels), pOleInfo, dX, dY, dW, dH);
				CString s = oId.GetPath2();

				pWriter->StartRecord(3);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, s);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);				

				pWriter->EndRecord();
			}
		public:
			virtual CString GetFullPicName(FileContainer* pRels = NULL)const;
			virtual CString GetFullOleName(const PPTX::RId& pRId, FileContainer* pRels = NULL)const;
		public:
			std::vector<UniEffect> Effects;

			nullable_limit<Limit::BlipCompression> cstate;
			nullable<PPTX::RId> embed;
			nullable<PPTX::RId> link;
		//private:
		public:
			CString m_namespace;
			nullable<NSShapeImageGen::COleInfo> oleInfo;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLIP_INCLUDE_H_