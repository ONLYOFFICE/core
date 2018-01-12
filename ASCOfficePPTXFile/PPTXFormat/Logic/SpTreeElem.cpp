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


#include "SpTreeElem.h"
#include "Shape.h"
#include "Pic.h"
#include "CxnSp.h"
#include "SpTree.h"
#include "GraphicFrame.h"

#include "../SlideMaster.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Media/Audio.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Media/Video.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Media/ActiveX.h"

namespace PPTX
{
	namespace Logic
	{
		std::wstring GetHexColor(const DWORD& ARGB)
		{
			BYTE r = (BYTE)((ARGB >> 16) & 0xFF);
			BYTE g = (BYTE)((ARGB >> 8) & 0xFF);
			BYTE b = (BYTE)((ARGB & 0xFF));

            std::wstringstream sstream;
            sstream << boost::wformat( L"%02X%02X%02X" ) % r % g % b;

            return L"#" + sstream.str();
		}

        void CalculateFill(PPTX::Logic::SpPr& oSpPr, nullable<ShapeStyle>& pShapeStyle, NSCommon::smart_ptr<PPTX::Theme>& oTheme,
			NSCommon::smart_ptr<PPTX::Logic::ClrMap>& oClrMap, std::wstring& strAttr, std::wstring& strNode, bool bOle, bool bSignature)
		{
			PPTX::Logic::UniFill fill;
			DWORD ARGB = 0;
			if (pShapeStyle.is_init() && oTheme.is_init())
			{
				oTheme->GetFillStyle(pShapeStyle->fillRef.idx.get_value_or(0), fill);

				if (pShapeStyle->fillRef.Color.is_init())
				{
					if (fill.is<PPTX::Logic::SolidFill>())
					{
						fill.as<PPTX::Logic::SolidFill>().Color = pShapeStyle->fillRef.Color;
					}
				}
				ARGB = pShapeStyle->fillRef.Color.GetRGBColor(oTheme, oClrMap, ARGB);
			}

			if (oSpPr.Fill.is_init())
				oSpPr.Fill.Merge(fill);

			if (fill.is<BlipFill>())
			{
				BlipFill& oBlip = fill.as<BlipFill>();

				if (oBlip.blip.is_init() && oBlip.blip->embed.is_init())
				{
					std::wstring fopacity = _T("");
					size_t eff_count = oBlip.blip->Effects.size();
					for (size_t eff = 0; eff < eff_count; ++eff)
					{

						if (oBlip.blip->Effects[eff].is<PPTX::Logic::AlphaModFix>())
						{
							PPTX::Logic::AlphaModFix& oAlpha = oBlip.blip->Effects[eff].as<PPTX::Logic::AlphaModFix>();
							if (oAlpha.amt.is_init())
							{
								double dA = (double)(oAlpha.amt.get());
								dA = (dA * 65536 / 100000);
								int nA = (int)(dA + 0.5);

								if (nA < 0)
									nA = 0;
								if (nA > 65536)
									nA = 65536;

                                fopacity = _T(" opacity=\"") + std::to_wstring(nA) + _T("f\"");
							}
							break;
						}
					}

					std::wstring strId = oBlip.blip->embed->ToString();

					if (bOle || bSignature)
					{
						strAttr = _T(" filled=\"f\"");
						strNode = _T("<v:imagedata r:id=\"") + strId + _T("\" o:title=\"\" />");
					}
					else
					{
						if (oBlip.tile.is_init())
							strNode = _T("<v:fill r:id=\"") + strId + _T("\" o:title=\"\" type=\"tile\"") + fopacity + _T(" />");
						else
							strNode = _T("<v:fill r:id=\"") + strId + _T("\" o:title=\"\" type=\"frame\"") + fopacity + _T(" />");
					}
				}				
			}
			else if (fill.is<SolidFill>())
			{
				ARGB = fill.as<SolidFill>().Color.GetRGBColor(oTheme, oClrMap, ARGB);
				strAttr = _T(" fillcolor=\"") + GetHexColor(ARGB) + _T("\"");

				BYTE A = (BYTE)((ARGB >> 24) & 0xFF);
				if (A != 255)
				{
					int fopacity = 100 - (int)(((double)A / 255.0) * 65536);
                    strNode = _T("<v:fill opacity=\"") + std::to_wstring(fopacity) + _T("f\" />");
				}
			}
			else if (fill.is<GradFill>())
			{
				GradFill& oGrad = fill.as<GradFill>();
				if (oGrad.GsLst.size() > 0)
				{
					ARGB = oGrad.GsLst[0].color.GetRGBColor(oTheme, oClrMap, ARGB);
					strAttr = _T(" fillcolor=\"") + GetHexColor(ARGB) + _T("\"");

					BYTE A = (BYTE)((ARGB >> 24) & 0xFF);
					if (A != 255)
					{
						int fopacity = 100 - (int)(((double)A / 255.0) * 65536);
                        strNode = _T("<v:fill opacity=\"") + std::to_wstring(fopacity) + _T("f\" />");
					}
				}
			}
			else if (fill.is<NoFill>() || !fill.is_init())
			{
				strAttr = _T(" filled=\"f\"");
			}
			else
			{
				strAttr = _T(" fillcolor=\"") + GetHexColor(ARGB) + _T("\"");

				BYTE A = (BYTE)((ARGB >> 24) & 0xFF);
				if (A != 255)
				{
					int fopacity = (int)(((double)A / 255.0) * 65536);
                    strNode = _T("<v:fill opacity=\"") + std::to_wstring(fopacity) + _T("f\" />");
				}
			}

			/*
			BYTE alpha = (BYTE)((ARGB >> 24) & 0xFF);
			if (alpha < 255)
			{
                std::wstring strA =  = std::to_string( alpha);
				strAttr += _T(" opacity=\"") + strA + _T("\"");
			}
			*/
		}
        void CalculateLine(PPTX::Logic::SpPr& oSpPr, nullable<ShapeStyle>& pShapeStyle, NSCommon::smart_ptr<PPTX::Theme>& oTheme,
			NSCommon::smart_ptr<PPTX::Logic::ClrMap>& oClrMap, std::wstring& strAttr, std::wstring& strNode, bool bOle, bool bSignature)
		{
			PPTX::Logic::Ln line;
			DWORD ARGB = 0;
			if (pShapeStyle.is_init() && oTheme.is_init())
			{
				oTheme->GetLineStyle(pShapeStyle->lnRef.idx.get_value_or(0), line);
				ARGB = pShapeStyle->lnRef.Color.GetRGBColor(oTheme, oClrMap, ARGB);
			}

			if (oSpPr.ln.is_init())
				oSpPr.ln->Merge(line);

			if (line.Fill.is<SolidFill>())
			{
				ARGB = line.Fill.as<SolidFill>().Color.GetRGBColor(oTheme, oClrMap, ARGB);
				strAttr = _T(" strokecolor=\"") + GetHexColor(ARGB) + _T("\"");
			}
			else if (bOle || bSignature)
				strAttr = _T(" stroked=\"f\"");

			if (line.w.is_init())
			{
				double dW = 72.0 * (*line.w) / (25.4 * 36000);
                std::wstring s = L" strokeweight=\"" + XmlUtils::DoubleToString(dW, L"%.2lf") + L"pt\"";
				strAttr += s;
			}
		}

