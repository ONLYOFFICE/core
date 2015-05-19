//#include "./stdafx.h"

#include "Shape.h"
#include "./../SlideLayout.h"
#include "./../SlideMaster.h"
#include "./../Slide.h"
#include "SpTree.h"

namespace PPTX
{
	namespace Logic
	{
		Shape::Shape()
		{
		}

		Shape::~Shape()
		{
		}

		Shape::Shape(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const Shape& Shape::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Shape::fromXML(XmlUtils::CXmlNode& node)
		{
			node.ReadAttributeBase(L"useBgFill", attrUseBgFill);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("nvSpPr") == strName)
						nvSpPr = oNode;
					else if (_T("spPr") == strName)
						spPr = oNode;
					else if (_T("style") == strName)
						style = oNode;
					else if (_T("txBody") == strName)
						txBody = oNode;
					else if (_T("txbx") == strName)
					{
						XmlUtils::CXmlNode _node = oNode.ReadNode(_T("w:txbxContent"));
						if (_node.IsValid())
							TextBoxShape = _node.GetXml();
					}
					else if (_T("cNvPr") == strName)
					{
						nvSpPr.cNvPr = oNode;
					}
					else if (_T("cNvSpPr") == strName)
					{
						nvSpPr.cNvSpPr = oNode;
					}
					else if (_T("txSp") == strName)
					{
						txBody = oNode.ReadNodeNoNS(_T("txBody"));
					}
					else if (_T("bodyPr") == strName)
					{
						TextBoxBodyPr = oNode;
					}
				}
			}

