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
//#include "./stdafx.h"

#include "Pic.h"
#include "SpTree.h"
#include "./../SlideLayout.h"
#include "./../SlideMaster.h"
#include "./../Slide.h"
#include "Media/MediaFile.h"
#include "Media/WavAudioFile.h"

namespace PPTX
{
	namespace Logic
	{
		void COLEObject::fromXML(XmlUtils::CXmlNode& node)
		{
			node.ReadAttributeBase(L"DrawAspect", m_oDrawAspect);
			node.ReadAttributeBase(L"r:id", m_oId);
			node.ReadAttributeBase(L"ObjectID", m_sObjectId);
			node.ReadAttributeBase(L"ProgID", m_sProgId);
			node.ReadAttributeBase(L"ShapeID", m_sShapeId);
			node.ReadAttributeBase(L"Type", m_oType);
			node.ReadAttributeBase(L"UpdateMode", m_oUpdateMode);
			node.ReadAttributeBase(L"pathbin", m_sFilepathBin);
			node.ReadAttributeBase(L"pathimg", m_sFilepathImg);
		}

		CString COLEObject::toXML() const
		{
			return L"";
		}
		void COLEObject::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			CString strName = L"o:OLEObject";
			pWriter->StartNode(strName);

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"DrawAspect", m_oDrawAspect);
			if(m_oId.IsInit())
			{
				pWriter->WriteAttribute(L"r:id", m_oId->ToString());
			}
			pWriter->WriteAttribute(L"ObjectID", m_sObjectId);
			pWriter->WriteAttribute2(L"ProgID", m_sProgId);
			pWriter->WriteAttribute(L"ShapeID", m_sShapeId);
			pWriter->WriteAttribute(L"Type", m_oType);
			pWriter->WriteAttribute(L"UpdateMode", m_oUpdateMode);
			pWriter->EndAttributes();

