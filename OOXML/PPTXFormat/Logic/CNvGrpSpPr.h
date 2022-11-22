/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
			WritingElement_AdditionConstructors(CNvGrpSpPr)

			CNvGrpSpPr(std::wstring ns = L"p")
			{
				m_namespace = ns;
			}

            CNvGrpSpPr& operator=(const CNvGrpSpPr& oSrc);

            virtual void fromXML(XmlUtils::CXmlNode& node);
            virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

            virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				std::wstring namespace_ = m_namespace;
				
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)					namespace_ = L"xdr";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		namespace_ = L"a";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	namespace_ = L"cdr";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			namespace_ = L"dgm";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		namespace_ = L"dsp";

				if (!noChangeAspect.is_init() &&
					!noGrp.is_init() &&
					!noMove.is_init() &&
					!noResize.is_init() &&
					!noRot.is_init() &&
					!noSelect.is_init() &&
					!noUngrp.is_init())
				{
					pWriter->WriteString(L"<" + namespace_ + L":cNvGrpSpPr/>");
					return;
				}

				pWriter->WriteString(L"<" + namespace_ + L":cNvGrpSpPr>");

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

				pWriter->WriteString(L"</" + namespace_ + L":cNvGrpSpPr>");
			}

			void toXmlWriter2(const std::wstring& strNS, NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (!noChangeAspect.is_init() &&
					!noGrp.is_init() &&
					!noMove.is_init() &&
					!noResize.is_init() &&
					!noRot.is_init() &&
					!noSelect.is_init() &&
					!noUngrp.is_init())
				{
					pWriter->WriteString(L"<" + strNS + L":cNvGrpSpPr/>");
					return;
				}

				pWriter->StartNode(strNS + L":cNvGrpSpPr");
					pWriter->EndAttributes();
				pWriter->StartNode(L"a:grpSpLocks");

				pWriter->StartAttributes();
				pWriter->WriteAttribute(L"noChangeAspect", noChangeAspect);
				pWriter->WriteAttribute(L"noGrp", noGrp);
				pWriter->WriteAttribute(L"noMove", noMove);
				pWriter->WriteAttribute(L"noResize", noResize);
				pWriter->WriteAttribute(L"noRot", noRot);
				pWriter->WriteAttribute(L"noSelect", noSelect);
				pWriter->WriteAttribute(L"noUngrp", noUngrp);
				pWriter->EndAttributes();
				
				pWriter->EndNode(L"a:grpSpLocks");
				pWriter->EndNode(strNS + L":cNvGrpSpPr");
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
				LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

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
			std::wstring		m_namespace;

			nullable_bool		noChangeAspect;
			nullable_bool		noGrp;
			nullable_bool		noMove;
			nullable_bool		noResize;
			nullable_bool		noRot;
			nullable_bool		noSelect;
			nullable_bool		noUngrp;
		protected:
			void ReadAttributesLocks(XmlUtils::CXmlLiteReader& oReader);
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CNVGRPSPPR_INCLUDE_H
