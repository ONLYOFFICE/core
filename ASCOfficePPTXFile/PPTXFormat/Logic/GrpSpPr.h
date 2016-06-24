/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
					BYTE _at = pReader->GetUChar_TypeNode();
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
							if (xfrm.is_init() == false)
							{
								xfrm = new Xfrm();
							}
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
