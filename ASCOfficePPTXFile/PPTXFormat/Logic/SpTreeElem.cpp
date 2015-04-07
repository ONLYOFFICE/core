//#include "./stdafx.h"

#include "SpTreeElem.h"
#include "Shape.h"
#include "Pic.h"
#include "CxnSp.h"
#include "SpTree.h"
#include "GraphicFrame.h"
#include "./../SlideMaster.h"

namespace PPTX
{
	namespace Logic
	{
		CString GetHexColor(const DWORD& ARGB)
		{
			BYTE r = (BYTE)((ARGB >> 16) & 0xFF);
			BYTE g = (BYTE)((ARGB >> 8) & 0xFF);
			BYTE b = (BYTE)((ARGB & 0xFF));

			CString sC = _T("");
			sC.Format(_T("#%02X%02X%02X"), r, g, b);
			return sC;
		}

		void CalculateFill(PPTX::Logic::SpPr& oSpPr, nullable<ShapeStyle>& pShapeStyle, NSCommon::smart_ptr<PPTX::WrapperFile>& _oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& _oClrMap, CString& strAttr, CString& strNode)
		{
			smart_ptr<PPTX::Theme> oTheme = _oTheme.smart_dynamic_cast<PPTX::Theme>();
			smart_ptr<PPTX::Logic::ClrMap> oClrMap = _oClrMap.smart_dynamic_cast<PPTX::Logic::ClrMap>();

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
				ARGB = pShapeStyle->fillRef.Color.GetRGBColor(_oTheme, _oClrMap, ARGB);
			}

			if (oSpPr.Fill.is_init())
				oSpPr.Fill.Merge(fill);

			if (fill.is<BlipFill>())
			{
				BlipFill& oBlip = fill.as<BlipFill>();

				if (oBlip.blip.is_init() && oBlip.blip->embed.is_init())
				{
					CString fopacity = _T("");
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

								fopacity = _T(" opacity=\"") + XmlUtils::IntToString(nA) + _T("f\"");
							}
							break;
						}
					}

					CString strId = oBlip.blip->embed->ToString();

