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
				oleInfo = oSrc.oleInfo;
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
			void writeOleStart(NSBinPptxRW::CXmlWriter *pWriter, NSShapeImageGen::COleInfo& oOleInfo, bool& bOle, CString& sOleProgID, CString& sOleNodeName)
			{
				if(oleInfo.IsInit())
					oOleInfo = oleInfo.get();
				if(!oOleInfo.m_sRid.IsEmpty() && !oOleInfo.m_sOleProperty.IsEmpty())
				{
					std::vector<CString> aOleProp;
					int nTokenPos = 0;
					CString strToken = oOleInfo.m_sOleProperty.Tokenize(_T("|"), nTokenPos);
					while (!strToken.IsEmpty())
					{
						aOleProp.push_back(strToken);
						strToken = oOleInfo.m_sOleProperty.Tokenize(_T("|"), nTokenPos);
					}
					if(3 == aOleProp.size())
					{
						bOle = true;
						CString dxaOrig = aOleProp[0];
						CString dyaOrig = aOleProp[1];
						sOleProgID = aOleProp[2];
						if(_T("0") != dxaOrig && _T("0") != dyaOrig)
						{
							sOleNodeName = _T("w:object");
							pWriter->StartNode(sOleNodeName);
							pWriter->StartAttributes();
							pWriter->WriteAttribute(_T("w:dxaOrig"), dxaOrig);
							pWriter->WriteAttribute(_T("w:dyaOrig"), dyaOrig);
							pWriter->EndAttributes();
						}
						else
						{
							sOleNodeName = _T("w:pict");
							pWriter->StartNode(sOleNodeName);
							pWriter->StartAttributes();
							pWriter->EndAttributes();
						}
					}
				}
			}
			void writeOleEnd(NSBinPptxRW::CXmlWriter *pWriter, const NSShapeImageGen::COleInfo& oOleInfo, const CString& strId, const CString& sOleProgID, const CString& sOleNodeName)
			{
				if(!sOleProgID.IsEmpty())
				{
					pWriter->StartNode(_T("o:OLEObject"));
					pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("Type"), CString(_T("Embed")));
					pWriter->WriteAttribute(_T("ProgID"), sOleProgID);
					pWriter->WriteAttribute(_T("ShapeID"), strId);
					pWriter->WriteAttribute(_T("DrawAspect"), CString(_T("Content")));
					CString sObjectID;
					sObjectID.Format(_T("_%010d"), pWriter->m_lObjectIdOle++);
					pWriter->WriteAttribute(_T("ObjectID"), sObjectID);
					pWriter->WriteAttribute(_T("r:id"), oOleInfo.m_sRid);
					pWriter->EndAttributes();
					pWriter->EndNode(_T("o:OLEObject"));

					pWriter->EndNode(sOleNodeName);
				}
			}

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