		SpTreeElem::SpTreeElem()
		{
		}
		SpTreeElem::~SpTreeElem()
		{
		}
		SpTreeElem::SpTreeElem(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}
		const SpTreeElem& SpTreeElem::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}
		SpTreeElem::SpTreeElem(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
		}
		const SpTreeElem& SpTreeElem::operator =(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader);
			return *this;
		}
		void SpTreeElem::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring name = XmlUtils::GetNameNoNS(oReader.GetName());

			if (name == _T("sp") || name == _T("wsp"))
				m_elem.reset(new Logic::Shape(oReader));
			else if (name == _T("pic"))
				m_elem.reset(new Logic::Pic(oReader));
			else if (name == _T("cxnSp"))
				m_elem.reset(new Logic::CxnSp(oReader));
			else if (name == _T("grpSp") || name == _T("wgp") || name == _T("spTree") || name == _T("lockedCanvas") || name == _T("wpc"))
				m_elem.reset(new Logic::SpTree(oReader));
			else if (name == _T("graphicFrame"))
			{
				Logic::GraphicFrame *pGraphic = new Logic::GraphicFrame(oReader);

				if (pGraphic && pGraphic->IsEmpty() == false)
					m_elem.reset(pGraphic);
				else
					RELEASEOBJECT(pGraphic);
			}
			else if (name == _T("AlternateContent"))
			{
				if ( oReader.IsEmptyNode() )
					return;
				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring strName = oReader.GetName();
					
					if ( oReader.IsEmptyNode() )
						continue;
					
					if (strName == L"mc:Choice")
					{
						oReader.ReadNextSiblingNode(nCurDepth + 1);
						//GetAttributeIfExist(L"Requires", sRequires) && L"a14" == sRequires)
						fromXML(oReader);
						
						if (m_elem.is_init())
							break;
					}
					else if (strName == L"mc:Fallback")
					{
						oReader.ReadNextSiblingNode(nCurDepth + 1);
						fromXML(oReader);
					}
				}
			}
		}
		void SpTreeElem::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("sp") || name == _T("wsp"))
				m_elem.reset(new Logic::Shape(node));
			else if (name == _T("pic"))
				m_elem.reset(new Logic::Pic(node));
			else if (name == _T("cxnSp"))
				m_elem.reset(new Logic::CxnSp(node));
			else if (name == _T("grpSp") || name == _T("wgp") || name == _T("spTree") || name == _T("lockedCanvas") || name == _T("wpc"))
				m_elem.reset(new Logic::SpTree(node));
			else if (name == _T("graphicFrame"))
				m_elem.reset(new Logic::GraphicFrame(node));
			else if (name == _T("AlternateContent"))
			{
				bool isEmpty = true;
				XmlUtils::CXmlNode oNodeChoice;
				if (node.GetNode(_T("mc:Choice"), oNodeChoice))
				{
					XmlUtils::CXmlNode oNodeFall;
					XmlUtils::CXmlNodes oNodesC;
					std::wstring sRequires;
					//todo better check (a14 can be math, slicer)
					if(oNodeChoice.GetAttributeIfExist(L"Requires", sRequires) && L"a14" == sRequires)
					{
						oNodeChoice.GetNodes(_T("*"), oNodesC);
					}
					else if (node.GetNode(_T("mc:Fallback"), oNodeFall))
					{
						oNodeFall.GetNodes(_T("*"), oNodesC);
					}
					if (1 == oNodesC.GetCount())
					{
						XmlUtils::CXmlNode oNodeC;
						oNodesC.GetAt(0, oNodeC);

						fromXML(oNodeC);
						isEmpty = false;
					}
				}
				if(isEmpty)
				{
					m_elem.reset();	
				}
				
			}
			else m_elem.reset();
		}

		void SpTreeElem::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			BYTE _type = pReader->GetUChar();

			switch (_type)
			{
			case SPTREE_TYPE_SHAPE:
				{
					Logic::Shape* p = new Logic::Shape();
					p->fromPPTY(pReader);
					m_elem.reset(p);
					break;
				}
			case SPTREE_TYPE_OLE:
			case SPTREE_TYPE_PIC:
			case SPTREE_TYPE_AUDIO:
			case SPTREE_TYPE_VIDEO:
				{
					Logic::Pic* p = new Logic::Pic();

					if (_type == SPTREE_TYPE_AUDIO)	
					{
						OOX::Audio *pAudio = new OOX::Audio(pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);
						p->blipFill.additionalFile = smart_ptr<OOX::File>(dynamic_cast<OOX::File*>(pAudio));
					}
					else if (_type == SPTREE_TYPE_VIDEO)
					{
						OOX::Video* pVideo = new OOX::Video(pReader->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX);
						p->blipFill.additionalFile = smart_ptr<OOX::File>(dynamic_cast<OOX::File*>(pVideo));
					}
					
					p->fromPPTY(pReader);
					m_elem.reset(p);
					break;
				}
			case SPTREE_TYPE_CXNSP:
				{
					Logic::CxnSp* p = new Logic::CxnSp();
					p->fromPPTY(pReader);
					m_elem.reset(p);
					break;
				}
			case SPTREE_TYPE_SPTREE:
				{
					Logic::SpTree* p = new Logic::SpTree();
					pReader->Seek(pReader->GetPos() - 5); // type back + len
					p->fromPPTY(pReader);

					if (getType() == OOX::et_p_ShapeTree)
					{
                        smart_ptr<PPTX::Logic::SpTree> parent = GetElem().smart_dynamic_cast<PPTX::Logic::SpTree>();
						p->m_lGroupIndex = parent->m_lGroupIndex + 1;
					}
					m_elem.reset(p);
					break;
				}
			case SPTREE_TYPE_GRFRAME:
				{
					Logic::GraphicFrame* pFrame = new Logic::GraphicFrame();
					pFrame->fromPPTY(pReader);
					m_elem.reset(pFrame);					
					break;
				}
			case SPTREE_TYPE_NONE:
				{
					pReader->SkipRecord();
					break;
				}
			default:
				break;
			}
		}

		std::wstring SpTreeElem::toXML() const
		{
			if (m_elem.IsInit())
				return m_elem->toXML();
			return _T("");
		}

		void SpTreeElem::toXmlWriterVML	(NSBinPptxRW::CXmlWriter* pWriter, smart_ptr<PPTX::Theme>& oTheme, smart_ptr<PPTX::Logic::ClrMap>& oClrMap) const
		{
			if (m_elem.IsInit() == false) return;

			switch(m_elem->getType())
			{
				case OOX::et_a_Shape:
				{
					smart_ptr<PPTX::Logic::Shape> oShape = m_elem.smart_dynamic_cast<PPTX::Logic::Shape>();
					if (oShape.IsInit()) oShape->toXmlWriterVML(pWriter, oTheme, oClrMap);
				}break;
				case OOX::et_pic:
				{
					smart_ptr<PPTX::Logic::Pic> oPic = m_elem.smart_dynamic_cast<PPTX::Logic::Pic>();
					if (oPic.IsInit()) oPic->toXmlWriterVML(pWriter, oTheme, oClrMap);
				}break;
				case OOX::et_p_ShapeTree:
				{
					smart_ptr<PPTX::Logic::SpTree> oSpTree = m_elem.smart_dynamic_cast<PPTX::Logic::SpTree>();
					if (oSpTree.IsInit()) oSpTree->toXmlWriterVML(pWriter, oTheme, oClrMap);
				}break;
				default:
					break;
			}
		}

	} // namespace Logic
} // namespace PPTX
