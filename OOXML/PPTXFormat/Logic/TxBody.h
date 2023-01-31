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
#ifndef PPTX_LOGIC_TXBODY_INCLUDE_H_
#define PPTX_LOGIC_TXBODY_INCLUDE_H_

#include "./../WrapperWritingElement.h"

#include "BodyPr.h"
#include "TextListStyle.h"
#include "Paragraph.h"
#include "ShapeStyle.h"

namespace PPTX
{
	namespace Logic
	{
		class TxBody : public WrapperWritingElement
		{
		public:
			TxBody(std::wstring name = L"p:txBody")	
			{
				m_name = name;
			}
			virtual ~TxBody() {}
			explicit TxBody(XmlUtils::CXmlNode& node)	
			{
				fromXML(node); 
			}
			const TxBody& operator =(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
				return *this;
			}
			explicit TxBody(XmlUtils::CXmlLiteReader& oReader)	
			{
				fromXML(oReader); 
			}
			const TxBody& operator =(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML(oReader);
				return *this;
			}
			TxBody(const TxBody& oSrc) { *this = oSrc; }

			TxBody& operator=(const TxBody& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				sp3d		= oSrc.sp3d;
				bodyPr		= oSrc.bodyPr;
				lstStyle	= oSrc.lstStyle;
				Paragrs		= oSrc.Paragrs;

				m_name		= oSrc.m_name;

				return *this;
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_name = oReader.GetName();

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    std::wstring strName = oReader.GetName();
					if (L"a:bodyPr" == strName)
					{
						bodyPr = oReader;
					}
					else if (L"a:lstStyle" == strName)
					{
						lstStyle = oReader;
					}
					else if (L"a:sp3d" == strName)
					{
						sp3d = oReader;
					}					
					else if (L"a:p" == strName)
					{
						Paragraph p;
						Paragrs.push_back(p);
						Paragrs.back().fromXML(oReader);
					}
				}
				FillParentPointersForChilds();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Paragrs.clear();

				m_name		= node.GetName();
				
				bodyPr		= node.ReadNode(L"a:bodyPr");
				lstStyle	= node.ReadNode(L"a:lstStyle");
				sp3d		= node.ReadNode(L"a:sp3d");

				XmlMacroLoadArray(node, L"a:p", Paragrs, Paragraph);

				FillParentPointersForChilds();
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CNodeValue oValue;
				
				oValue.WriteNullable(bodyPr);
				oValue.WriteNullable(lstStyle);
				oValue.WriteArray(Paragrs);

				return XmlUtils::CreateNode(m_name, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);
				pWriter->EndAttributes();

				if (bodyPr.IsInit())
				{
					bodyPr->m_namespace = L"a";
					bodyPr->toXmlWriter(pWriter);
				}
				if (sp3d.IsInit())
				{
					sp3d->toXmlWriter(pWriter);
				}
				if (lstStyle.IsInit())
					lstStyle->m_name = L"a:lstStyle";
				pWriter->Write(lstStyle);
				
				size_t nCount = Paragrs.size();
				for (size_t i = 0; i < nCount; ++i)
					Paragrs[i].toXmlWriter(pWriter);
				
				pWriter->EndNode(m_name);
			}

			void toXmlWriterExcel(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (bodyPr.IsInit())
				{
					bodyPr->m_namespace = L"a";
					bodyPr->toXmlWriter(pWriter);
				}
				if (sp3d.IsInit())
				{
					sp3d->toXmlWriter(pWriter);
				}
				if (lstStyle.is_init())
					lstStyle->m_name = L"a:lstStyle";
				pWriter->Write(lstStyle);
				
				size_t nCount = Paragrs.size();
				for (size_t i = 0; i < nCount; ++i)
					Paragrs[i].toXmlWriter(pWriter);
				
				/*
				pWriter->EndNode(L"c:rich"));
				*/
			}

			std::wstring GetText(bool bParagraphSeparator = true)const
			{
				std::wstring result;
				size_t count = Paragrs.size();

				for (size_t i = 0; i < count; ++i)
					result += Paragrs[i].GetText(bParagraphSeparator);
				return result;
			}

            void Merge(nullable<TxBody>& txBody)
			{
                if (!bodyPr.IsInit())
                    bodyPr = new Logic::BodyPr();

                bodyPr->Merge(txBody->bodyPr);

				if(lstStyle.IsInit())
					lstStyle->Merge(txBody->lstStyle);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, bodyPr);
				pWriter->WriteRecord2(1, lstStyle);
				pWriter->WriteRecordArray(2, 0, Paragrs);
				pWriter->WriteRecord2(3, sp3d);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							bodyPr = new Logic::BodyPr();
							bodyPr->fromPPTY(pReader);
							break;
						}
						case 1:
						{
							lstStyle = new Logic::TextListStyle();
							lstStyle->fromPPTY(pReader);
							break;
						}
						case 2:
						{
							pReader->Skip(4);
							ULONG _c = pReader->GetULong();
							for (ULONG i = 0; i < _c; ++i)
							{
								pReader->Skip(1); // type
								Paragrs.push_back(Paragraph());
								Paragrs.back().fromPPTY(pReader);
							}
							break;
						}
						case 3:
						{
							sp3d = new Logic::Sp3d();
							sp3d->fromPPTY(pReader);
							break;
						}
						default:
						{
							break;
						}
					}
				}

				pReader->Seek(_end_rec);

                if (!bodyPr.IsInit())
                    bodyPr = new Logic::BodyPr();
            }

			nullable<Sp3d>			sp3d;
			nullable<BodyPr>		bodyPr;
			nullable<TextListStyle> lstStyle;
			std::vector<Paragraph>	Paragrs;

			std::wstring			m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(bodyPr.is_init())
					bodyPr->SetParentPointer(this);
				
				if(lstStyle.is_init())
					lstStyle->SetParentPointer(this);
				
				size_t count = Paragrs.size();
				for (size_t i = 0; i < count; ++i)
					Paragrs[i].SetParentPointer(this);
			}

		public:

			std::wstring GetDocxTxBoxContent(NSBinPptxRW::CBinaryFileWriter* pWriter, const nullable<PPTX::Logic::ShapeStyle>& shape_style);
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TXBODY_INCLUDE_H
