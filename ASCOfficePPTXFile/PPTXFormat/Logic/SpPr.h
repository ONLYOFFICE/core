#pragma once
#ifndef PPTX_LOGIC_SPPR_INCLUDE_H_
#define PPTX_LOGIC_SPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/BWMode.h"
#include "Xfrm.h"
#include "UniFill.h"
#include "Ln.h"
#include "EffectProperties.h"
#include "Scene3d.h"
#include "Sp3d.h"
#include "Geometry.h"

namespace PPTX
{
	namespace Logic
	{
		class SpPr : public WrapperWritingElement
		{
		public:
			SpPr();
			virtual ~SpPr();			
			explicit SpPr(XmlUtils::CXmlNode& node);
			const SpPr& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual CString toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
				{
					if (0 == (pWriter->m_lFlag & 0x01))
						pWriter->StartNode(_T("wps:spPr"));
					else
						pWriter->StartNode(_T("pic:spPr"));
				}
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->StartNode(_T("xdr:spPr"));
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART)
					pWriter->StartNode(_T("c:spPr"));
				else
				{
					if (0 != (pWriter->m_lFlag & 0x04))//theme
						pWriter->StartNode(_T("a:spPr"));
					else
						pWriter->StartNode(_T("p:spPr"));
				}
				
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("bwMode"), bwMode);
				pWriter->EndAttributes();
				
				pWriter->Write(xfrm);
				Geometry.toXmlWriter(pWriter);

				if ((pWriter->m_lFlag & 0x02) != 0 && !Fill.is_init())
				{
					pWriter->WriteString(_T("<a:grpFill/>"));
				}
				Fill.toXmlWriter(pWriter);
				
				pWriter->Write(ln);
				EffectList.toXmlWriter(pWriter);
				pWriter->Write(scene3d);
				pWriter->Write(sp3d);

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
				{
					if (0 == (pWriter->m_lFlag & 0x01))
						pWriter->EndNode(_T("wps:spPr"));
					else
						pWriter->EndNode(_T("pic:spPr"));
				}
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->EndNode(_T("xdr:spPr"));
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART)
					pWriter->EndNode(_T("c:spPr"));
				else
				{
					if (0 != (pWriter->m_lFlag & 0x04))//theme
						pWriter->EndNode(_T("a:spPr"));
					else
						pWriter->EndNode(_T("p:spPr"));
				}
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, bwMode);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, xfrm);
				pWriter->WriteRecord1(1, Geometry);
				pWriter->WriteRecord1(2, Fill);
				pWriter->WriteRecord2(3, ln);
				pWriter->WriteRecord1(4, EffectList);
				pWriter->WriteRecord2(5, scene3d);
				pWriter->WriteRecord2(6, sp3d);
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
							xfrm = new Logic::Xfrm();
							xfrm->fromPPTY(pReader);
							break;
						}
						case 1:
						{
							Geometry.fromPPTY(pReader);
							break;
						}
						case 2:
						{
							Fill.fromPPTY(pReader);
							break;
						}
						case 3:
						{
							ln = new Logic::Ln();
							ln->fromPPTY(pReader);
							break;
						}
						case 4:
						{
							pReader->SkipRecord();
							break;
						}
						case 5:
						{
							pReader->SkipRecord();
							break;
						}
						case 6:
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

			void Merge(SpPr& spPr)const;

		public:
			nullable<Xfrm>				xfrm;
			Logic::Geometry				Geometry;
			UniFill						Fill;
			nullable<Ln>				ln;
			EffectProperties			EffectList;
			nullable<Scene3d>			scene3d;
			nullable<Sp3d>				sp3d;

			nullable_limit<Limit::BWMode> bwMode;
		//private:
		public:
			CString m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SPPR_INCLUDE_H