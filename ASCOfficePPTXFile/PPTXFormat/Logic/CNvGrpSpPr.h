#pragma once
#ifndef PPTX_LOGIC_CNVGRPSPPR_INCLUDE_H_
#define PPTX_LOGIC_CNVGRPSPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class CNvGrpSpPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CNvGrpSpPr)

            CNvGrpSpPr&         operator=(const CNvGrpSpPr& oSrc);

        public:
            virtual void fromXML(XmlUtils::CXmlNode& node);

            virtual CString toXML() const;


			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (!noChangeAspect.is_init() &&
					!noGrp.is_init() &&
					!noMove.is_init() &&
					!noResize.is_init() &&
					!noRot.is_init() &&
					!noSelect.is_init() &&
					!noUngrp.is_init())
				{
					if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
						pWriter->WriteString(_T("<xdr:cNvGrpSpPr/>"));
					else
						pWriter->WriteString(_T("<p:cNvGrpSpPr/>"));
					return;
				}

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->WriteString(_T("<xdr:cNvGrpSpPr>"));
				else
					pWriter->WriteString(_T("<p:cNvGrpSpPr>"));

				pWriter->StartNode(_T("a:grpSpLocks"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("noChangeAspect"), noChangeAspect);
				pWriter->WriteAttribute(_T("noGrp"), noGrp);
				pWriter->WriteAttribute(_T("noMove"), noMove);
				pWriter->WriteAttribute(_T("noResize"), noResize);
				pWriter->WriteAttribute(_T("noRot"), noRot);
				pWriter->WriteAttribute(_T("noSelect"), noSelect);
				pWriter->WriteAttribute(_T("noUngrp"), noUngrp);
				pWriter->EndAttributes();
				
				pWriter->EndNode(_T("a:grpSpLocks"));

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->WriteString(_T("</xdr:cNvGrpSpPr>"));
				else
					pWriter->WriteString(_T("</p:cNvGrpSpPr>"));
			}

			void toXmlWriter2(const CString& strNS, NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (!noChangeAspect.is_init() &&
					!noGrp.is_init() &&
					!noMove.is_init() &&
					!noResize.is_init() &&
					!noRot.is_init() &&
					!noSelect.is_init() &&
					!noUngrp.is_init())
				{
					pWriter->WriteString(_T("<") + strNS + _T(":cNvGrpSpPr/>"));
					return;
				}

				pWriter->StartNode(strNS + _T(":cNvGrpSpPr"));
				pWriter->StartNode(_T("a:grpSpLocks"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("noChangeAspect"), noChangeAspect);
				pWriter->WriteAttribute(_T("noGrp"), noGrp);
				pWriter->WriteAttribute(_T("noMove"), noMove);
				pWriter->WriteAttribute(_T("noResize"), noResize);
				pWriter->WriteAttribute(_T("noRot"), noRot);
				pWriter->WriteAttribute(_T("noSelect"), noSelect);
				pWriter->WriteAttribute(_T("noUngrp"), noUngrp);
				pWriter->EndAttributes();
				
				pWriter->EndNode(_T("a:grpSpLocks"));
				pWriter->EndNode(strNS + _T(":cNvGrpSpPr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, noChangeAspect);
				pWriter->WriteBool2(1, noGrp);
				pWriter->WriteBool2(2, noMove);
				pWriter->WriteBool2(3, noResize);
				pWriter->WriteBool2(4, noRot);
				pWriter->WriteBool2(5, noSelect);
				pWriter->WriteBool2(6, noUngrp);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							noChangeAspect = pReader->GetBool();
							break;
						}
						case 1:
						{
							noGrp = pReader->GetBool();
							break;
						}
						case 2:
						{
							noMove = pReader->GetBool();
							break;
						}
						case 3:
						{
							noResize = pReader->GetBool();
							break;
						}
						case 4:
						{
							noRot = pReader->GetBool();
							break;
						}
						case 5:
						{
							noSelect = pReader->GetBool();
							break;
						}
						case 6:
						{
							noUngrp = pReader->GetBool();
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			nullable_bool		noChangeAspect;
			nullable_bool		noGrp;
			nullable_bool		noMove;
			nullable_bool		noResize;
			nullable_bool		noRot;
			nullable_bool		noSelect;
			nullable_bool		noUngrp;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CNVGRPSPPR_INCLUDE_H