					if (!oBlip.srcRect.is_init())
					{
						if (oBlip.tile.is_init())
							strNode = _T("<v:fill r:id=\"") + strId + _T("\" o:title=\"\" type=\"tile\"") + fopacity + _T(" />");
						else
							strNode = _T("<v:fill r:id=\"") + strId + _T("\" o:title=\"\" type=\"frame\"") + fopacity + _T(" />");
					}
					else
					{
						// crop
						if (oBlip.tile.is_init())
							strNode = _T("<v:fill r:id=\"") + strId + _T("\" o:title=\"\" type=\"tile\"") + fopacity + _T(" />");
						else
							strNode = _T("<v:fill r:id=\"") + strId + _T("\" o:title=\"\" type=\"frame\"") + fopacity + _T(" />");
					}
				}				
			}
			else if (fill.is<SolidFill>())
			{
				ARGB = fill.as<SolidFill>().Color.GetRGBColor(_oTheme, _oClrMap, ARGB);
				strAttr = _T(" fillcolor=\"") + GetHexColor(ARGB) + _T("\"");

				BYTE A = (BYTE)((ARGB >> 24) & 0xFF);
				if (A != 255)
				{
					int fopacity = (int)(((double)A / 255.0) * 65536);
					strNode = _T("<v:fill opacity=\"") + XmlUtils::IntToString(fopacity) + _T("f\" />");
				}
			}
			else if (fill.is<GradFill>())
			{
				GradFill& oGrad = fill.as<GradFill>();
				if (oGrad.GsLst.size() > 0)
				{
					ARGB = oGrad.GsLst[0].color.GetRGBColor(_oTheme, _oClrMap, ARGB);
					strAttr = _T(" fillcolor=\"") + GetHexColor(ARGB) + _T("\"");

					BYTE A = (BYTE)((ARGB >> 24) & 0xFF);
					if (A != 255)
					{
						int fopacity = (int)(((double)A / 255.0) * 65536);
						strNode = _T("<v:fill opacity=\"") + XmlUtils::IntToString(fopacity) + _T("f\" />");
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
					strNode = _T("<v:fill opacity=\"") + XmlUtils::IntToString(fopacity) + _T("f\" />");
				}
			}

			/*
			BYTE alpha = (BYTE)((ARGB >> 24) & 0xFF);
			if (alpha < 255)
			{
				CString strA = _T("");
				strA.Format(_T("%d"), alpha);
				strAttr += _T(" opacity=\"") + strA + _T("\"");
			}
			*/
		}

		void CalculateLine(PPTX::Logic::SpPr& oSpPr, nullable<ShapeStyle>& pShapeStyle, NSCommon::smart_ptr<PPTX::WrapperFile>& _oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& _oClrMap, CString& strAttr, CString& strNode)
		{
			smart_ptr<PPTX::Theme> oTheme = _oTheme.smart_dynamic_cast<PPTX::Theme>();
			smart_ptr<PPTX::Logic::ClrMap> oClrMap = _oClrMap.smart_dynamic_cast<PPTX::Logic::ClrMap>();

			PPTX::Logic::Ln line;
			DWORD ARGB = 0;
			if (pShapeStyle.is_init() && oTheme.is_init())
			{
				oTheme->GetLineStyle(pShapeStyle->lnRef.idx.get_value_or(0), line);
				ARGB = pShapeStyle->lnRef.Color.GetRGBColor(_oTheme, _oClrMap, ARGB);
			}

			if (oSpPr.ln.is_init())
				oSpPr.ln->Merge(line);

			if (line.Fill.is<SolidFill>())
			{
				ARGB = line.Fill.as<SolidFill>().Color.GetRGBColor(_oTheme, _oClrMap, ARGB);
				strAttr = _T(" strokecolor=\"") + GetHexColor(ARGB) + _T("\"");
			}

			if (line.w.is_init())
			{
				double dW = 72.0 * (*line.w) / (25.4 * 36000);
				CString s = _T(""); 
				s.Format(_T(" strokeweight=\"%.2lfpt\""), dW);
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


		void SpTreeElem::fromXML(XmlUtils::CXmlNode& node)
		{
			CString name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("sp") || name == _T("wsp"))
				m_elem.reset(new Logic::Shape(node));
			else if (name == _T("pic"))
				m_elem.reset(new Logic::Pic(node));
			else if (name == _T("cxnSp"))
				m_elem.reset(new Logic::CxnSp(node));
			else if (name == _T("grpSp") || name == _T("wgp") || name == _T("spTree") || name == _T("lockedCanvas"))
				m_elem.reset(new Logic::SpTree(node));
			else if (name == _T("graphicFrame"))
				m_elem.reset(new Logic::GraphicFrame(node));
			else if (name == _T("AlternateContent"))
			{
				XmlUtils::CXmlNode oNodeFall;
				if (node.GetNode(_T("mc:Fallback"), oNodeFall))
				{
					XmlUtils::CXmlNodes oNodesC;
					if (oNodeFall.GetNodes(_T("*"), oNodesC))
					{
						if (1 == oNodesC.GetCount())
						{
							XmlUtils::CXmlNode oNodeC;
							oNodesC.GetAt(0, oNodeC);

							fromXML(oNodeC);
						}
						else
						{
							m_elem.reset();	
						}
					}
					else
					{
						m_elem.reset();	
					}
				}
				else 
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
			case SPTREE_TYPE_PIC:
				{
					Logic::Pic* p = new Logic::Pic();
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
					p->m_name = _T("p:grpSp");
					pReader->Seek(pReader->GetPos() - 5); // type back + len
					p->fromPPTY(pReader);
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

		CString SpTreeElem::toXML() const
		{
			if (m_elem.IsInit())
				return m_elem->toXML();
			return _T("");
		}

	} // namespace Logic
} // namespace PPTX