			pWriter->EndNode(strName);
		}
		void COLEObject::toXmlWriterXlsx(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			CString strName = L"oleObject";
			pWriter->StartNode(strName);

			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"progId", m_sProgId);
			if(m_oDrawAspect.IsInit())
			{
				CString sDrawAspect;
				if(0 == m_oDrawAspect->GetBYTECode())
				{
					sDrawAspect = L"DVASPECT_CONTENT";
				}
				else
				{
					sDrawAspect = L"DVASPECT_ICON";
				}
				pWriter->WriteAttribute(L"dvAspect", sDrawAspect);
			}
			if(m_oUpdateMode.IsInit())
			{
				CString sUpdateMode;
				if(0 == m_oUpdateMode->GetBYTECode())
				{
					sUpdateMode = L"OLEUPDATE_ALWAYS";
				}
				else
				{
					sUpdateMode = L"OLEUPDATE_ONCALL";
				}
				pWriter->WriteAttribute(L"oleUpdate", sUpdateMode);
			}
			pWriter->WriteAttribute(L"shapeId", m_sShapeId);
			if(m_oId.IsInit())
			{
				pWriter->WriteAttribute(L"r:id", m_oId->ToString());
			}
			pWriter->WriteAttribute2(L"pathbin", m_sFilepathBin);
			pWriter->WriteAttribute2(L"pathimg", m_sFilepathImg);
			pWriter->WriteAttribute2(L"idimg", m_oIdImg->ToString());
			pWriter->EndAttributes();

			pWriter->EndNode(strName);
		}
		void COLEObject::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			std::wstring sData;
			if(m_oId.IsInit() && m_sProgId.IsInit() && 0 == m_sProgId.get().Find(L"asc."))
			{
				FileContainer* pRels = NULL;
				if (pWriter->m_pCommonRels->is_init())
					pRels = pWriter->m_pCommonRels->operator ->();

				CString sFilePath = this->GetFullOleName(PPTX::RId(m_oId.get()), pRels);
				if(!sFilePath.IsEmpty())
				{
					sData = GetOleData(string2std_string(sFilePath));
				}
			}

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, m_sProgId);
			if(!sData.empty())
			{
				pWriter->WriteString1Data(1, sData.c_str(), sData.length());
			}
			pWriter->WriteInt2(2, m_oDxaOrig);
			pWriter->WriteInt2(3, m_oDyaOrig);
			pWriter->WriteLimit2(4, m_oDrawAspect);
			pWriter->WriteLimit2(5, m_oType);
			pWriter->WriteLimit2(6, m_oUpdateMode);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}

		void COLEObject::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					m_sProgId = pReader->GetString2();
				}
				else if (1 == _at)
				{
					m_sData = pReader->GetString2();
				}
				else if (2 == _at)
				{
					m_oDxaOrig = pReader->GetLong();
				}
				else if (3 == _at)
				{
					m_oDyaOrig = pReader->GetLong();
				}
				else if (4 == _at)
				{
					m_oDrawAspect = new Limit::OLEDrawAspectType();
					m_oDrawAspect->SetBYTECode(pReader->GetUChar());
				}
				else if (5 == _at)
				{
					m_oType = new Limit::OLEType();
					m_oType->SetBYTECode(pReader->GetUChar());
				}
				else if (6 == _at)
				{
					m_oUpdateMode = new Limit::OLEUpdateMode();
					m_oUpdateMode->SetBYTECode(pReader->GetUChar());
				}
				else
					break;
			}

			pReader->Seek(_end_rec);
		}
		void COLEObject::FillParentPointersForChilds()
		{
		}
		bool COLEObject::isValid()
		{
			return m_sProgId.IsInit() && (m_sData.IsInit() || m_oId.IsInit() || m_sFilepathBin.IsInit());
		}

		CString COLEObject::GetFullOleName(const PPTX::RId& oRId, FileContainer* pRels)const
		{
			if (pRels != NULL)
			{
				smart_ptr<PPTX::OleObject> p = pRels->oleObject(oRId);
				if (p.is_init())
					return p->filename().m_strFilename;
			}

			if(parentFileIs<Slide>())
				return parentFileAs<Slide>().GetOleFromRId(oRId);
			else if(parentFileIs<SlideLayout>())
				return parentFileAs<SlideLayout>().GetOleFromRId(oRId);
			else if(parentFileIs<SlideMaster>())
				return parentFileAs<SlideMaster>().GetOleFromRId(oRId);
			else if(parentFileIs<Theme>())
				return parentFileAs<Theme>().GetOleFromRId(oRId);
			return _T("");
		}
		std::wstring COLEObject::GetOleData(const std::wstring& sFilePath)const
		{
			std::wstring sRes;
			//EncodingMode.unparsed https://github.com/tonyqus/npoi/blob/master/main/POIFS/FileSystem/Ole10Native.cs
			POLE::Storage oStorage(sFilePath.c_str());
			if(oStorage.open(false, false))
			{
				POLE::Stream oStream(&oStorage, "Ole10Native");
				if(oStream.size() > 4)
				{
					BYTE head[] = {0x00, 0x00, 0x00, 0x00};
					oStream.read(head, 4);
					_UINT32 nDataSize = (_UINT32)((head[0]<< 0) | ((head[1]) << 8) | ((head[2]) << 16) | ((head[3]) << 24));
					BYTE* aData = new BYTE[nDataSize];
					oStream.read(aData, nDataSize);
					sRes = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(aData, nDataSize);
					RELEASEARRAYOBJECTS(aData);
				}
			}
			return sRes;
		}

		Pic::Pic()
		{
		}


		Pic::~Pic()
		{
		}
	

		Pic::Pic(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}


		const Pic& Pic::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Pic::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode item;
					oNodes.GetAt(i, item);

					CString strName = XmlUtils::GetNameNoNS(item.GetName());

					if (_T("nvPicPr") == strName)
						nvPicPr = item;
					else if (_T("blipFill") == strName)
						blipFill = item;
					else if (_T("spPr") == strName)
						spPr = item;
					else if (_T("style") == strName)
					{
						if (!style.IsInit())
							style = item;
					}
				}

				if (!blipFill.blip.is_init())
				{
					XmlUtils::CXmlNode oNodeMSAlternate;
					if (node.GetNode(_T("mc:AlternateContent"), oNodeMSAlternate))
					{
						XmlUtils::CXmlNode oNodeFallback;
						if (oNodeMSAlternate.GetNode(_T("mc:Fallback"), oNodeFallback))
						{
							blipFill = oNodeFallback.ReadNode(_T("p:blipFill"));
						}
					}
				}
			}			
			
			FillParentPointersForChilds();
		}


		CString Pic::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(nvPicPr);
			oValue.Write(blipFill);
			oValue.Write(spPr);
			oValue.WriteNullable(style);

			return XmlUtils::CreateNode(_T("p:pic"), oValue);
		}

		void Pic::FillParentPointersForChilds()
		{
			nvPicPr.SetParentPointer(this);
			blipFill.SetParentPointer(this);
			spPr.SetParentPointer(this);
			if (style.IsInit())
				style->SetParentPointer(this);
		}

		void Pic::GetRect(Aggplus::RECT& pRect)const
		{
			pRect.bottom	= 0;
			pRect.left		= 0;
			pRect.right		= 0;
			pRect.top		= 0;

			if (spPr.xfrm.IsInit())
			{
				pRect.left		= spPr.xfrm->offX.get_value_or(0);
				pRect.top		= spPr.xfrm->offY.get_value_or(0);
				pRect.right		= pRect.left + spPr.xfrm->extX.get_value_or(0);
				pRect.bottom	= pRect.top + spPr.xfrm->extY.get_value_or(0);
			}
			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(pRect);
		}

		CString Pic::GetFullPicName()const
		{
			if (blipFill.blip.IsInit())
				return blipFill.blip->GetFullPicName();
			return _T("");
		}

		CString Pic::GetVideoLink()const
		{
			CString file = _T("");
			if (parentFileIs<Slide>())
			{
				if (nvPicPr.nvPr.media.is<MediaFile>())
				{
					if ((nvPicPr.nvPr.media.as<MediaFile>().name == _T("videoFile")) || (nvPicPr.nvPr.media.as<MediaFile>().name == _T("quickTimeFile")))
					{
						file = parentFileAs<Slide>().GetLinkFromRId(nvPicPr.nvPr.media.as<MediaFile>().link.get());						
						if (CString (_T("NULL")) == file)	//	HAVE TRIM
						{
							if(nvPicPr.nvPr.extLst.size())
							{
								file = parentFileAs<Slide>().GetLinkFromRId(nvPicPr.nvPr.extLst[0].link.get());
							}
						}		
					}
				}
			}

			return file;
		}

		CString Pic::GetAudioLink()const
		{
			CString file = _T("");
			if (parentFileIs<Slide>())
			{
				if (nvPicPr.nvPr.media.is<WavAudioFile>())
				{
					return parentFileAs<Slide>().GetLinkFromRId(nvPicPr.nvPr.media.as<WavAudioFile>().embed.get());
				}

				if (nvPicPr.nvPr.media.is<MediaFile>())
				{
					if (nvPicPr.nvPr.media.as<MediaFile>().name == _T("audioFile"))
					{
						file = parentFileAs<Slide>().GetLinkFromRId(nvPicPr.nvPr.media.as<MediaFile>().link.get());		

						if (CString (_T("NULL")) == file)	//	HAVE TRIM
						{
							if(nvPicPr.nvPr.extLst.size())
							{
								file = parentFileAs<Slide>().GetLinkFromRId(nvPicPr.nvPr.extLst[0].link.get());
							}
						}		
					}
				}
			}
			return file;
		}

		DWORD Pic::GetFill(UniFill& fill)const
		{
			DWORD BGRA = 0;
			fill.SetParentFilePointer(parentFile);

			if (style.IsInit())
			{
				if (parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if (parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if (parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);

				BGRA = style->fillRef.Color.GetBGRA();
			}

			if (spPr.Fill.is_init())
				spPr.Fill.Merge(fill);
			return BGRA;
		}

		DWORD Pic::GetLine(Ln& line)const
		{
			DWORD BGRA = 0;
			line.SetParentFilePointer(parentFile);

			if (style.is_init())
			{
				if (parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if (parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if (parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);

				BGRA = style->lnRef.Color.GetBGRA();
			}

			if (spPr.ln.IsInit())
				spPr.ln->Merge(line);
			return BGRA;
		}

		
		double Pic::GetStTrim () const
		{
			double trim = 0.0;
			
			if (parentFileIs<Slide>())
			{
				if (nvPicPr.nvPr.media.is<MediaFile>())
				{
					if ((nvPicPr.nvPr.media.as<MediaFile>().name == _T("videoFile")) ||
						(nvPicPr.nvPr.media.as<MediaFile>().name == _T("quickTimeFile")) ||
						(nvPicPr.nvPr.media.as<MediaFile>().name == _T("audioFile")) )
					{
						if (CString (_T("NULL")) == parentFileAs<Slide>().GetLinkFromRId(nvPicPr.nvPr.media.as<MediaFile>().link.get()) )	//	HAVE TRIM
						{
							if(nvPicPr.nvPr.extLst.size())
							{
								if (nvPicPr.nvPr.extLst[0].st.is_init())
									trim = nvPicPr.nvPr.extLst[0].st.get();
							}
						}		
					}
				}
			}

			return trim;
		}

		double Pic::GetEndTrim () const
		{
			double trim = -1.0;
			
			if (parentFileIs<Slide>())
			{
				if (nvPicPr.nvPr.media.is<MediaFile>())
				{
					if ((nvPicPr.nvPr.media.as<MediaFile>().name == _T("videoFile")) ||
						(nvPicPr.nvPr.media.as<MediaFile>().name == _T("quickTimeFile")) ||
						(nvPicPr.nvPr.media.as<MediaFile>().name == _T("audioFile")) )
					{
						if (CString (_T("NULL")) == parentFileAs<Slide>().GetLinkFromRId(nvPicPr.nvPr.media.as<MediaFile>().link.get()))	//	HAVE TRIM
						{
							if(nvPicPr.nvPr.extLst.size())
							{
								if (nvPicPr.nvPr.extLst[0].end.is_init())
									trim = nvPicPr.nvPr.extLst[0].end.get();
							}
						}		
					}
				}
			}

			return trim;
		}

		long Pic::GetRefId() const
		{
			return (long) nvPicPr.cNvPr.id;
		}

		void Pic::toXmlWriterVML(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::WrapperFile>& _oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& _oClrMap)
		{
			smart_ptr<PPTX::Theme> oTheme = _oTheme.smart_dynamic_cast<PPTX::Theme>();
			smart_ptr<PPTX::Logic::ClrMap> oClrMap = oTheme.smart_dynamic_cast<PPTX::Logic::ClrMap>();

			bool bOle = oleObject.IsInit() && oleObject->isValid();
			CString sOleNodeName;
			if (XMLWRITER_DOC_TYPE_XLSX != pWriter->m_lDocType)
			{
				if(bOle && oleObject->m_oDxaOrig.IsInit() && oleObject->m_oDyaOrig.IsInit())
				{
					sOleNodeName = L"w:object";
					pWriter->StartNode(sOleNodeName);
					pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("w:dxaOrig"), oleObject->m_oDxaOrig);
					pWriter->WriteAttribute(_T("w:dyaOrig"), oleObject->m_oDyaOrig);
					pWriter->EndAttributes();
				}
				else
				{
					sOleNodeName = L"w:pict";
					pWriter->StartNode(sOleNodeName);
					pWriter->StartAttributes();
					pWriter->EndAttributes();
				}
			}

			int dL = 0;
			int dT = 0;
			int dW = 0;
			int dH = 0;

			int nShapeId = pWriter->m_lObjectIdVML;
			CString strId = _T("");
			strId.Format(_T("_x0000_i%04d"), nShapeId);
			CString strSpid = _T("");
			strSpid.Format(_T("_x%04d_s%04d"), 0xFFFF & (pWriter->m_lObjectIdVML >> 16), 0xFFFF & pWriter->m_lObjectIdVML);
			CString strObjectid = _T("");
			strObjectid.Format(_T("_152504%04d"), pWriter->m_lObjectIdVML);
			pWriter->m_lObjectIdVML++;

			NSBinPptxRW::CXmlWriter oStylesWriter;

			if(_T("") == pWriter->m_strStyleMain)
			{
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
			}

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

			if (spPr.Geometry.is_init())
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

#ifdef AVS_USE_CONVERT_PPTX_TOCUSTOM_VML
				spPr.Geometry.ConvertToCustomVML(pWriter->m_pOOXToVMLRenderer, strPath, strTextRect, lW, lH);
#endif

				pWriter->StartNode(_T("v:shape"));

				if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
				{
					pWriter->WriteAttribute(_T("id"), strSpid);
				}
				else
				{
					pWriter->WriteAttribute(_T("id"), strId);
					pWriter->WriteAttribute(_T("o:spid"), strSpid);
				}

				pWriter->StartAttributes();
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
				if(bOle)
				{
					pWriter->WriteAttribute(_T("filled"), (CString)L"f");
				}
				CString strNodeVal = _T("");
				if (!spPr.ln.is_init())
				{
					pWriter->WriteAttribute(_T("stroked"), (CString)_T("false"));
				}
				else
				{
					CString strPenAttr = _T("");
					nullable<ShapeStyle> pShapeStyle;
					CalculateLine(spPr, pShapeStyle, _oTheme, _oClrMap, strPenAttr, strNodeVal, bOle);
					pWriter->WriteString(strPenAttr);
				}

				pWriter->EndAttributes();

				pWriter->StartNode(_T("v:path"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("textboxrect"), strTextRect);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("v:path"));

				if (blipFill.blip.is_init() && blipFill.blip->embed.is_init())
				{
					pWriter->StartNode(_T("v:imagedata"));
					pWriter->StartAttributes();
					if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
					{
						pWriter->WriteAttribute(_T("o:relid"), blipFill.blip->embed->ToString());
					}
					else
					{
						pWriter->WriteAttribute(_T("r:id"), blipFill.blip->embed->ToString());
					}
					pWriter->WriteAttribute(_T("o:title"), CString(_T("")));
					pWriter->EndAttributes();
					pWriter->EndNode(_T("v:imagedata"));
				}

				pWriter->EndNode(_T("v:shape"));
			}
			else
			{
				pWriter->StartNode(_T("v:rect"));

				pWriter->StartAttributes();

				if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
				{
					pWriter->WriteAttribute(_T("id"), strSpid);
				}
				else
				{
					pWriter->WriteAttribute(_T("id"), strId);
					pWriter->WriteAttribute(_T("o:spid"), strSpid);
				}

				if (oStylesWriter.GetSize() == 0)
				{
					pWriter->WriteAttribute(_T("style"), pWriter->m_strStyleMain);
				}
				else
				{
					pWriter->WriteAttribute(_T("style"), pWriter->m_strStyleMain + oStylesWriter.GetXmlString());
				}
				if(bOle)
				{
					pWriter->WriteAttribute(_T("filled"), L"f");
					pWriter->WriteAttribute(_T("stroked"), L"f");
				}

				pWriter->EndAttributes();

				if (blipFill.blip.is_init() && blipFill.blip->embed.is_init())
				{
					pWriter->StartNode(_T("v:imagedata"));
					pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("r:id"), blipFill.blip->embed->ToString());
					pWriter->WriteAttribute(_T("o:title"), CString(_T("")));
					pWriter->EndAttributes();
					pWriter->EndNode(_T("v:imagedata"));
				}

				pWriter->EndNode(_T("v:rect"));
			}
			pWriter->m_strStyleMain = _T("");

			if(bOle)
			{
				oleObject->m_sObjectId = strObjectid;
				if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
				{
					oleObject->m_sShapeId = std_string2string(std::to_wstring(nShapeId));
					NSBinPptxRW::CXmlWriter oTempWriter;
					oleObject->toXmlWriterXlsx(&oTempWriter);
					pWriter->m_strOleXlsx = oTempWriter.GetXmlString();
				}
				else
				{
					oleObject->m_sShapeId = strId;
					oleObject->toXmlWriter(pWriter);
				}
			}
			if (XMLWRITER_DOC_TYPE_XLSX != pWriter->m_lDocType)
			{
				pWriter->EndNode(sOleNodeName);
			}
		}
	} // namespace Logic
} // namespace PPTX
