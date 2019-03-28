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


#include "Shape.h"
#include "../SlideLayout.h"
#include "../SlideMaster.h"
#include "../Slide.h"
#include "SpTree.h"
#include "Pic.h"

namespace PPTX
{
	namespace Logic
	{
		Shape::Shape(std::wstring name_)
		{
			m_name				= name_;
			m_pLevelUp			= NULL;
		}

		Shape::~Shape()
		{
		}

		Shape::Shape(XmlUtils::CXmlNode& node)
		{
			m_pLevelUp			= NULL;
			
			fromXML(node);
		}
		Shape::Shape(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pLevelUp			= NULL;
			
			fromXML(oReader);
		}		
		const Shape& Shape::operator =(XmlUtils::CXmlNode& node)
		{
			m_pLevelUp			= NULL;

			fromXML(node);
			return *this;
		}
		const Shape& Shape::operator =(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pLevelUp			= NULL;

			fromXML(oReader);
			return *this;
		}
		void Shape::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = oReader.GetName();

			ReadAttributes( oReader );
			
			if ( oReader.IsEmptyNode() )
				return;
					
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("nvSpPr") == strName)
					nvSpPr = oReader;
				else if (_T("spPr") == strName)
					spPr = oReader;
				else if (_T("style") == strName)
					style = oReader;
				else if (_T("txBody") == strName)
					txBody = oReader;
				else if (_T("txXfrm")  == strName)
					txXfrm = oReader;
				else if (_T("txbx") == strName || _T("textbox") == strName)
				{
					if ( oReader.IsEmptyNode() )
						continue;
							
					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						std::wstring strName1 = XmlUtils::GetNameNoNS(oReader.GetName());

						if (strName1 == L"txbxContent")
						{
							oTextBoxShape = oReader;
							break;
						}
					}
				}
				else if (_T("cNvPr") == strName)
					nvSpPr.cNvPr = oReader;
				else if (_T("cNvSpPr") == strName)
					nvSpPr.cNvSpPr = oReader;
				else if (_T("txSp") == strName)
				{
					if ( oReader.IsEmptyNode() )
						continue;
							
					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						std::wstring strName1 = XmlUtils::GetNameNoNS(oReader.GetName());

						if (strName1 == L"txBody")
						{
							txBody = oReader;
							break;
						}
					}
				}
				else if (_T("bodyPr") == strName)
					oTextBoxBodyPr = oReader;
			}
			FillParentPointersForChilds();
		}
		void Shape::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = node.GetName();

            XmlMacroReadAttributeBase(node,L"useBgFill", attrUseBgFill);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("nvSpPr") == strName)
						nvSpPr = oNode;
					else if (_T("spPr") == strName)
						spPr = oNode;
					else if (_T("style") == strName)
						style = oNode;
					else if (_T("txBody") == strName)
						txBody = oNode;
					else if (_T("txXfrm")  == strName)
						txXfrm = oNode;
					else if (_T("txbx") == strName || _T("textbox") == strName)
						oTextBoxShape = oNode.ReadNode(_T("w:txbxContent"));
					else if (_T("cNvPr") == strName)
						nvSpPr.cNvPr = oNode;
					else if (_T("cNvSpPr") == strName)
						nvSpPr.cNvSpPr = oNode;
					else if (_T("txSp") == strName)
						txBody = oNode.ReadNodeNoNS(_T("txBody"));
					else if (_T("bodyPr") == strName)
						oTextBoxBodyPr = oNode;
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring Shape::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("useBgFill"), attrUseBgFill);

			XmlUtils::CNodeValue oValue;
			oValue.Write(nvSpPr);
			oValue.Write(spPr);
			oValue.WriteNullable(style);
			oValue.WriteNullable(txBody);

			return XmlUtils::CreateNode(m_name, oAttr, oValue);
		}		

		void Shape::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring name_ = m_name;

			if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)			name_ = L"wps:wsp";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			name_ = L"xdr:sp";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		name_ = L"a:sp";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	name_ = L"cdr:sp";
				
			pWriter->StartNode(name_);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("useBgFill"), attrUseBgFill);
			pWriter->EndAttributes();

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
			{
				nvSpPr.cNvSpPr.toXmlWriter2(_T("wps"), pWriter);
			}
			else
				nvSpPr.toXmlWriter(pWriter);
			
			bool bIsPresentStyle = false;
			if (style.is_init() && (style->fillRef.idx.is_init() || style->fillRef.Color.Color.is_init()))
			{
				bIsPresentStyle = true;
			}
			if (pWriter->m_lGroupIndex > 1 && !bIsPresentStyle)
			{
				pWriter->m_lFlag += 0x02;
			}
			spPr.toXmlWriter(pWriter);
			if (pWriter->m_lGroupIndex > 1 && !bIsPresentStyle)
			{
				pWriter->m_lFlag -= 0x02;
			}
			if (style.is_init())
			{
				if		(pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)			style->m_namespace = L"wps";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			style->m_namespace = L"xdr";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		style->m_namespace = L"a";
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	style->m_namespace = L"cdr";

                pWriter->Write(style);
            }
			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
			{	
				bool bIsWritedBodyPr = false;
				if (strTextBoxShape.is_init())
				{
					pWriter->WriteString(_T("<wps:txbx>"));					
					//pWriter->WriteString(oTextBoxShape->toXML());
					pWriter->WriteString(*strTextBoxShape);
					pWriter->WriteString(_T("</wps:txbx>"));

					if (oTextBoxBodyPr.is_init())
					{
						oTextBoxBodyPr->m_namespace = _T("wps");
						oTextBoxBodyPr->toXmlWriter(pWriter);
						bIsWritedBodyPr = true;
					}
				}
				else if (txBody.is_init())
				{
					txBody->m_name = _T("wps:txBody");
					pWriter->Write(txBody);
				}

                if (!bIsWritedBodyPr)
				{
					pWriter->WriteString(_T("<wps:bodyPr rot=\"0\"><a:prstTxWarp prst=\"textNoShape\"><a:avLst/></a:prstTxWarp><a:noAutofit/></wps:bodyPr>"));
				}
			}
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)
			{
				txBody->m_name = L"a:txBody";

				pWriter->StartNode(L"a:txSp");
				pWriter->EndAttributes();
					pWriter->Write(txBody);
					pWriter->WriteString(L"<a:useSpRect/>");
				pWriter->EndNode(L"a:txSp");
			}
			else
			{
				if (txBody.is_init())
				{
					if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)	
						txBody->m_name = L"xdr:txBody";
					if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	
						txBody->m_name = L"cdr:txBody";
				}
				pWriter->Write(txBody);
			}

			pWriter->EndNode(name_);
		}
		
		void Shape::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
        {
            LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						attrUseBgFill = pReader->GetBool();
						break;
					}
					default:
						break;
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						nvSpPr.fromPPTY(pReader);
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
					case 3:
					{
						txBody = new TxBody();
						txBody->fromPPTY(pReader);
						break;
					}
					case 4:
					{
						if (NULL != pReader->m_pMainDocument)
						{
                            LONG lLenRec = pReader->GetLong();

                            LONG lPosition = pReader->GetPos();
                            LONG lSize_Reader = pReader->GetSize();
							BYTE* pData_Reader = pReader->GetData();
			
							std::wstring sXmlContent;
							pReader->m_pMainDocument->getXmlContent(*pReader, lLenRec, sXmlContent);

							std::wstring strC = _T("<w:txbxContent>");
							strC += sXmlContent;
							strC += _T("</w:txbxContent>");
							strTextBoxShape = strC;

							//pReader->Seek(lPosition + lLenRec);
							pReader->Init(pData_Reader, lPosition + lLenRec, lSize_Reader - (lPosition + lLenRec));
						}
						else
						{
							pReader->SkipRecord();
						}
						break;
					}
					case 5:
					{
						oTextBoxBodyPr = new PPTX::Logic::BodyPr();
						oTextBoxBodyPr->fromPPTY(pReader);
						break;
					}
					case 6:
					{
						txXfrm = new PPTX::Logic::Xfrm();
						txXfrm->fromPPTY(pReader);
						break;
					}
					case 7:
					{
						signatureLine = new OOX::VmlOffice::CSignatureLine();
						signatureLine->fromPPTY(pReader);
						break;
					}
					default:
					{
						pReader->SkipRecord();
						break;
					}
				}
			}

			pReader->Seek(_end_rec);
		}



		void Shape::FillParentPointersForChilds()
		{
			nvSpPr.SetParentPointer(this);
			spPr.SetParentPointer(this);
			
			if (style.IsInit())
				style->SetParentPointer(this);
			if (txBody.IsInit())
				txBody->SetParentPointer(this);			
		}
		
		void Shape::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter)const
		{
			pWriter->StartRecord(SPTREE_TYPE_SHAPE);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBool2(0, attrUseBgFill);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, nvSpPr);
			pWriter->WriteRecord1(1, spPr);
			pWriter->WriteRecord2(2, style);

			if (pWriter->m_pMainDocument != NULL)
			{
				if (oTextBoxShape.is_init())
				{	
                    long lDataSize = 0;
                    ULONG lPos = pWriter->GetPosition();
					pWriter->SetPosition(lPos);
					
					pWriter->StartRecord(4);
					pWriter->m_pMainDocument->getBinaryContentElem(OOX::et_w_sdtContent, oTextBoxShape.GetPointer(), *pWriter, lDataSize);
					pWriter->EndRecord();
					
					if (oTextBoxBodyPr.is_init())
					{
						pWriter->StartRecord(5);
						oTextBoxBodyPr->toPPTY(pWriter);
						pWriter->EndRecord();
					}
				}
				else if (strTextBoxShape.is_init())//после конвертации старого шейпа (vml)
				{
                    long lDataSize = 0;
                    ULONG lPos = pWriter->GetPosition();
					pWriter->SetPosition(lPos);
					
					pWriter->StartRecord(4);
					pWriter->m_pMainDocument->getBinaryContent(strTextBoxShape.get(), *pWriter, lDataSize);
					pWriter->EndRecord();		
					
					if (oTextBoxBodyPr.is_init())
					{
						pWriter->StartRecord(5);
						oTextBoxBodyPr->toPPTY(pWriter);
						pWriter->EndRecord();
					}
				}

				else if (txBody.is_init())
				{
					std::wstring strContent = txBody->GetDocxTxBoxContent(pWriter, style);
					//tooooo convert a:p -> w:p

                    long lDataSize = 0;
                    ULONG lPos = pWriter->GetPosition();
					pWriter->SetPosition(lPos);
					pWriter->StartRecord(4);
					pWriter->m_pMainDocument->getBinaryContent(strContent, *pWriter, lDataSize);
					pWriter->EndRecord();

					pWriter->WriteRecord2(5, txBody->bodyPr);
				}
			}
			else
			{
				pWriter->WriteRecord2(3, txBody);
			}

			pWriter->WriteRecord2(6, txXfrm);
			pWriter->WriteRecord2(7, signatureLine);
			
			pWriter->EndRecord();
		}

		void Shape::FillLevelUp()
		{
			if ((m_pLevelUp == NULL) && (nvSpPr.nvPr.ph.IsInit()))
			{
				if ((nvSpPr.nvPr.ph->type.IsInit()) || (nvSpPr.nvPr.ph->idx.IsInit()))
				{
					if (parentFileIs<Slide>())
					{
						if (parentFileAs<Slide>().Layout.IsInit())
							parentFileAs<Slide>().Layout->GetLevelUp(this);
					}
					else if(parentFileIs<SlideLayout>())
					{
						if (parentFileAs<SlideLayout>().Master.IsInit())
							parentFileAs<SlideLayout>().Master->GetLevelUp(this);
					}
					else if(parentFileIs<NotesSlide>())
					{
						if (parentFileAs<NotesSlide>().master_.IsInit())
							parentFileAs<NotesSlide>().master_->GetLevelUp(this);
					}
				}
			}
		}

		bool Shape::IsListStyleEmpty()
		{
			if ((m_pLevelUp) && (m_pLevelUp->IsListStyleEmpty() == false)) return false;

			if (txBody.IsInit()				== false) return true;
			if (txBody->lstStyle.IsInit()	== false) return true;
			
			return txBody->lstStyle->IsListStyleEmpty();
		}
		void Shape::Merge(Pic& pic, bool bIsSlidePlaceholder)
		{
			if (m_pLevelUp)
				m_pLevelUp->Merge(pic, true);

			pic.nvPicPr.cNvPr	= nvSpPr.cNvPr;
			pic.nvPicPr.nvPr	= nvSpPr.nvPr;

			spPr.Merge(pic.spPr);

			if (style.is_init())
			{
				pic.style = style;
				pic.style->SetParentFilePointer(parentFile);
			}
		}
		void Shape::Merge(Shape& shape, bool bIsSlidePlaceholder)
		{
			if (m_pLevelUp)
				m_pLevelUp->Merge(shape, true);

			shape.m_name = m_name;	
			shape.nvSpPr = nvSpPr;			
			spPr.Merge(shape.spPr);

			if (style.is_init())
			{
				shape.m_bIsFontRefInSlide = bIsSlidePlaceholder;
				shape.style = style;
				shape.style->SetParentFilePointer(parentFile);
			}

			if (txBody.IsInit())
			{
				if(!shape.txBody.is_init())
					shape.txBody = new TxBody();
				
				if (!bIsSlidePlaceholder)
				{					
					txBody->Merge(shape.txBody);
					shape.txBody->Paragrs = txBody->Paragrs;
				}
				else
				{
					if (txBody->bodyPr.IsInit())
						txBody->bodyPr->Merge(shape.txBody->bodyPr);					
				}
				if (txBody->lstStyle.is_init())
				{
					if(!shape.txBody->lstStyle.is_init())
						shape.txBody->lstStyle = new TextListStyle();
					for (int i = 0; i < 10; i++)
					{
						if(txBody->lstStyle->levels[i].is_init())
							txBody->lstStyle->levels[i]->Merge(shape.txBody->lstStyle->levels[i]);
					}
				}
			}
		}

		void Shape::toXmlWriterVML(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::Theme>& oTheme, NSCommon::smart_ptr<PPTX::Logic::ClrMap>& oClrMap, const WCHAR* pId, bool in_group, bool bSignature)
		{
			std::wstring strPath, strTextRect;
			SimpleTypes::Vml::SptType vmlPrst = SimpleTypes::Vml::sptNotPrimitive;

			LONG lW = 43200, lH = 43200;
			int dL = 0, dT = 0, dW = 0, dH = 0;
			
			if (spPr.Geometry.is<PrstGeom>())
			{
				const PPTX::Logic::PrstGeom & lpGeom = spPr.Geometry.as<PPTX::Logic::PrstGeom>();
				
				SimpleTypes::CShapeType<> ooxPrst = SimpleTypes::CShapeType<>(lpGeom.prst.get());
				vmlPrst =  OOX::PrstGeom2VmlShapeType( ooxPrst.GetValue());
			}   

			if (spPr.xfrm.is_init())
			{
				if (spPr.xfrm->offX.is_init())	dL = *spPr.xfrm->offX;
				if (spPr.xfrm->offY.is_init())	dT = *spPr.xfrm->offY;
				if (spPr.xfrm->extX.is_init())	dW = *spPr.xfrm->extX;
				if (spPr.xfrm->extY.is_init())	dH = *spPr.xfrm->extY;

				lW = spPr.xfrm->extX.get_value_or(43200);
				lH = spPr.xfrm->extY.get_value_or(43200);
			}

			spPr.Geometry.ConvertToCustomVML(pWriter->m_pOOXToVMLRenderer, strPath, strTextRect, lW, lH);
            
			std::wstring strId		= L"shape "		+ std::to_wstring(pWriter->m_lObjectIdVML);
			std::wstring strSpid	= L"_x0000_s"	+ std::to_wstring(pWriter->m_lObjectIdVML);
			
			pWriter->m_lObjectIdVML++;

            std::wstring strFillAttr;
            std::wstring strStrokeAttr;
            std::wstring strFillNode;
            std::wstring strStrokeNode;;

			CalculateFill(spPr, style, oTheme, oClrMap, strFillAttr, strFillNode, false, bSignature);
			CalculateLine(spPr, style, oTheme, oClrMap, strStrokeAttr, strStrokeNode, false, bSignature);

			pWriter->StartNode(L"v:shape");

			pWriter->StartAttributes();

			if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
			{
				if(NULL == pId)
				{
					pWriter->WriteAttribute(L"id", strSpid);
				}
				else
				{
					pWriter->WriteAttribute(L"id", pId);
					pWriter->WriteAttribute(L"o:spid", strSpid);
				}
			}
			else
			{
				pWriter->WriteAttribute(L"id",		strId);
				pWriter->WriteAttribute(L"o:spid",	strSpid);
			}

			if (vmlPrst != SimpleTypes::Vml::sptNotPrimitive)
				pWriter->WriteAttribute(L"o:spt", (int)vmlPrst);

			NSBinPptxRW::CXmlWriter oStylesWriter;
			if (pWriter->m_strStyleMain.empty())
			{
				oStylesWriter.WriteAttributeCSS(L"position", L"absolute");
				if (in_group)
				{
					oStylesWriter.WriteAttributeCSS_int(L"left",	dL / 100);
					oStylesWriter.WriteAttributeCSS_int(L"top",		dT / 100);
					oStylesWriter.WriteAttributeCSS_int(L"width",	dW / 100);
					oStylesWriter.WriteAttributeCSS_int(L"height",	dH / 100);
				}
				else
				{
					oStylesWriter.WriteAttributeCSS_int_pt(L"left",		dL / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"top",		dT / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"width",	dW / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"height",	dH / 12700);
				}
			}
			if (spPr.xfrm.is_init())
			{
				if (spPr.xfrm->rot.is_init())
				{
					int nRot = (int)((double)(*(spPr.xfrm->rot)) / 60000.0);
					oStylesWriter.WriteAttributeCSS_int(L"rotation", nRot);
				}
				bool bIsFH = spPr.xfrm->flipH.get_value_or(false);
				bool bIsFV = spPr.xfrm->flipV.get_value_or(false);
				if (bIsFH && bIsFV)
				{
					oStylesWriter.WriteAttributeCSS(L"flip", L"xy");
				}
				else if (bIsFH)
				{
					oStylesWriter.WriteAttributeCSS(L"flip", L"x");
				}
				else if (bIsFV)
				{
					oStylesWriter.WriteAttributeCSS(L"flip", L"y");
				}
			}

			if (txBody.is_init())
			{
				if (txBody->bodyPr.IsInit() && txBody->bodyPr->anchor.is_init())
				{
					std::wstring _strAnchor = txBody->bodyPr->anchor->get();
					if (_strAnchor == L"t")
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"top");
					else if (_strAnchor == L"b")
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"bottom");
					else if (_strAnchor == L"ctr")
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"middle");
				}
			}
			else if (oTextBoxBodyPr.is_init())
			{
				if (oTextBoxBodyPr->anchor.is_init())
				{
					std::wstring _strAnchor = oTextBoxBodyPr->anchor->get();
					if (_strAnchor == L"t")
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"top");
					else if (_strAnchor == L"b")
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"bottom");
					else if (_strAnchor == _T("ctr"))
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"middle");
				}
			}

			pWriter->WriteAttribute(L"style", pWriter->m_strStyleMain + oStylesWriter.GetXmlString());

			pWriter->WriteAttribute(L"coordsize", (std::wstring)L"100000,100000");
			pWriter->WriteAttribute(L"path", strPath);

			if (!pWriter->m_strAttributesMain.empty())
			{
				pWriter->WriteString(pWriter->m_strAttributesMain);
				pWriter->m_strAttributesMain.clear();
			}

			pWriter->WriteString(strFillAttr);
			pWriter->WriteString(strStrokeAttr);

			pWriter->EndAttributes();

			pWriter->StartNode(L"v:path");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"textboxrect", strTextRect);
			pWriter->EndAttributes();
			pWriter->EndNode(L"v:path");

			pWriter->WriteString(strFillNode);
			pWriter->WriteString(strStrokeNode);
			
			pWriter->WriteString(pWriter->m_strNodes);
			pWriter->m_strNodes.clear();

			if (strTextBoxShape.is_init())
			{
				pWriter->StartNode(L"v:textbox");
				pWriter->EndAttributes();
				pWriter->WriteString(*strTextBoxShape); //??? todooo -> oTextBoxShape
				pWriter->EndNode(L"v:textbox");
			}
			if (signatureLine.is_init())
			{
				signatureLine->toXmlWriter(pWriter);
			}

			pWriter->EndNode(L"v:shape");

			pWriter->m_strStyleMain.clear();
		}
		void Shape::toXmlWriterVMLBackground(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::Theme>& oTheme, NSCommon::smart_ptr<PPTX::Logic::ClrMap>& oClrMap)
		{
			std::wstring strFillAttr, strFillNode;
			CalculateFill(spPr, style, oTheme, oClrMap, strFillAttr, strFillNode, false);

			pWriter->StartNode(L"v:background");

			pWriter->StartAttributes();

			pWriter->WriteString(L" id=\"_x0000_s1025\"");

			if (!pWriter->m_strAttributesMain.empty())
			{
				pWriter->WriteString(pWriter->m_strAttributesMain);
				pWriter->m_strAttributesMain.clear();
			}

			pWriter->WriteString(strFillAttr);

			pWriter->EndAttributes();

			pWriter->WriteString(strFillNode);

			pWriter->EndNode(L"v:background");
		}
	} // namespace Logic
} // namespace PPTX
