#pragma once
#ifndef PPTX_LOGIC_SLIDE_GRPSPPR_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_GRPSPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/BWMode.h"
#include "Xfrm.h"
#include "UniFill.h"
#include "EffectProperties.h"
#include "Scene3d.h"

namespace PPTX
{
	namespace Logic
	{

		class GrpSpPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(GrpSpPr)

			GrpSpPr& operator=(const GrpSpPr& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				xfrm		= oSrc.xfrm;
				Fill		= oSrc.Fill;
				EffectList	= oSrc.EffectList;
				scene3d		= oSrc.scene3d;
				bwMode		= oSrc.bwMode;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"bwMode", bwMode);

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int count = oNodes.GetCount();
					for (int i = 0; i < count; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());
						if (_T("xfrm") == strName)
						{
							if (!xfrm.IsInit())
								xfrm = oNode;
						}
						else if (_T("scene3d") == strName)
						{
							if (!scene3d.IsInit())
								scene3d = oNode;
						}
					}
				}

				Fill.GetFillFrom(node);
				EffectList.GetEffectListFrom(node);

				FillParentPointersForChilds();
			}


			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("bwMode"), bwMode);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(xfrm);
				oValue.Write(Fill);
				oValue.Write(EffectList);
				oValue.WriteNullable(scene3d);

				return XmlUtils::CreateNode(_T("p:grpSpPr"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
					pWriter->StartNode(_T("wpg:grpSpPr"));
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->StartNode(_T("xdr:grpSpPr"));
				else
					pWriter->StartNode(_T("p:grpSpPr"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("bwMode"), bwMode);
				pWriter->EndAttributes();

				pWriter->Write(xfrm);
				Fill.toXmlWriter(pWriter);
				EffectList.toXmlWriter(pWriter);
				pWriter->Write(scene3d);
				
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
					pWriter->EndNode(_T("wpg:grpSpPr"));
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->EndNode(_T("xdr:grpSpPr"));
				else
					pWriter->EndNode(_T("p:grpSpPr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, bwMode);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, xfrm);
				pWriter->WriteRecord1(1, Fill);
				pWriter->WriteRecord1(2, EffectList);
				pWriter->WriteRecord2(3, scene3d);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
					{
						bwMode = new Limit::BWMode();
						bwMode->SetBYTECode(pReader->GetUChar());
					}
					else
						break;
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							xfrm = new Xfrm();
							xfrm->fromPPTY(pReader);
							break;
						}
						case 1:
						{
							Fill.fromPPTY(pReader);
							break;
						}
						case 2:
						{
							pReader->SkipRecord();
							break;
						}
						case 3:
						{
							pReader->SkipRecord();
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			nullable<Xfrm>				xfrm;
			UniFill						Fill;
			EffectProperties			EffectList;
			nullable<Scene3d>			scene3d;

			nullable_limit<Limit::BWMode>	bwMode;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(xfrm.IsInit())
					xfrm->SetParentPointer(this);
				Fill.SetParentPointer(this);
				EffectList.SetParentPointer(this);
				if(scene3d.IsInit())
					scene3d->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_GRPSPPR_INCLUDE_H_
