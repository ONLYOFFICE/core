/*
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

		std::wstring COLEObject::toXML() const
		{
			return L"";
		}
		void COLEObject::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring strName = L"o:OLEObject";
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
			std::wstring strName = L"oleObject";
			pWriter->StartNode(strName);

			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"progId", m_sProgId);
			if(m_oDrawAspect.IsInit())
			{
				std::wstring sDrawAspect;
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
				std::wstring sUpdateMode;
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
			if((m_oId.IsInit() || m_sFilepathBin.IsInit()) && m_sProgId.IsInit() && 0 == m_sProgId.get().find(L"asc."))
			{
				std::wstring sFilePath;
				if (m_sFilepathBin.IsInit())
				{
					sFilePath = m_sFilepathBin.get();
				}
				else if (m_oId.IsInit())
				{
					FileContainer* pRels = NULL;
					if (pWriter->m_pCommonRels->is_init())
						pRels = pWriter->m_pCommonRels->operator ->();

					sFilePath = this->GetFullOleName(OOX::RId(m_oId.get()), pRels);
				}
                if(!sFilePath.empty())
				{
					sData = GetOleData(sFilePath);
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

		std::wstring COLEObject::GetFullOleName(const OOX::RId& oRId, FileContainer* pRels)const
		{
			if (pRels != NULL)
			{
				smart_ptr<OOX::OleObject> p = pRels->GetOleObject(oRId);
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

					std::wstring strName = XmlUtils::GetNameNoNS(item.GetName());

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


		std::wstring Pic::toXML() const
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
			if (oleObject.IsInit())
				oleObject->SetParentPointer(this);
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

		std::wstring Pic::GetFullPicName()const
		{
			if (blipFill.blip.IsInit())
				return blipFill.blip->GetFullPicName();
			return _T("");
		}

		std::wstring Pic::GetVideoLink()const
		{
			std::wstring file = _T("");
			if (parentFileIs<Slide>())
			{
				if (nvPicPr.nvPr.media.is<MediaFile>())
				{
					if ((nvPicPr.nvPr.media.as<MediaFile>().name == _T("videoFile")) || (nvPicPr.nvPr.media.as<MediaFile>().name == _T("quickTimeFile")))
					{
						file = parentFileAs<Slide>().GetLinkFromRId(nvPicPr.nvPr.media.as<MediaFile>().link.get());						
						if (std::wstring (_T("NULL")) == file)	//	HAVE TRIM
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

		std::wstring Pic::GetAudioLink()const
		{
			std::wstring file = _T("");
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

						if (std::wstring (_T("NULL")) == file)	//	HAVE TRIM
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
						if (std::wstring (_T("NULL")) == parentFileAs<Slide>().GetLinkFromRId(nvPicPr.nvPr.media.as<MediaFile>().link.get()) )	//	HAVE TRIM
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
						if (std::wstring (_T("NULL")) == parentFileAs<Slide>().GetLinkFromRId(nvPicPr.nvPr.media.as<MediaFile>().link.get()))	//	HAVE TRIM
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

		void Pic::toXmlWriterVML(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::WrapperFile>& _oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& _oClrMap, bool in_group)
		{
			smart_ptr<PPTX::Theme> oTheme = _oTheme.smart_dynamic_cast<PPTX::Theme>();
			smart_ptr<PPTX::Logic::ClrMap> oClrMap = oTheme.smart_dynamic_cast<PPTX::Logic::ClrMap>();

			bool bOle = oleObject.IsInit() && oleObject->isValid();
			std::wstring sOleNodeName;
			
			if (XMLWRITER_DOC_TYPE_XLSX != pWriter->m_lDocType)
			{
				if(bOle && oleObject->m_oDxaOrig.IsInit() && oleObject->m_oDyaOrig.IsInit())
				{
					sOleNodeName = L"w:object";
					pWriter->StartNode(sOleNodeName);
					pWriter->StartAttributes();
					pWriter->WriteAttribute(L"w:dxaOrig", oleObject->m_oDxaOrig);
					pWriter->WriteAttribute(L"w:dyaOrig", oleObject->m_oDyaOrig);
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

			int nShapeId = pWriter->m_lObjectIdVML;
            std::wstring strId			= L"_x0000_i" + std::to_wstring(nShapeId);
            std::wstring strSpid		= L"_x" + std::to_wstring(0xFFFF & (pWriter->m_lObjectIdVML >> 16)) + L"_s" + std::to_wstring(0xFFFF & pWriter->m_lObjectIdVML);
            std::wstring strObjectid	= L"_152504" + std::to_wstring(pWriter->m_lObjectIdVML);
			pWriter->m_lObjectIdVML++;

			int dL = 0, dT = 0, dW = 0, dH = 0;
			if (spPr.xfrm.is_init())
			{
				if (spPr.xfrm->offX.is_init())	dL = *spPr.xfrm->offX;
				if (spPr.xfrm->offY.is_init())	dT = *spPr.xfrm->offY;
				if (spPr.xfrm->extX.is_init())	dW = *spPr.xfrm->extX;
				if (spPr.xfrm->extY.is_init())	dH = *spPr.xfrm->extY;
			}		
			
			NSBinPptxRW::CXmlWriter oStylesWriter;

            if(pWriter->m_strStyleMain.empty())
			{
				oStylesWriter.WriteAttributeCSS (L"position", L"absolute");
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

			if (spPr.Geometry.is_init())
			{
                std::wstring strPath;
                std::wstring strTextRect;

				LONG lW = 43200;
				LONG lH = 43200;
				if (spPr.xfrm.is_init())
				{
					lW = spPr.xfrm->extX.get_value_or(43200);
					lH = spPr.xfrm->extY.get_value_or(43200);
				}

				spPr.Geometry.ConvertToCustomVML(pWriter->m_pOOXToVMLRenderer, strPath, strTextRect, lW, lH);

				pWriter->StartNode(L"v:shape");

				if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
				{
					pWriter->WriteAttribute(L"id", strSpid);
				}
				else
				{
					pWriter->WriteAttribute(L"id", strId);
					pWriter->WriteAttribute(L"o:spid", strSpid);
				}

				pWriter->StartAttributes();
				if (oStylesWriter.GetSize() == 0)
				{
					pWriter->WriteAttribute(L"style", pWriter->m_strStyleMain);
				}
				else
				{
					pWriter->WriteAttribute(L"style", pWriter->m_strStyleMain + oStylesWriter.GetXmlString());
				}

				if(!bOle)
				{
					oStylesWriter.ClearNoAttack();
					oStylesWriter.m_oWriter.AddSize(30);
					oStylesWriter.m_oWriter.AddIntNoCheck(dW / 100);
					oStylesWriter.m_oWriter.AddCharNoCheck(WCHAR(','));
					oStylesWriter.m_oWriter.AddIntNoCheck(dH / 100);
					pWriter->WriteAttribute(L"coordsize", oStylesWriter.GetXmlString());

					pWriter->WriteAttribute(L"path", strPath);
				}

				if (!pWriter->m_strAttributesMain.empty())
				{
					pWriter->WriteString(pWriter->m_strAttributesMain);
					pWriter->m_strAttributesMain.clear();
				}
				if(bOle)
				{
					pWriter->WriteAttribute(L"filled", L"f");
				}
				std::wstring strNodeVal;
				if (!spPr.ln.is_init())
				{
					pWriter->WriteAttribute(L"stroked", L"false");
				}
				else
				{
					std::wstring strPenAttr = _T("");
					nullable<ShapeStyle> pShapeStyle;
					CalculateLine(spPr, pShapeStyle, _oTheme, _oClrMap, strPenAttr, strNodeVal, bOle);
					pWriter->WriteString(strPenAttr);
				}

				pWriter->EndAttributes();

				pWriter->StartNode(L"v:path");
				pWriter->StartAttributes();
				pWriter->WriteAttribute(L"textboxrect", strTextRect);
				pWriter->EndAttributes();
				pWriter->EndNode(L"v:path");

				if (blipFill.blip.is_init() && blipFill.blip->embed.is_init())
				{
					pWriter->StartNode(L"v:imagedata");
					pWriter->StartAttributes();
					if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
					{
						pWriter->WriteAttribute(L"o:relid", blipFill.blip->embed->ToString());
					}
					else
					{
						pWriter->WriteAttribute(L"r:id", blipFill.blip->embed->ToString());
					}
					pWriter->WriteAttribute(L"o:title", L"");
					pWriter->EndAttributes();
					pWriter->EndNode(L"v:imagedata");
				}

				pWriter->EndNode(L"v:shape");
			}
			else
			{
				pWriter->StartNode(L"v:rect");

				pWriter->StartAttributes();

				if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
				{
					pWriter->WriteAttribute(L"id", strSpid);
				}
				else
				{
					pWriter->WriteAttribute(L"id", strId);
					pWriter->WriteAttribute(L"o:spid", strSpid);
				}

				if (oStylesWriter.GetSize() == 0)
				{
					pWriter->WriteAttribute(L"style", pWriter->m_strStyleMain);
				}
				else
				{
					pWriter->WriteAttribute(L"style", pWriter->m_strStyleMain + oStylesWriter.GetXmlString());
				}
				if(bOle)
				{
					pWriter->WriteAttribute(L"filled", L"f");
					pWriter->WriteAttribute(L"stroked", L"f");
				}

				pWriter->EndAttributes();

				if (blipFill.blip.is_init() && blipFill.blip->embed.is_init())
				{
					pWriter->StartNode(L"v:imagedata");
					pWriter->StartAttributes();
					pWriter->WriteAttribute(L"r:id", blipFill.blip->embed->ToString());
					pWriter->WriteAttribute(L"o:title", L"");
					pWriter->EndAttributes();
					pWriter->EndNode(L"v:imagedata");
				}

				pWriter->EndNode(L"v:rect");
			}
			pWriter->m_strStyleMain.clear();

			if(bOle)
			{
				oleObject->m_sObjectId = strObjectid;
				if (XMLWRITER_DOC_TYPE_XLSX == pWriter->m_lDocType)
				{
					oleObject->m_sShapeId = std::to_wstring(nShapeId);
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
		void Pic::fromXMLOle(XmlUtils::CXmlNode& node)
		{
			oleObject.Init();
			node.ReadAttributeBase(L"progId", oleObject->m_sProgId);
			node.ReadAttributeBase(L"r:id", oleObject->m_oId);
			int imgW = node.GetAttributeInt(std::wstring(L"imgW"), 0);
			if(imgW > 0)
			{
				oleObject->m_oDxaOrig = (int)Emu_To_Twips(imgW);
			}
			int imgH = node.GetAttributeInt(std::wstring(L"imgH"), 0);
			if(imgH > 0)
			{
				oleObject->m_oDyaOrig = (int)Emu_To_Twips(imgH);
			}
			if(oleObject->m_oId.IsInit())
			{
				blipFill.blip->oleRid = oleObject->m_oId.get().ToString();
			}
		}
	} // namespace Logic
} // namespace PPTX