			FillParentPointersForChilds();
		}

		CString Shape::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("useBgFill"), attrUseBgFill);

			XmlUtils::CNodeValue oValue;
			oValue.Write(nvSpPr);
			oValue.Write(spPr);
			oValue.WriteNullable(style);
			oValue.WriteNullable(txBody);

			return XmlUtils::CreateNode(_T("p:sp"), oAttr, oValue);
		}		

		void Shape::FillParentPointersForChilds()
		{
			nvSpPr.SetParentPointer(this);
			spPr.SetParentPointer(this);
			if(style.IsInit())
				style->SetParentPointer(this);
			if(txBody.IsInit())
				txBody->SetParentPointer(this);
			levelUp = NULL;
		}

		void Shape::GetRect(Aggplus::RECT& pRect)const
		{
			pRect.bottom = 0;
			pRect.left = 0;
			pRect.right = 0;
			pRect.top = 0;

			if(spPr.xfrm.is_init())
			{
				pRect.left		= spPr.xfrm->offX.get_value_or(0);
				pRect.top		= spPr.xfrm->offY.get_value_or(0);
				pRect.right		= pRect.left + spPr.xfrm->extX.get_value_or(0);
				pRect.bottom	= pRect.top + spPr.xfrm->extY.get_value_or(0);
			}
			//else
			//{
			//	if(bMergeWithLevelUp)
			//	{
			//		FillLevelUp();
			//		if(levelUp != NULL)
			//			levelUp->GetRect(pRect);
			//	}
			//}
			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(pRect);
		}

		void Shape::FillLevelUp()const
		{
			if((levelUp == NULL) && (nvSpPr.nvPr.ph.IsInit()))
			{
				if((nvSpPr.nvPr.ph->type.IsInit()) || (nvSpPr.nvPr.ph->idx.IsInit()))
				{
					if(parentFileIs<Slide>())
						parentFileAs<Slide>().Layout->GetLevelUp(*this);
					else if(parentFileIs<SlideLayout>())
						parentFileAs<SlideLayout>().Master->GetLevelUp(*this);
				}
			}
		}

		void Shape::FillShapeProperties(ShapeProperties& props)
		{
			//props.SetParentFilePointer(*parentFile);
			if(parentFileIs<SlideMaster>())
			{
				CString type = nvSpPr.nvPr.ph.IsInit()?nvSpPr.nvPr.ph->type.get_value_or(_T("body")):_T("");
				//if( (nvSpPr->cNvSpPr->txBox.get_value_or(false)) && (type == "") )
				//	type = "text-box";
				parentFileAs<SlideMaster>().FillShapeProperties(props, type);
				props.FillFromTextBody(txBody);
				if(style.IsInit())
					props.FillFontRef(style->fontRef);

				//props.SetParentFilePointer(parentFile);
			}
			else if(parentFileIs<SlideLayout>())
			{
				//FillLevelUp();
				//if(levelUp != NULL)
				//	levelUp->GetShapeProperties(props);
				//else
				//{
					CString type = nvSpPr.nvPr.ph.IsInit()?nvSpPr.nvPr.ph->type.get_value_or(_T("body")):_T("");
					//if( (nvSpPr->cNvSpPr->txBox.get_value_or(false)) && (type == "") )
					//	type = "text-box";
					parentFileAs<SlideLayout>().FillShapeProperties(props, type);
				//}

				props.FillFromTextBody(txBody);
				if(style.IsInit())
					props.FillFontRef(style->fontRef);

				//props.SetParentFilePointer(parentFile);
			}
			else if(parentFileIs<Slide>())
			{
				//FillLevelUp();
				//if(levelUp != NULL)
				//	levelUp->GetShapeProperties(props);
				//else
				//{
					CString type = nvSpPr.nvPr.ph.is_init()?nvSpPr.nvPr.ph->type.get_value_or(_T("body")):_T("");
					//if( (nvSpPr->cNvSpPr->txBox.get_value_or(false)) && (type == "") )
					//	type = "text-box";
					parentFileAs<Slide>().FillShapeProperties(props, type);
				//}

				props.FillFromTextBody(txBody);
				if(style.IsInit())
					props.FillFontRef(style->fontRef);

				//props.SetParentFilePointer(parentFile);
			}
			props.SetParentFilePointer(parentFile);
		}

		void Shape::FillShapeTextProperties(CShapeTextProperties& props)
		{
			CString type = nvSpPr.nvPr.ph.IsInit()?nvSpPr.nvPr.ph->type.get_value_or(_T("body")):_T("");
			if (parentFileIs<SlideMaster>())
			{
				parentFileAs<SlideMaster>().FillShapeTextProperties(props, type);
				props.FillFromTextBody(txBody, NULL);
				if (style.IsInit())
					props.FillFontRef(style->fontRef, isFontRefInSlide);
			}
			else if (parentFileIs<SlideLayout>())
			{
				parentFileAs<SlideLayout>().FillShapeTextProperties(props, type);
				props.FillFromTextBody(txBody, NULL);
				if(style.IsInit())
					props.FillFontRef(style->fontRef, isFontRefInSlide);
			}
			else if (parentFileIs<Slide>())
			{
				parentFileAs<Slide>().FillShapeTextProperties(props, type);
				props.FillFromTextBody(txBody, body);
				if(style.IsInit())
					props.FillFontRef(style->fontRef, isFontRefInSlide);
			}

			props.SetParentFilePointer(parentFile);
		}

		DWORD Shape::GetFill(UniFill& fill)const
		{
			DWORD BGRA = 0;
			//fill.SetParentFilePointer(*parentFile);

			if(style.IsInit())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);

				if (style->fillRef.Color.is_init())
				{
					if (fill.is<PPTX::Logic::SolidFill>())
					{
						fill.as<PPTX::Logic::SolidFill>().Color = style->fillRef.Color;
					}
				}
				BGRA = style->fillRef.Color.GetBGRA();
			}

			if(spPr.Fill.is_init())
				spPr.Fill.Merge(fill);

			//if((!fill.is_init()) && (bMergeWithLevelUp))
			//{
			//	FillLevelUp();
			//	if(levelUp != NULL)
			//		BGRA = levelUp->GetFill(fill);
			//}

			return BGRA;
		}

		DWORD Shape::GetLine(Ln& line)const
		{
			DWORD BGRA = 0;
			//line.SetParentFilePointer(*parentFile);

			//if(bMergeWithLevelUp)
			//{
			//	FillLevelUp();
			//	if(levelUp != NULL)
			//		BGRA = levelUp->GetLine(line);
			//}
			if(style.IsInit())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);

				BGRA = style->lnRef.Color.GetBGRA();
			}

			if(spPr.ln.IsInit())
				spPr.ln->Merge(line);
			return BGRA;
		}

		void Shape::Merge(Shape& shape, bool bIsSlidePlaceholder)const
		{
			shape.nvSpPr = nvSpPr;
			spPr.Merge(shape.spPr);
			if(style.is_init())
			{
				shape.isFontRefInSlide = bIsSlidePlaceholder;
				shape.style = style;
				shape.style->SetParentFilePointer(parentFile);
			}
			if (!bIsSlidePlaceholder)
			{
				if(txBody.IsInit())
					txBody->Merge(shape.txBody);
			}
			else
			{
				if (txBody.is_init())
				{				
					if(!shape.txBody.is_init())
						shape.txBody = new TxBody();
				
					txBody->bodyPr.Merge(shape.txBody->bodyPr);
					if(txBody->lstStyle.is_init())
					{
						for (int i = 0; i < 10; i++)
							if(txBody->lstStyle->levels[i].is_init())
								txBody->lstStyle->levels[i]->Merge(body[i]);
					}
				}
			}
		}

		void Shape::GetShapeFullDescription(Shape& shape, int level)const
		{
			if(level == 0)
				//shape.SetParentFilePointer(*parentFile);
				shape.SetParentPointer(parentElement);

			if(!nvSpPr.nvPr.ph.is_init())
			{
				Merge(shape);
				return;
			}
			if(parentFileIs<PPTX::SlideMaster>())
			{
				Merge(shape);
				return;
			}
			if(parentFileIs<PPTX::SlideLayout>())
			{
				FillLevelUp();
				if(levelUp != NULL)
					levelUp->GetShapeFullDescription(shape, level + 1);
				Merge(shape);
				return;
			}
			if(parentFileIs<PPTX::Slide>())
			{
				FillLevelUp();
				if(levelUp != NULL)
					levelUp->GetShapeFullDescription(shape, level + 1);
				Merge(shape, true);
				return;
			}
		}

		void Shape::toXmlWriterVML(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::WrapperFile>& oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& oClrMap)
		{
			CString strPath = _T("");
			CString strTextRect = _T("");

			LONG lW = 43200;
			LONG lH = 43200;
			if (spPr.xfrm.is_init())
			{
				lW = spPr.xfrm->extX.get_value_or(43200);
				lH = spPr.xfrm->extY.get_value_or(43200);
			}
			NSShapeImageGen::COleInfo oOleInfo;
			bool bOle = false;
			CString sOleProgID;
			CString sOleNodeName;
			if(this->spPr.Fill.Fill.is<PPTX::Logic::BlipFill>())
			{
				PPTX::Logic::BlipFill& oBlipFill = this->spPr.Fill.Fill.as<PPTX::Logic::BlipFill>();
				if(oBlipFill.blip.IsInit() && oBlipFill.blip->oleInfo.IsInit())
					oOleInfo = oBlipFill.blip->oleInfo.get();
			}
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
			
#ifdef AVS_USE_CONVERT_PPTX_TOCUSTOM_VML
			spPr.Geometry.ConvertToCustomVML(pWriter->m_pOOXToVMLRenderer, strPath, strTextRect, lW, lH);
#endif

			CString strId = _T("");
			strId.Format(_T("shape %d"), pWriter->m_lObjectIdVML);
			CString strSpid = _T("");
			strSpid.Format(_T("_x%04d_s%04d"), 0xFFFF & (pWriter->m_lObjectIdVML >> 16), 0xFFFF & pWriter->m_lObjectIdVML);
			pWriter->m_lObjectIdVML++;

			CString strFillAttr = _T("");
			CString strStrokeAttr = _T("");
			CString strFillNode = _T("");
			CString strStrokeNode = _T("");
			CalculateFill(spPr, style, oTheme, oClrMap, strFillAttr, strFillNode, bOle);
			CalculateLine(spPr, style, oTheme, oClrMap, strStrokeAttr, strStrokeNode);

			if (pWriter->m_strStyleMain != _T(""))
			{
				pWriter->StartNode(_T("v:shape"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("id"), strId);
				pWriter->WriteAttribute(_T("o:spid"), strSpid);

				NSBinPptxRW::CXmlWriter oStylesWriter;
				if (spPr.xfrm.is_init())
				{
					if (spPr.xfrm->rot.is_init())
					{
						int nRot = (int)((double)(*(spPr.xfrm->rot)) / 60000.0);
						oStylesWriter.WriteAttributeCSS_int(_T("rotation"), nRot);
					}
					bool bIsFH = spPr.xfrm->flipH.get_value_or(false);
					bool bIsFV = spPr.xfrm->flipV.get_value_or(false);
					if (bIsFH && bIsFV)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("xy"));
					}
					else if (bIsFH)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("x"));
					}
					else if (bIsFV)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("y"));
					}
				}

				if (txBody.is_init())
				{
					if (txBody->bodyPr.anchor.is_init())
					{
						CString _strAnchor = txBody->bodyPr.anchor->get();
						if (_strAnchor == _T("t"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("top"));
						else if (_strAnchor == _T("b"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("bottom"));
						else if (_strAnchor == _T("ctr"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("middle"));
					}
				}
				else if (TextBoxBodyPr.is_init())
				{
					if (TextBoxBodyPr->anchor.is_init())
					{
						CString _strAnchor = TextBoxBodyPr->anchor->get();
						if (_strAnchor == _T("t"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("top"));
						else if (_strAnchor == _T("b"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("bottom"));
						else if (_strAnchor == _T("ctr"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("middle"));
					}
				}

				if (oStylesWriter.GetSize() == 0)
				{
					pWriter->WriteAttribute(_T("style"), pWriter->m_strStyleMain);
				}
				else
				{
					pWriter->WriteAttribute(_T("style"), pWriter->m_strStyleMain + oStylesWriter.GetXmlString());
				}
				if(!bOle)
				{
					pWriter->WriteAttribute(_T("coordsize"), (CString)_T("100000,100000"));
					pWriter->WriteAttribute(_T("path"), strPath);
				}

				if (pWriter->m_strAttributesMain)
				{
					pWriter->WriteString(pWriter->m_strAttributesMain);
					pWriter->m_strAttributesMain = _T("");
				}

				pWriter->WriteString(strFillAttr);
				pWriter->WriteString(strStrokeAttr);

				pWriter->EndAttributes();

				pWriter->StartNode(_T("v:path"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("textboxrect"), strTextRect);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("v:path"));

				pWriter->WriteString(strFillNode);
				pWriter->WriteString(strStrokeNode);
				pWriter->WriteString(pWriter->m_strNodes);
				pWriter->m_strNodes = _T("");

				if (TextBoxShape.is_init())
				{
					pWriter->StartNode(_T("v:textbox"));
					pWriter->EndAttributes();
					pWriter->WriteString(*TextBoxShape);
					pWriter->EndNode(_T("v:textbox"));
				}

				pWriter->EndNode(_T("v:shape"));

				pWriter->m_strStyleMain = _T("");
			}
			else
			{
				int dL = 0;
				int dT = 0;
				int dW = 0;
				int dH = 0;

				NSBinPptxRW::CXmlWriter oStylesWriter;
				if (spPr.xfrm.is_init())
				{
					if (spPr.xfrm->offX.is_init())
						dL = (*spPr.xfrm->offX);
					if (spPr.xfrm->offY.is_init())
						dT = (*spPr.xfrm->offY);
					if (spPr.xfrm->extX.is_init())
						dW = (*spPr.xfrm->extX);
					if (spPr.xfrm->extY.is_init())
						dH = (*spPr.xfrm->extY);
				}
				oStylesWriter.WriteAttributeCSS(_T("position"), _T("absolute"));
				oStylesWriter.WriteAttributeCSS_int(_T("left"), dL);
				oStylesWriter.WriteAttributeCSS_int(_T("top"), dT);
				oStylesWriter.WriteAttributeCSS_int(_T("width"), dW);
				oStylesWriter.WriteAttributeCSS_int(_T("height"), dH);

				if (spPr.xfrm.is_init())
				{
					if (spPr.xfrm->rot.is_init())
					{
						int nRot = (int)((double)(*(spPr.xfrm->rot)) / 60000.0);
						oStylesWriter.WriteAttributeCSS_int(_T("rotation"), nRot);
					}
					bool bIsFH = spPr.xfrm->flipH.get_value_or(false);
					bool bIsFV = spPr.xfrm->flipV.get_value_or(false);
					if (bIsFH && bIsFV)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("xy"));
					}
					else if (bIsFH)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("x"));
					}
					else if (bIsFV)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("y"));
					}
				}

				if (txBody.is_init())
				{
					if (txBody->bodyPr.anchor.is_init())
					{
						CString _strAnchor = txBody->bodyPr.anchor->get();
						if (_strAnchor == _T("t"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("top"));
						else if (_strAnchor == _T("b"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("bottom"));
						else if (_strAnchor == _T("ctr"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("middle"));
					}
				}
				else if (TextBoxBodyPr.is_init())
				{
					if (TextBoxBodyPr->anchor.is_init())
					{
						CString _strAnchor = TextBoxBodyPr->anchor->get();
						if (_strAnchor == _T("t"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("top"));
						else if (_strAnchor == _T("b"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("bottom"));
						else if (_strAnchor == _T("ctr"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("middle"));
					}
				}

				pWriter->StartNode(_T("v:shape"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("id"), strId);
				pWriter->WriteAttribute(_T("o:spid"), strSpid);

				pWriter->WriteAttribute(_T("style"), oStylesWriter.GetXmlString());

				if(!bOle)
				{
					pWriter->WriteAttribute(_T("coordsize"), (CString)_T("100000,100000"));
					pWriter->WriteAttribute(_T("path"), strPath);
				}

				if (pWriter->m_strAttributesMain)
				{
					pWriter->WriteString(pWriter->m_strAttributesMain);
					pWriter->m_strAttributesMain = _T("");
				}

				pWriter->WriteString(strFillAttr);
				pWriter->WriteString(strStrokeAttr);

				pWriter->EndAttributes();

				pWriter->StartNode(_T("v:path"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("textboxrect"), strTextRect);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("v:path"));

				pWriter->WriteString(strFillNode);
				pWriter->WriteString(strStrokeNode);

				if (TextBoxShape.is_init())
				{
					pWriter->StartNode(_T("v:textbox"));
					pWriter->EndAttributes();
					pWriter->WriteString(*TextBoxShape);
					pWriter->EndNode(_T("v:textbox"));
				}

				pWriter->EndNode(_T("v:shape"));
			}
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
	} // namespace Logic
} // namespace PPTX