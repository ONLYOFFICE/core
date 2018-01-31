﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef PPTX_LOGIC_CXNSP_INCLUDE_H_
#define PPTX_LOGIC_CXNSP_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "NvCxnSpPr.h"
#include "SpPr.h"
#include "ShapeStyle.h"


namespace PPTX
{
	namespace Logic
	{
		class CxnSp : public WrapperWritingElement
		{
		public:
			CxnSp(std::wstring ns = L"p");
			virtual ~CxnSp();			
			
			explicit CxnSp(XmlUtils::CXmlNode& node);
			const CxnSp& operator =(XmlUtils::CXmlNode& node);

			explicit CxnSp(XmlUtils::CXmlLiteReader& oReader);
			const CxnSp& operator =(XmlUtils::CXmlLiteReader& oReader);

			virtual OOX::EElementType getType () const
			{
				return OOX::et_cxnSp;
			}

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			
			virtual std::wstring toXML() const;
			virtual void GetRect(Aggplus::RECT& pRect)const;

			DWORD GetLine(Ln& line)const;
			DWORD GetFill(UniFill& fill)const;

			//void FillLevelUp();
			//void Merge(CxnSp& cxnSp, bool bIsSlidePlaceholder = false);

			//void SetLevelUpElement( CxnSp* p){m_pLevelUp = p;};

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(SPTREE_TYPE_CXNSP);

				pWriter->WriteRecord1(0, nvCxnSpPr);
				pWriter->WriteRecord1(1, spPr);
				pWriter->WriteRecord2(2, style);

				pWriter->EndRecord();
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				std::wstring namespace_ = m_namespace;

                if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)	namespace_ = L"wps";
                else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)	namespace_ = L"xdr";
                    
				pWriter->StartNode(namespace_ + L":cxnSp");

                pWriter->EndAttributes();

                nvCxnSpPr.toXmlWriter(pWriter);
                spPr.toXmlWriter(pWriter);

                if (style.is_init())
                {
                    if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
                        style->m_namespace = _T("wps");
                    else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
                        style->m_namespace = _T("xdr");

                    pWriter->Write(style);
                }
				pWriter->EndNode(namespace_ + L":cxnSp");
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							nvCxnSpPr.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							spPr.fromPPTY(pReader);
							break;
						}
						case 2:
						{
							style = new ShapeStyle(L"p");
							style->fromPPTY(pReader);
							break;
						}
						default:
						{
							break;
						}
					}
				}

				pReader->Seek(_end_rec);
			}

			std::wstring			m_namespace;

			NvCxnSpPr				nvCxnSpPr;
			SpPr					spPr;
			nullable<ShapeStyle>	style;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CXNSP_INCLUDE_H
