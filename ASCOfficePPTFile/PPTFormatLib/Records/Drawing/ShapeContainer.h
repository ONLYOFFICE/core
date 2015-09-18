#pragma once
#include "../ExObjRefAtom.h"
#include "../TextBytesAtom.h"
#include "../TextCharsAtom.h"
#include "../TextHeaderAtom.h"
#include "../PlaceHolderAtom.h"
#include "../StyleTextPropAtom.h"
#include "../OutlineTextRefAtom.h"
#include "../InteractiveInfoAtom.h"
#include "../TextInteractiveInfoAtom.h"
//#include "../../Reader/Slide.h"
#include "../../../../ASCPresentationEditor/OfficeDrawing/Document.h"
#include "Shape.h"
#include "ShapeProperties.h"
#include "../../Reader/ClassesAtom.h"
#include "../../Reader/SlideInfo.h"
#include "../../../../ASCPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTShape/ElementSettings.h"

const double EMU_MM = 36000;

using namespace NSOfficeDrawing;
using namespace NSPresentationEditor;

class CPPTElement
{
public:
	static inline LONG CorrectPlaceHolderType(const LONG& lType)
	{
		switch (lType)
		{
		case 0x00:		//PT_None
			break;

		case 0x01:		//PT_MasterTitle
		case 0x0D:		//PT_Title
		case 0x11:		//PT_VerticalTitle
			return 15;	// title

		case 0x02:		//PT_MasterBody
		case 0x0E:		//PT_Body
		case 0x06:		//PT_MasterNotesBody
		case 0x12:		//PT_VerticalBody
		case 0x0C:		//PT_NotesBody
			return 0;	// body

		case 0x03:		//PT_MasterCenterTitle
		case 0x0F:		//PT_CenterTitle
			return 3;	// ctrTitle

		case 0x04:		//PT_MasterSubTitle
		case 0x10:		//PT_SubTitle
			return 13;	// subtitle

		case 0x13:		//PT_Object
		case 0x19:		//PT_VerticalObject
			return 9;	// object

		case 0x05:		//PT_MasterNotesSlideImage
		case 0x14:		//PT_Graph //????
		case 0x0B:		//PT_NotesSlideImage
			return 11;	// slideImg

		case 0x15:		//PT_Table
			return 14;	// table

		case 0x16:		//PT_ClipArt
			return 2;	// clipArt

		case 0x17:		//PT_OrgChart
			return 1;	// chart

		case 0x18:		//PT_Media
			return 8;	// media

		case 0x1A:		//PT_Picture
			return 10;	// picture

		case 0x07:		//PT_MasterDate
			return 5;	// date

		case 0x08:		//PT_MasterSlideNumber
			return 12;	// sldNum

		case 0x09:		//PT_MasterFooter
			return 6;	// footer

		case 0x0A:		//PT_MasterHeader
			return 7;	// header
		default:
			break;
		}

		return lType; // undefined
	}

	inline void SetUpProperties(IElement* pElement, CTheme* pTheme, CSlideInfo* pWrapper, CSlide* pSlide, CProperties* pProperties)
	{
		long lCount = pProperties->m_lCount;
		switch (pElement->m_etType)
		{
		case NSPresentationEditor::etVideo:
			{
				for (long i = 0; i < lCount; ++i)
				{
					SetUpPropertyVideo((CVideoElement*)pElement, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
				}
				break;
			}
		case NSPresentationEditor::etPicture:
			{
				for (long i = 0; i < lCount; ++i)
				{
					SetUpPropertyImage((CImageElement*)pElement, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
				}
				break;
			}
		case NSPresentationEditor::etAudio:
			{
				for (long i = 0; i < lCount; ++i)
				{
					SetUpPropertyAudio((CAudioElement*)pElement, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
				}
				break;
			}
		case NSPresentationEditor::etShape:
			{
				CShapeElement* pShapeElem = (CShapeElement*)pElement;
				CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(pShapeElem->m_oShape.m_pShape);

				if (NULL != pPPTShape)
				{
					pPPTShape->m_oCustomVML.SetAdjusts(&pPPTShape->m_arAdjustments);
				}

				for (long i = 0; i < lCount; ++i)
				{
					SetUpPropertyShape(pShapeElem, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
				}

				if (NULL != pPPTShape)
				{
					pPPTShape->m_oCustomVML.ToCustomShape(pPPTShape, pPPTShape->m_oManager);
					pPPTShape->ReCalculate();
				}
				break;
			}
		default:
			break;
		}
	}

	inline bool SetUpProperty(IElement* pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
	{
		switch (pProperty->m_ePID)
		{
		case wzName:
			{
				pElement->m_sName = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue /2 - 1); 
				return true;
			}
		case wzDescription:
			{
				pElement->m_sDescription = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue /2 - 1); 
				return true;
			}
		case hspMaster:
			{
				pElement->m_lLayoutID = (LONG)pProperty->m_lValue; 
				return true;
			}
		case rotation:
			{
				pElement->m_dRotate = (double)((LONG)pProperty->m_lValue) / 0x00010000;
				return true;
			}
		case fFlipH:
			{
				BYTE flag1 = (BYTE)pProperty->m_lValue;
				BYTE flag3 = (BYTE)(pProperty->m_lValue >> 16);

				bool bFlipH = (0x01 == (0x01 & flag1));
				bool bFlipV = (0x02 == (0x02 & flag1));

				bool bUseFlipH = (0x01 == (0x01 & flag3));
				bool bUseFlipV = (0x02 == (0x02 & flag3));

				if (bUseFlipH)
					pElement->m_bFlipH = bFlipH;

				if (bUseFlipV)
					pElement->m_bFlipV = bFlipV;

				return true;
			}
		default:
			break;
		}
		return false;
	}

	inline void SetUpPropertyVideo(CVideoElement* pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
	{
		SetUpProperty((IElement*)pElement, pTheme, pInfo, pSlide, pProperty);
	}
	inline void SetUpPropertyAudio(CAudioElement* pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
	{
		SetUpProperty((IElement*)pElement, pTheme, pInfo, pSlide, pProperty);
	}
	inline void SetUpPropertyImage(CImageElement* pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
	{
		SetUpProperty((IElement*)pElement, pTheme, pInfo, pSlide, pProperty);

		switch(pProperty->m_ePID)
		{
		case Pib:
			{
				DWORD dwIndex = pInfo->GetIndexPicture(pProperty->m_lValue);

				CString strVal = CDirectory::ToString(dwIndex);
				pElement->m_strFileName = pElement->m_strFileName + strVal.GetBuffer() + L".jpg";
			}break;
		case pibName:
			{
				pElement->m_sName = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue /2); 
			}break;
		case cropFromTop:
			{
				pElement->m_lcropFromTop = pProperty->m_lValue; 
				pElement->m_bCropEnabled = true;
			}break;
		case cropFromBottom:
			{
				pElement->m_lcropFromBottom = pProperty->m_lValue; 
				pElement->m_bCropEnabled = true;
			}break;
		case cropFromLeft:
			{
				pElement->m_lcropFromLeft = pProperty->m_lValue; 
				pElement->m_bCropEnabled = true;
			}break;
		case cropFromRight:
			{
				pElement->m_lcropFromRight = pProperty->m_lValue; 
				pElement->m_bCropEnabled = true;
			}break;
		case pibFlags:
			{
			}break;
		}
	}
	inline void SetUpPropertyShape(CShapeElement* pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
	{
		if (SetUpProperty((IElement*)pElement, pTheme, pInfo, pSlide, pProperty))
			return;

		CShape* pParentShape	= &pElement->m_oShape;
		CPPTShape* pShape		= dynamic_cast<CPPTShape*>(pParentShape->m_pShape);

		CElementProperties* pElemProps = &pElement->m_oProperties;

		if (NULL == pShape)
			return;

		bool bIsFilled	= true;
		bool bIsDraw	= true;

		switch (pProperty->m_ePID)
		{
			// здесь просто применяем проперти...
			// geometry ----------------------------------------------------
			// top, left, right, bottom logic
		case NSOfficeDrawing::geoRight:
			{
				if (0 < pProperty->m_lValue)
					pParentShape->m_dWidthLogic = (double)(pProperty->m_lValue);
				break;
			}
		case NSOfficeDrawing::geoBottom:
			{
				if (0 < pProperty->m_lValue)
					pParentShape->m_dHeightLogic = (double)(pProperty->m_lValue);
				break;
			}
			// shapePath
		case NSOfficeDrawing::shapePath:
			{
				pShape->m_oCustomVML.SetPath((RulesType)pProperty->m_lValue);
				break;
			}
			// segmentsInfo
		case NSOfficeDrawing::pSegmentInfo:
			{
				if (pProperty->m_bComplex)
				{
					pShape->m_oCustomVML.LoadSegments(pProperty);
				}
				break;
			}
			// verticesInfo
		case NSOfficeDrawing::pVertices:
			{
				if (pProperty->m_bComplex)
				{
					pShape->m_oCustomVML.LoadVertices(pProperty);
				}
				break;
			}
		case NSOfficeDrawing::pGuides:
			{
				if (pProperty->m_bComplex)
				{
					pShape->m_oCustomVML.LoadGuides(pProperty);
				}
				break;
			}
		case NSOfficeDrawing::pAdjustHandles:
			{
				if (pProperty->m_bComplex)
				{
					pShape->m_oCustomVML.LoadAHs(pProperty);
				}
				break;
			}
		case NSOfficeDrawing::dxTextLeft:
			{
				pParentShape->m_dTextMarginX = (double)pProperty->m_lValue / EMU_MM;
				//pElemProps->SetAt(CElementProperty::epTextMarginLeft, pProperty->m_lValue);
				break;
			}
		case NSOfficeDrawing::dxTextRight:
			{
				pParentShape->m_dTextMarginRight = (double)pProperty->m_lValue / EMU_MM;
				//pElemProps->SetAt(CElementProperty::epTextMarginRight, pProperty->m_lValue);
				break;
			}
		case NSOfficeDrawing::dyTextTop:
			{
				pParentShape->m_dTextMarginY = (double)pProperty->m_lValue / EMU_MM;
				//pElemProps->SetAt(CElementProperty::epTextMarginTop, pProperty->m_lValue);
				break;
			}
		case NSOfficeDrawing::dyTextBottom:
			{
				pParentShape->m_dTextMarginBottom = (double)pProperty->m_lValue / EMU_MM;
				//pElemProps->SetAt(CElementProperty::epTextMarginBottom, pProperty->m_lValue);
				break;
			}
		case NSOfficeDrawing::WrapText:
			{
				pParentShape->m_oText.m_lWrapMode = (LONG)pProperty->m_lValue;
				//pElemProps->SetAt(CElementProperty::epTextWrap, pProperty->m_lValue);
				break;
			}
		case NSOfficeDrawing::adjustValue:
		case NSOfficeDrawing::adjust2Value:
		case NSOfficeDrawing::adjust3Value:
		case NSOfficeDrawing::adjust4Value:
		case NSOfficeDrawing::adjust5Value:
		case NSOfficeDrawing::adjust6Value:
		case NSOfficeDrawing::adjust7Value:
		case NSOfficeDrawing::adjust8Value:
		case NSOfficeDrawing::adjust9Value:
		case NSOfficeDrawing::adjust10Value:
			{
				LONG lIndexAdj = pProperty->m_ePID - NSOfficeDrawing::adjustValue;
				if (lIndexAdj >= 0 && lIndexAdj < pShape->m_arAdjustments.size())
				{
					//SetAdjustment(lIndexAdj, (LONG)pProperty->m_lValue);
					pShape->m_oCustomVML.LoadAdjusts(lIndexAdj, (LONG)pProperty->m_lValue);
				}
				else
				{
					pShape->m_oCustomVML.LoadAdjusts(lIndexAdj, (LONG)pProperty->m_lValue);
				}
				break;
			}

		case NSOfficeDrawing::fillType:
			{
				DWORD dwType = pProperty->m_lValue;
				if (NSOfficeDrawing::fillPattern == dwType ||
					NSOfficeDrawing::fillTexture == dwType ||
					NSOfficeDrawing::fillPicture == dwType)
				{
					//pElemProps->SetAt(CElementProperty::epBrushType, c_BrushTypeTexture);
					//pElemProps->SetAt(CElementProperty::epBrushTxMode, 
					//	(NSOfficeDrawing::fillPicture == dwType) ? c_BrushTextureModeStretch : c_BrushTextureModeTile);

					pParentShape->m_oBrush.Type = c_BrushTypeTexture;
					pParentShape->m_oBrush.TextureMode = (NSOfficeDrawing::fillPicture == dwType) ? c_BrushTextureModeStretch : c_BrushTextureModeTile;
				}
				else if (NSOfficeDrawing::fillShade == dwType			||
					NSOfficeDrawing::fillShadeCenter == dwType	||
					NSOfficeDrawing::fillShadeTitle == dwType)
				{
					pParentShape->m_oBrush.Type = c_BrushTypeVertical;
					//pElemProps->SetAt(CElementProperty::epBrushType, c_BrushTypeVertical);
				}
				else if (NSOfficeDrawing::fillShadeShape == dwType	|| NSOfficeDrawing::fillShadeScale == dwType)
				{
					pParentShape->m_oBrush.Type = c_BrushTypeSolid;
					//pElemProps->SetAt(CElementProperty::epBrushType, c_BrushTypeSolid);
				}
				else
				{
					pParentShape->m_oBrush.Type = c_BrushTypeSolid;
					//pElemProps->SetAt(CElementProperty::epBrushType, c_BrushTypeSolid);
				}
				break;
			}
		case NSOfficeDrawing::fillBlip:
			{
				DWORD dwIndex = pInfo->GetIndexPicture(pProperty->m_lValue);

				CString strVal = CDirectory::ToString(dwIndex);

                int nIndex	= pParentShape->m_oBrush.TexturePath.rfind(FILE_SEPARATOR_CHAR);
				int nLen	= pParentShape->m_oBrush.TexturePath.length() - 1;
				if (nLen != nIndex)
				{
					pParentShape->m_oBrush.TexturePath.erase(nIndex + 1, nLen - nIndex);
				}

				//pElemProps->SetAt(CElementProperty::epBrushTxPath, pParentShape->m_oBrush.TexturePath + strVal + L".jpg");
				pParentShape->m_oBrush.TexturePath = pParentShape->m_oBrush.TexturePath + strVal.GetBuffer() + L".jpg";
				break;
			}
		case NSOfficeDrawing::fillColor:
			{
				SColorAtom oAtom;
				oAtom.FromValue(pProperty->m_lValue);
				//pElemProps->SetAt(CElementProperty::epBrushColor1, oAtom.ToValueProperty());
				oAtom.ToColor(&pParentShape->m_oBrush.Color1);
				break;
			}
		case NSOfficeDrawing::fillBackColor:
			{
				SColorAtom oAtom;
				oAtom.FromValue(pProperty->m_lValue);
				//pElemProps->SetAt(CElementProperty::epBrushColor2, oAtom.ToValueProperty());
				oAtom.ToColor(&pParentShape->m_oBrush.Color2);
				break;
			}
		case NSOfficeDrawing::fillOpacity:
			{
                //pElemProps->SetAt(CElementProperty::epBrushAlpha1, (std::min)(255, CDirectory::NormFixedPoint(pProperty->m_lValue, 255)));
                pParentShape->m_oBrush.Alpha1 = (BYTE)(std::min)(255, (int)CDirectory::NormFixedPoint(pProperty->m_lValue, 255));
				break;
			}
		case NSOfficeDrawing::fillBackOpacity:
			{
                pParentShape->m_oBrush.Alpha2 = (BYTE)(std::min)(255, (int)CDirectory::NormFixedPoint(pProperty->m_lValue, 255));
                //pElemProps->SetAt(CElementProperty::epBrushAlpha2,(std::min)(255, CDirectory::NormFixedPoint(pProperty->m_lValue, 255)));
				break;
			}
		case NSOfficeDrawing::fillBackground:
			{
				//bIsFilled = false;
				break;
			}
		case NSOfficeDrawing::fNoFillHitTest:
			{
				BYTE flag1 = (BYTE)(pProperty->m_lValue);
				BYTE flag2 = (BYTE)(pProperty->m_lValue >> 16);

				bool bNoFillHitTest			= (0x01 == (0x01 & flag1));
				bool bFillUseRect			= (0x02 == (0x02 & flag1));
				bool bFillShape				= (0x04 == (0x04 & flag1));
				bool bHitTestFill			= (0x08 == (0x08 & flag1));
				bool bFilled				= (0x10 == (0x10 & flag1));
				bool bUseShapeAnchor		= (0x20 == (0x20 & flag1));
				bool bRecolorFillAsPictures = (0x40 == (0x40 & flag1));

				bool bUsebNoFillHitTest			= (0x01 == (0x01 & flag2));
				bool bUsebFillUseRect			= (0x02 == (0x02 & flag2));
				bool bUsebFillShape				= (0x04 == (0x04 & flag2));
				bool bUsebHitTestFill			= (0x08 == (0x08 & flag2));
				bool bUsebFilled				= (0x10 == (0x10 & flag2));
				bool bUsebUseShapeAnchor		= (0x20 == (0x20 & flag2));
				bool bUsebRecolorFillAsPictures = (0x40 == (0x40 & flag2));

				if (bUsebFilled)
					bIsFilled = bFilled;

				break;
			}
			// line --------------------------------------------------------
		case NSOfficeDrawing::lineColor:
			{
				SColorAtom oAtom;
				oAtom.FromValue(pProperty->m_lValue);
				//pElemProps->SetAt(CElementProperty::epPenColor, oAtom.ToValueProperty());
				oAtom.ToColor(&pParentShape->m_oPen.Color);
				break;
			}
		case NSOfficeDrawing::lineOpacity:
			{
				//pElemProps->SetAt(CElementProperty::epPenAlpha, min(255, CDirectory::NormFixedPoint(pProperty->m_lValue, 255)));
                pParentShape->m_oPen.Alpha = (BYTE)(std::min)(255, (int)CDirectory::NormFixedPoint(pProperty->m_lValue, 255));
				break;
			}
		case NSOfficeDrawing::lineWidth:
			{
				pParentShape->m_oPen.Size = (double)pProperty->m_lValue / EMU_MM;
				//pElemProps->SetAt(CElementProperty::epPenWidth, pProperty->m_lValue);
				break;
			}
		case NSOfficeDrawing::lineDashing:
			{
				BYTE nDashStyle = 0;
				switch (pProperty->m_lValue)
				{
				case 0: 
					{ 
						nDashStyle = 0; 
						break; 
					}	// solid 
				case 1: 
				case 6:
				case 7:
					{ 
						nDashStyle = 1; 
						break; 
					}	// dash 
				case 2:
				case 5:
					{ 
						nDashStyle = 2; 
						break; 
					}	// dot 
				case 3:
				case 8:
				case 9:
					{ 
						nDashStyle = 3; 
						break; 
					}	// dashdot 
				case 4:
				case 10:
					{ 
						nDashStyle = 4;
						break; 
					}	// dashdotdot 
				default:
					{
						break; 
					}
				};
				//pElemProps->SetAt(CElementProperty::epLineDash, nDashStyle);
				pParentShape->m_oPen.DashStyle = nDashStyle;
				break;
			}
		case NSOfficeDrawing::lineJoinStyle:
			{
				BYTE nLineJoin = 2;
				switch (pProperty->m_lValue)
				{
				case 0: 
					{ 
						nLineJoin = 1; 
						break; 
					}	// bevel 
				case 1: 

					{ 
						nLineJoin = 1; 
						break; 
					}	// Miter 
				case 2:
					{ 
						nLineJoin = 2; 
						break; 
					}	// round 
				default:
					{
						break; 
					}
				};

				pParentShape->m_oPen.LineJoin = nLineJoin;
				//pElemProps->SetAt(CElementProperty::epPenJoin, nLineJoin);
				break;
			}
		case NSOfficeDrawing::lineStartArrowhead:
			{
				BYTE nStartCap = 0;
				switch (pProperty->m_lValue)
				{
				case 1: 
				case 2:
				case 5:
					{ 
						nStartCap = 0x14; 
						break; 
					}
				case 3:
				case 4:

					{ 
						nStartCap = 2; 
						break; 
					}
				default:
					{
						break; 
					}
				};

				pParentShape->m_oPen.LineStartCap = nStartCap;
				//pElemProps->SetAt(CElementProperty::epLineStartCap, nStartCap);

				break;
			}
		case NSOfficeDrawing::lineEndArrowhead:
			{
				BYTE nEndCap = 0;
				switch (pProperty->m_lValue)
				{
				case 1: 
				case 2:
				case 5:
					{ 
						nEndCap = 0x14; 
						break; 
					}
				case 3:
				case 4:

					{ 
						nEndCap = 2; 
						break; 
					}
				default:
					{
						break; 
					}
				};

				pParentShape->m_oPen.LineEndCap = nEndCap;
				//pElemProps->SetAt(CElementProperty::epLineEndCap, nEndCap);
				break;
			}
		case NSOfficeDrawing::fNoLineDrawDash:
			{
				BYTE flag1 = (BYTE)(pProperty->m_lValue);
				BYTE flag2 = (BYTE)(pProperty->m_lValue >> 8);
				BYTE flag3 = (BYTE)(pProperty->m_lValue >> 16);
				BYTE flag4 = (BYTE)(pProperty->m_lValue >> 24);

				bool bNoLineDrawDash			= (0x01 == (0x01 & flag1));
				bool bLineFillShape				= (0x02 == (0x02 & flag1));
				bool bHitTestLine				= (0x04 == (0x04 & flag1));
				bool bLine						= (0x08 == (0x08 & flag1));
				bool bArrowheadsOK				= (0x10 == (0x10 & flag1));
				bool bInsertPenOK				= (0x20 == (0x20 & flag1));
				bool bInsertPen					= (0x40 == (0x40 & flag1));

				bool bLineOpaqueBackColor		= (0x02 == (0x02 & flag2));

				bool bUsebNoLineDrawDash		= (0x01 == (0x01 & flag3));
				bool bUsebLineFillShape			= (0x02 == (0x02 & flag3));
				bool bUsebHitTestLine			= (0x04 == (0x04 & flag3));
				bool bUsebLine					= (0x08 == (0x08 & flag3));
				bool bUsebArrowheadsOK			= (0x10 == (0x10 & flag3));
				bool bUsebInsertPenOK			= (0x20 == (0x20 & flag3));
				bool bUsebInsertPen				= (0x40 == (0x40 & flag3));

				bool bUsebLineOpaqueBackColor	= (0x02 == (0x02 & flag4));

				if (bUsebLine)
					bIsDraw = bLine;

				break;
			}
			// text --------------------------------------------------------
		case NSOfficeDrawing::gtextUNICODE:
			{
				if (pProperty->m_bComplex && 0 < pProperty->m_lValue)
				{
					std::wstring str = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue/2);
					//pParentShape->m_oText.m_sText = str;
				}
				break;
			}
			// font --------------------------------------------------------
		case NSOfficeDrawing::gtextFont:
			{
				if (pProperty->m_bComplex && 0 < pProperty->m_lValue)
				{
					std::wstring str = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue/2);
					pParentShape->m_oText.m_oAttributes.m_oFont.Name = std_string2string(str);
					//pElemProps->SetAt(CElementProperty::epFontName, (CString)str);
				}
				break;
			}
		case NSOfficeDrawing::gtextSize:
			{
				pParentShape->m_oText.m_oAttributes.m_oFont.Size = (INT)((pProperty->m_lValue >> 16) & 0x0000FFFF);
				//pElemProps->SetAt(CElementProperty::epFontSize, (DWORD)((pProperty->m_lValue >> 16) & 0x0000FFFF));
				break;
			}
		case NSOfficeDrawing::anchorText:
			{
				switch (pProperty->m_lValue)
				{
				case NSOfficeDrawing::anchorTop:
				case NSOfficeDrawing::anchorTopBaseline:
					{
						//m_oText.m_nTextAlignHorizontal = 0;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 0;
						//pElemProps->SetAt(CElementProperty::epFontVertAlign, (DWORD)0);
						break;
					}
				case NSOfficeDrawing::anchorMiddle:
					{
						//m_oText.m_nTextAlignHorizontal = 0;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 1;
						//pElemProps->SetAt(CElementProperty::epFontVertAlign, (DWORD)1);
						break;
					}
				case NSOfficeDrawing::anchorBottom:
				case NSOfficeDrawing::anchorBottomBaseline:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 0;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 2;
						//pElemProps->SetAt(CElementProperty::epFontHorAlign, (DWORD)0);
						//pElemProps->SetAt(CElementProperty::epFontVertAlign, (DWORD)2);
						break;
					}
				case NSOfficeDrawing::anchorTopCentered:
				case NSOfficeDrawing::anchorTopCenteredBaseline:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 0;
						//pElemProps->SetAt(CElementProperty::epFontHorAlign, (DWORD)1);
						//pElemProps->SetAt(CElementProperty::epFontVertAlign, (DWORD)0);
						break;
					}
				case NSOfficeDrawing::anchorMiddleCentered:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 1;
						//pElemProps->SetAt(CElementProperty::epFontHorAlign, (DWORD)1);
						//pElemProps->SetAt(CElementProperty::epFontVertAlign, (DWORD)1);
						break;
					}
				case NSOfficeDrawing::anchorBottomCentered:
				case NSOfficeDrawing::anchorBottomCenteredBaseline:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 2;
						//pElemProps->SetAt(CElementProperty::epFontHorAlign, (DWORD)1);
						//pElemProps->SetAt(CElementProperty::epFontVertAlign, (DWORD)2);
						break;
					}
				default:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 0;
						//pElemProps->SetAt(CElementProperty::epFontHorAlign, (DWORD)1);
						//pElemProps->SetAt(CElementProperty::epFontVertAlign, (DWORD)0);
						break;
					}
				};
				break;
			}
		case NSOfficeDrawing::gtextAlign:
			{
				switch (pProperty->m_lValue)
				{
				case NSOfficeDrawing::alignTextLeft:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 0;
						//pElemProps->SetAt(CElementProperty::epFontHorAlign, (DWORD)0);
						break;
					}
				case NSOfficeDrawing::alignTextCenter:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						//pElemProps->SetAt(CElementProperty::epFontHorAlign, (DWORD)1);
						break;
					}
				case NSOfficeDrawing::alignTextRight:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 2;
						//pElemProps->SetAt(CElementProperty::epFontHorAlign, (DWORD)2);
						break;
					}
				default:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						//pElemProps->SetAt(CElementProperty::epFontHorAlign, (DWORD)1);
					}
				};
				break;
			}
		case NSOfficeDrawing::gtextFStrikethrough:
			{
				// вот здесь - нужно единицы перевести в пикселы
				BYTE flag1 = (BYTE)(pProperty->m_lValue);
				BYTE flag2 = (BYTE)(pProperty->m_lValue >> 8);
				BYTE flag3 = (BYTE)(pProperty->m_lValue >> 16);
				BYTE flag4 = (BYTE)(pProperty->m_lValue >> 24);

				bool bStrikethrought			= (0x01 == (0x01 & flag1));
				bool bSmallCaps					= (0x02 == (0x02 & flag1));
				bool bShadow					= (0x04 == (0x04 & flag1));
				bool bUnderline					= (0x08 == (0x08 & flag1));
				bool bItalic					= (0x10 == (0x10 & flag1));
				bool bBold						= (0x20 == (0x20 & flag1));

				bool bUseStrikethrought			= (0x01 == (0x01 & flag3));
				bool bUseSmallCaps				= (0x02 == (0x02 & flag3));
				bool bUseShadow					= (0x04 == (0x04 & flag3));
				bool bUseUnderline				= (0x08 == (0x08 & flag3));
				bool bUseItalic					= (0x10 == (0x10 & flag3));
				bool bUseBold					= (0x20 == (0x20 & flag3));

				bool bVertical					= (0x20 == (0x20 & flag2));
				bool bUseVertical				= (0x20 == (0x20 & flag4));

				if (bUseStrikethrought)
				{
                    pParentShape->m_oText.m_oAttributes.m_oFont.Strikeout = (BYTE)bStrikethrought;
					//pElemProps->SetAt(CElementProperty::epFontStrikeout, bStrikethrought);
				}
				if (bUseShadow)
				{
					pParentShape->m_oText.m_oAttributes.m_oTextShadow.Visible = true;
				}
				if (bUseUnderline)
				{
                    pParentShape->m_oText.m_oAttributes.m_oFont.Underline = (BYTE)bUnderline;
					//pElemProps->SetAt(CElementProperty::epFontUnderline, bUnderline);
				}
				if (bUseItalic)
				{
                    pParentShape->m_oText.m_oAttributes.m_oFont.Italic = bItalic;
					//pElemProps->SetAt(CElementProperty::epFontItalic, bItalic);
				}
				if (bUseBold)
				{
                    pParentShape->m_oText.m_oAttributes.m_oFont.Bold = bBold;
					//pElemProps->SetAt(CElementProperty::epFontBold, bBold);
				}

				if (bUseVertical)
				{
                    pParentShape->m_oText.m_bVertical = (true == bVertical) ? true : false;
				}
				break;
			}
		case NSOfficeDrawing::fFitTextToShape:
			{
				BYTE flag1 = (BYTE)(pProperty->m_lValue);
				BYTE flag2 = (BYTE)(pProperty->m_lValue >> 8);
				BYTE flag3 = (BYTE)(pProperty->m_lValue >> 16);
				BYTE flag4 = (BYTE)(pProperty->m_lValue >> 24);

				bool bFitShapeToText		= (0x02 == (0x02 & flag1));
				bool bAutoTextMargin		= (0x08 == (0x08 & flag1));
				bool bSelectText			= (0x10 == (0x10 & flag1));

				bool bUseFitShapeToText		= (0x02 == (0x02 & flag3));
				bool bUseAutoTextMargin		= (0x08 == (0x08 & flag3));
				bool bUseSelectText			= (0x10 == (0x10 & flag3));

				if (bUseAutoTextMargin)
				{
					if (bAutoTextMargin)
					{
						pParentShape->m_dTextMarginX		= 2.54;
						pParentShape->m_dTextMarginRight	= 1.27;
						pParentShape->m_dTextMarginY		= 2.54;
						pParentShape->m_dTextMarginBottom	= 1.27;
						//pElemProps->SetAt(CElementProperty::epTextMarginLeft,	(DWORD)(2.54 * EMU_MM));
						//pElemProps->SetAt(CElementProperty::epTextMarginTop,	(DWORD)(2.54 * EMU_MM));
						//pElemProps->SetAt(CElementProperty::epTextMarginRight,	(DWORD)(1.27 * EMU_MM));
						//pElemProps->SetAt(CElementProperty::epTextMarginBottom, (DWORD)(1.27 * EMU_MM));
					}
				}
				break;
			}
			// geometry shape
		case NSOfficeDrawing::fFillOK:
			{
				BYTE flag1 = (BYTE)(pProperty->m_lValue);
				BYTE flag2 = (BYTE)(pProperty->m_lValue >> 8);
				BYTE flag3 = (BYTE)(pProperty->m_lValue >> 16);
				BYTE flag4 = (BYTE)(pProperty->m_lValue >> 24);

				bool bFillOk					= (0x01 == (0x01 & flag1));
				bool bFillShadeShapeOk			= (0x02 == (0x02 & flag1));
				bool bGTextOk					= (0x04 == (0x04 & flag1));
				bool bLineOk					= (0x08 == (0x08 & flag1));
				bool b3DOk						= (0x10 == (0x10 & flag1));
				bool bShadowOk					= (0x20 == (0x20 & flag1));

				bool bUseFillOk					= (0x01 == (0x01 & flag3));
				bool bUseFillShadeShapeOk		= (0x02 == (0x02 & flag3));
				bool bUseGTextOk				= (0x04 == (0x04 & flag3));
				bool bUseLineOk					= (0x08 == (0x08 & flag3));
				bool bUse3DOk					= (0x10 == (0x10 & flag3));
				bool bUseShadowOk				= (0x20 == (0x20 & flag3));

				if (bUseLineOk)
					bIsDraw = bLineOk;

				if (bUseFillOk)
					bIsFilled = bFillOk;

				break;
			}
		default:
			break;
		}

		if (!bIsDraw)
		{
			pParentShape->m_oPen.Alpha = 0;
			//pElemProps->SetAt(CElementProperty::epStroked, (DWORD)0);
		}

		if (!bIsFilled)
		{
			pParentShape->m_oBrush.Type = (int)c_BrushTypeSolid;
			pParentShape->m_oBrush.Alpha1 = 0;
			pParentShape->m_oBrush.Alpha2 = 0;
			//pElemProps->SetAt(CElementProperty::epFilled, (DWORD)0);
		}
	}
};

// один из самых главных классов
// он умеет отдавать интерфейс IElement...
// пока - это video, image, shape, text, color - наверное не надо
class CRecordShapeContainer : public CRecordsContainer
{
private:
	POLE::Stream* m_pStream;

public:

	RECT* m_pGroupBounds;
	RECT* m_pGroupClientAnchor;

public:

	CRecordShapeContainer()
	{
		m_pStream = NULL;

		m_pGroupBounds = NULL;
		m_pGroupClientAnchor = NULL;
	}

	~CRecordShapeContainer()
	{
		m_pStream = NULL;

		m_pGroupBounds = NULL;
		m_pGroupClientAnchor = NULL;
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_pStream = pStream;
		CRecordsContainer::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CRecordsContainer::ToString();
	}

	virtual void GetElement(IElement** ppElement, CExMedia* pMapIDs,
		long lSlideWidth, long lSlideHeight, CTheme* pTheme, CLayout* pLayout, 
		CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide = NULL)
	{
		if (NULL == ppElement)
			return;

		*ppElement = NULL;

		std::vector<CRecordShape*> oArrayShape;
		this->GetRecordsByType(&oArrayShape, true, true);

		if (0 == oArrayShape.size())
			return;

		std::vector<CRecordShapeProperties*> oArrayOptions;
		this->GetRecordsByType(&oArrayOptions, true, /*true*/false/*secondary & tetriary*/);

		PPTShapes::ShapeType eType = (PPTShapes::ShapeType)oArrayShape[0]->m_oHeader.RecInstance;
		ElementType elType = GetTypeElem((NSOfficeDrawing::SPT)oArrayShape[0]->m_oHeader.RecInstance); 

		LONG lMasterID = -1;
		if (NULL != pSlide)
		{
			bool bIsMaster = oArrayShape[0]->m_bHaveMaster;
			if (bIsMaster)
			{
				for (int i = 0; i < oArrayOptions[0]->m_oProperties.m_lCount; ++i)
				{
					if (hspMaster == oArrayOptions[0]->m_oProperties.m_arProperties[i].m_ePID)
					{
						lMasterID = oArrayOptions[0]->m_oProperties.m_arProperties[i].m_lValue;

                        if (pLayout)
                        {
                            size_t nIndexMem = pLayout->m_arElements.size();
                            for (size_t nIndex = 0; nIndex < nIndexMem; ++nIndex)
                            {
                                if (lMasterID == pLayout->m_arElements[nIndex]->m_lID && (elType == pLayout->m_arElements[nIndex]->m_etType))
                                {
                                    *ppElement = pLayout->m_arElements[nIndex]->CreateDublicate();

                                    if (elType == etShape)
                                    {
                                        CShapeElement* pShape = dynamic_cast<CShapeElement*>(*ppElement);
                                        if (NULL != pShape)
                                            pShape->m_oShape.m_oText.m_arParagraphs.clear();
                                    }
                                    break;
                                }
                            }
                        }
						break;
					}
				}
			}
		}
		// раньше искался шейп - и делался дубликат. Теперь думаю это не нужно
		// нужно ориентироваться на placeholder (type & id)						
		IElement* pElem = *ppElement;

		if (NULL == pElem)
		{
			switch (eType)
			{
				//case sptMin:
			case sptMax:
			case sptNil:
				{
					break;
				}
			case sptPictureFrame:
				{
					std::vector<CRecordExObjRefAtom*> oArrayEx;
					this->GetRecordsByType(&oArrayEx, true, true);

					CExFilesInfo oInfo;

					std::wstring strPathPicture = _T("");

					// по умолчанию картинка (или оле объект)
					CExFilesInfo::ExFilesType exType = CExFilesInfo::eftNone;
					CExFilesInfo* pInfo = pMapIDs->Lock(0xFFFFFFFF, exType);
					if (NULL != pInfo)
					{
						oInfo = *pInfo;
						strPathPicture = oInfo.m_strFilePath + FILE_SEPARATOR_STR;
					}

					if (0 != oArrayEx.size())
					{
						CExFilesInfo* pInfo = pMapIDs->Lock(oArrayEx[0]->m_nExObjID, exType);
						if (NULL != pInfo)
						{
							oInfo = *pInfo;
						}
					}

					if (CExFilesInfo::eftVideo == exType)
					{
						CVideoElement* pVideoElem	= new CVideoElement();
						pVideoElem->m_strFileName	= oInfo.m_strFilePath + FILE_SEPARATOR_STR;

						pElem = (IElement*)pVideoElem;
					}
					else if (CExFilesInfo::eftAudio == exType)
					{
						CAudioElement* pAudioElem		= new CAudioElement();
						pAudioElem->m_strFileName		= oInfo.m_strFilePath + FILE_SEPARATOR_STR;

						pAudioElem->m_dClipStartTime	= oInfo.m_dStartTime;
						pAudioElem->m_dClipEndTime		= oInfo.m_dEndTime;

						pAudioElem->m_bLoop				= oInfo.m_bLoop;						

						if (NULL != pSlide)
						{
							pAudioElem->m_dStartTime	= pSlide->m_dStartTime;
							pAudioElem->m_dEndTime		= pSlide->m_dEndTime;

							pSlide->m_arElements.push_back(pAudioElem);
						}
						else
                        {
                            if (pLayout)
                                pLayout->m_arElements.push_back(pAudioElem);
                        }

						CImageElement* pImageElem	= new CImageElement();
						pImageElem->m_strFileName	= strPathPicture;

						pElem = (IElement*)pImageElem;

						//
						//pElem = (IElement*)pAudioElem;
					}
					else
					{
						CImageElement* pImageElem	= new CImageElement();
						pImageElem->m_strFileName	= oInfo.m_strFilePath + FILE_SEPARATOR_STR;

						pElem = (IElement*)pImageElem;
					}
					break;
				}
			default:
				{
					// shape
					CShapeElement* pShape = new CShapeElement(NSBaseShape::ppt, eType);
                    if (true)//if (/*отключим пока кастом*/OOXMLShapes::sptCustom != pShape->m_oShape.m_eType)
					{
						CExFilesInfo::ExFilesType exType = CExFilesInfo::eftNone;
						CExFilesInfo* pTextureInfo = pMapIDs->Lock(0xFFFFFFFF, exType);

						if (NULL != pTextureInfo)
						{
							pShape->m_oShape.m_oBrush.TexturePath = pTextureInfo->m_strFilePath + FILE_SEPARATOR_STR;
						}

						pElem = (IElement*)pShape;
					}
					else
					{
						delete pShape;
						pShape = NULL;
					}
					break;
				}
			};
		}

		if (NULL == pElem)
			return;

		pElem->m_lID = oArrayShape[0]->m_nID;

		std::vector<CRecordClientAnchor*> oArrayAnchor;
		this->GetRecordsByType(&oArrayAnchor, true, true);

		if (0 != oArrayAnchor.size())
		{
			pElem->m_rcBoundsOriginal.left		= (LONG)oArrayAnchor[0]->m_oBounds.Left;
			pElem->m_rcBoundsOriginal.top		= (LONG)oArrayAnchor[0]->m_oBounds.Top;
			pElem->m_rcBoundsOriginal.right		= (LONG)oArrayAnchor[0]->m_oBounds.Right;
			pElem->m_rcBoundsOriginal.bottom	= (LONG)oArrayAnchor[0]->m_oBounds.Bottom;
		}
		else
		{
			std::vector<CRecordChildAnchor*> oArrayChildAnchor;
			this->GetRecordsByType(&oArrayChildAnchor, true, true);

			if (0 != oArrayChildAnchor.size())
			{
				pElem->m_rcBoundsOriginal.left		= oArrayChildAnchor[0]->m_oBounds.left;
				pElem->m_rcBoundsOriginal.top		= oArrayChildAnchor[0]->m_oBounds.top;
				pElem->m_rcBoundsOriginal.right		= oArrayChildAnchor[0]->m_oBounds.right;
				pElem->m_rcBoundsOriginal.bottom	= oArrayChildAnchor[0]->m_oBounds.bottom;

				RecalcGroupShapeAnchor(pElem->m_rcBoundsOriginal);
			}
			else
			{			
				if (oArrayShape[0]->m_bBackground)
				{
					// здесь background
					pElem->m_rcBoundsOriginal.left		= 0;
					pElem->m_rcBoundsOriginal.top		= 0;
					pElem->m_rcBoundsOriginal.right		= lSlideWidth;
					pElem->m_rcBoundsOriginal.bottom	= lSlideHeight;
				}
				else
				{
					// не понятно...					
					pElem->m_rcBoundsOriginal.left		= 0;
					pElem->m_rcBoundsOriginal.top		= 0;
					pElem->m_rcBoundsOriginal.right		= lSlideWidth;
					pElem->m_rcBoundsOriginal.bottom	= lSlideHeight;

					//delete pElem;
					//return;
				}
			}
		}

		double dScaleX = c_dMasterUnitsToMillimetreKoef;
		double dScaleY = c_dMasterUnitsToMillimetreKoef;

		pElem->NormalizeCoords(dScaleX, dScaleY);

		pElem->m_bFlipH = oArrayShape[0]->m_bFlipH;
		pElem->m_bFlipV = oArrayShape[0]->m_bFlipV;

		// проверка на наличие текста
		CShapeElement* pShapeElem = dynamic_cast<CShapeElement*>(pElem);
		if (NULL != pShapeElem)
		{
			CElementInfo oElementInfo;

			pShapeElem->m_oShape.m_dWidthLogic  = ShapeSizeVML;
			pShapeElem->m_oShape.m_dHeightLogic = ShapeSizeVML;

			// проверка на textheader present
			std::vector<CRecordTextHeaderAtom*> oArrayTextHeader;
			this->GetRecordsByType(&oArrayTextHeader, true, true);
			if (0 < oArrayTextHeader.size())
			{
				pShapeElem->m_oShape.m_oText.m_lTextType		= oArrayTextHeader[0]->m_nTextType;
				pShapeElem->m_oShape.m_oText.m_lTextMasterType	= oArrayTextHeader[0]->m_nTextType;
				oElementInfo.m_lMasterTextType							= oArrayTextHeader[0]->m_nTextType;
			}
			else
			{
				pShapeElem->m_oShape.m_oText.m_lTextType		= NSOfficePPT::NoPresent;
				pShapeElem->m_oShape.m_oText.m_lTextMasterType	= NSOfficePPT::NoPresent;
				oElementInfo.m_lMasterTextType							= NSOfficePPT::NoPresent;
			}

			// проверка на ссылку в персист
			std::vector<CRecordOutlineTextRefAtom*> oArrayTextRefs;
			this->GetRecordsByType(&oArrayTextRefs, true, true);
			if (0 < oArrayTextRefs.size())
			{
				oElementInfo.m_lPersistIndex = oArrayTextRefs[0]->m_nIndex;
			}

			// проверка на placeholder
			std::vector<CRecordPlaceHolderAtom*> oArrayPlace;
			this->GetRecordsByType(&oArrayPlace, true, true);

			if (0 < oArrayPlace.size())
			{
				pElem->m_lPlaceholderID		= (LONG)(oArrayPlace[0]->m_nPosition);
				pElem->m_lPlaceholderType	= (LONG)(oArrayPlace[0]->m_nPlacementID);

				oElementInfo.m_lMasterPlaceholderType = pElem->m_lPlaceholderType;

				if (0 == pElem->m_lPlaceholderType)
					pElem->m_lPlaceholderID = 1;
				else if (15 == pElem->m_lPlaceholderType)
					pElem->m_lPlaceholderID = -1;

				pElem->m_lPlaceholderType	= CPPTElement::CorrectPlaceHolderType(pElem->m_lPlaceholderType);
			}

			CString strText = _T("");
			// теперь сам текст...
			std::vector<CRecordTextBytesAtom*> oArrayTextBytes;
			this->GetRecordsByType(&oArrayTextBytes, true, true);
			if (0 < oArrayTextBytes.size())
			{
				strText = (CStringW)(oArrayTextBytes[0]->m_strText);
			}
			std::vector<CRecordTextCharsAtom*> oArrayTextChars;
			this->GetRecordsByType(&oArrayTextChars, true, true);

			if (0 < oArrayTextChars.size())
			{
				strText = oArrayTextChars[0]->m_strText;
			}

			// теперь настройки этого текста...

			// сначала - проперти
			for (int nIndexProp = 0; nIndexProp < oArrayOptions.size(); ++nIndexProp)
			{
				CPPTElement oElement;
				oElement.SetUpProperties(pElem, pTheme, pSlideWrapper, pSlide, &oArrayOptions[nIndexProp]->m_oProperties);
			}

			pElem->m_lLayoutID = lMasterID;

			std::vector<CRecordStyleTextPropAtom*> oArrayTextStyle;
			this->GetRecordsByType(&oArrayTextStyle, true, true);
			if (0 != oArrayTextStyle.size())
			{
				oElementInfo.m_pStream				= m_pStream;
				oElementInfo.m_lOffsetTextStyle	= oArrayTextStyle[0]->m_lOffsetInStream;
			}

			std::vector<CRecordTextSpecInfoAtom*> oArrayTextProp;
			this->GetRecordsByType(&oArrayTextProp, true, true);
			if (0 != oArrayTextProp.size())
			{
				oElementInfo.m_pStream				= m_pStream;
				oElementInfo.m_lOffsetTextProp		= oArrayTextProp[0]->m_lOffsetInStream;
			}

			std::vector<CRecordTextRulerAtom*> oArrayTextRuler;
			this->GetRecordsByType(&oArrayTextRuler, true, true);
			if (0 != oArrayTextRuler.size())
			{
				pShapeElem->m_oShape.m_oText.m_oRuler = oArrayTextRuler[0]->m_oTextRuler;
			}

			std::vector<CRecordTextInteractiveInfoAtom*> oArrayTextInteractive;
			this->GetRecordsByType(&oArrayTextInteractive, true);
			if (0 != oArrayTextInteractive.size())
			{
				pShapeElem->m_oActions.m_bPresent = true;

				int nSize = oArrayTextInteractive.size();
				for (int i = 0; i < nSize; ++i)
				{
					CTextRange oRange;

					oRange.m_lStart = oArrayTextInteractive[i]->m_lStart;
					oRange.m_lEnd	= oArrayTextInteractive[i]->m_lEnd;

					pShapeElem->m_oActions.m_arRanges.push_back(oRange);
				}
			}

			// теперь смотрим какой угол поворота. если он ближе к 90 и 270 чем 0 и 180 - то 
			// его надо повернуть обратно на 90 градусов.
			double dAngle = pShapeElem->m_dRotate;
			if (0 <= dAngle)
			{
				LONG lCount = (LONG)dAngle / 360;
				dAngle -= (lCount * 360.0);
			}
			else
			{
				LONG lCount = (LONG)dAngle / 360;
				dAngle += ((-lCount + 1) * 360.0);
			}
			if (((dAngle > 45) && (dAngle < 135)) || ((dAngle > 225) && (dAngle < 315)))
			{
				double dW = pShapeElem->m_rcBounds.GetWidth();
				double dH = pShapeElem->m_rcBounds.GetHeight();

				double dCx = (pShapeElem->m_rcBounds.left + pShapeElem->m_rcBounds.right) / 2.0;
				double dCy = (pShapeElem->m_rcBounds.top + pShapeElem->m_rcBounds.bottom) / 2.0;

				pShapeElem->m_rcBounds.left		= dCx - dH / 2.0;
				pShapeElem->m_rcBounds.right	= dCx + dH / 2.0;

				pShapeElem->m_rcBounds.top		= dCy - dW / 2.0;
				pShapeElem->m_rcBounds.bottom	= dCy + dW / 2.0;
			}

			pSlideWrapper->m_mapElements.insert(std::pair<LONG, CElementInfo>(pShapeElem->m_lID, oElementInfo));
			SetUpTextStyle(strText, pTheme, pLayout, pElem, pThemeWrapper, pSlideWrapper, pSlide);
		}
		else
		{
			for (int nIndexProp = 0; nIndexProp < oArrayOptions.size(); ++nIndexProp)
			{
				CPPTElement oElement;
				oElement.SetUpProperties(pElem, pTheme, pSlideWrapper, pSlide, &oArrayOptions[nIndexProp]->m_oProperties);
			}

			pElem->m_lLayoutID = lMasterID;
		}

		if (NULL != pSlide)
		{
			pElem->m_dStartTime		= pSlide->m_dStartTime;
			pElem->m_dEndTime		= pSlide->m_dEndTime;

			pElem->m_oMetric.SetUnitsContainerSize(pSlide->m_lOriginalWidth, pSlide->m_lOriginalHeight);
		}
		else
		{
			pElem->m_dStartTime		= 0;
			pElem->m_dEndTime		= 0;

			pElem->m_oMetric.SetUnitsContainerSize(lSlideWidth, lSlideHeight);
		}

		pElem->m_bIsBackground = (true == oArrayShape[0]->m_bBackground);

		*ppElement = pElem;
	}

	void RecalcGroupShapeAnchor(CDoubleRect& rcChildAnchor)
	{
		if ((NULL == m_pGroupBounds) || (NULL == m_pGroupClientAnchor))
			return;

		// здесь переводим координаты, чтобы они не зависили от группы
		LONG lWidthClient = m_pGroupClientAnchor->right - m_pGroupClientAnchor->left;
		LONG lHeightClient = m_pGroupClientAnchor->bottom - m_pGroupClientAnchor->top;
		LONG lWidthGroup = m_pGroupBounds->right - m_pGroupBounds->left;
		LONG lHeightGroup = m_pGroupBounds->bottom - m_pGroupBounds->top;

		double dScaleX = (double)(lWidthClient) / (lWidthGroup);
		double dScaleY = (double)(lHeightClient) / (lHeightGroup);

		rcChildAnchor.left = m_pGroupClientAnchor->left + (LONG)(dScaleX * (rcChildAnchor.left - m_pGroupBounds->left));
		rcChildAnchor.right = m_pGroupClientAnchor->left + (LONG)(dScaleX * (rcChildAnchor.right - m_pGroupBounds->left));

		rcChildAnchor.top = m_pGroupClientAnchor->top + (LONG)(dScaleY * (rcChildAnchor.top - m_pGroupBounds->top));
		rcChildAnchor.bottom = m_pGroupClientAnchor->top + (LONG)(dScaleY * (rcChildAnchor.bottom - m_pGroupBounds->top));
	}

	NSPresentationEditor::ElementType GetTypeElem(SPT eType)
	{
		switch (eType)
		{
			//case sptMin:
		case sptMax:
		case sptNil:
			{
				return etShape;
			}
		case sptPictureFrame:
			{
				return etPicture;
			}
		default:
			{
				return etShape;
			}
		};
		return etShape;
	}

	AVSINLINE CString GetFileName(CString strFilePath)
	{
		int nIndex = strFilePath.ReverseFind(TCHAR('\\'));
		if (-1 != nIndex)
		{
			return strFilePath.Mid(nIndex + 1);
		}
		return strFilePath;
	}

protected:

	void SetUpTextStyle(CString& strText, CTheme* pTheme, CLayout* pLayout, IElement* pElem, CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide = NULL)
	{
		// сначала проверяем на shape
		// затем применяем все настройки по-очереди
		// 1) master + TextMasterStyles
		// 2) persist + TextMasterStyles
		// 3) свои настройки + TextMasterStyles
		// причем "свои настройки" - это чисто "продвинутые настройки"
		// потому что все общие ( через проперти ) - уже установлены
		// тут важно выставить правильный порядок.
		// словом - важная очень функция для текста, 
		// и, чтобы убрать всякие лишние .cpp файлы - здесь же будем учитывать 
		// настройки слайда (т.е. структуры не будут работать со слайдами)

		if (NULL == pElem)
			return;

		if (etShape != pElem->m_etType)
			return;

		CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);
		if (NULL == pShape)
			return;

		CTextAttributesEx* pTextSettings = &(pShape->m_oShape.m_oText);

		// сначала применим ссылки на masterstyle (для шаблонного элемента)
		// как узнать - просто есть ли массивы (т.к. они могли появиться пока только оттуда)
		// - теперь этого делать не нужно - т.к. в мастере тоже вызывается эта функция - 
		// и там все это должно уже примениться
        bool bIsPersistPresentSettings	= false;
        bool bIsOwnPresentSettings		= false;

		NSOfficePPT::TextType eTypeMaster	= NSOfficePPT::NoPresent;
		NSOfficePPT::TextType eTypePersist	= NSOfficePPT::NoPresent;
		NSOfficePPT::TextType eTypeOwn		= (NSOfficePPT::TextType)pTextSettings->m_lTextType;

		CShapeElement* pElementLayoutPH = NULL;

		// выставим тип мастера
		if (NULL != pSlide)
		{
			LONG ph_type	= pShape->m_lPlaceholderType;
			LONG ph_pos		= pShape->m_lPlaceholderID;

			pTextSettings->m_lPlaceholderType = ph_type;

            size_t lElemsCount = 0;

            if (pLayout)
            {
                for (size_t i = 0; i < pLayout->m_arElements.size(); ++i)
                {
                    IElement* pPh = pLayout->m_arElements[i];
                    if ((etShape == pPh->m_etType) && (ph_type == pPh->m_lPlaceholderType) && (/*ph_pos == pPh->m_lPlaceholderID*/true))
                    {
                        pElementLayoutPH = dynamic_cast<CShapeElement*>(pPh);
                        eTypeMaster = (NSOfficePPT::TextType)pElementLayoutPH->m_oShape.m_oText.m_lTextMasterType;
                        break;
                    }
                }
            }
		}
		else
		{
			eTypeMaster = (NSOfficePPT::TextType)pTextSettings->m_lTextMasterType;
		}

		// ------------------------------------------------------------------------------
		CElementInfo oElemInfo;
		std::map<LONG, CElementInfo>::iterator pPair = pSlideWrapper->m_mapElements.find(pShape->m_lID);
		if (pSlideWrapper->m_mapElements.end() != pPair)
			oElemInfo = pPair->second;

		//  persist ----------------------------------------------------------------------
		std::vector<CTextFullSettings>* pArrayPlaseHolders	= &pSlideWrapper->m_arTextPlaceHolders;
		LONG lCountPersistObjects							= (LONG)pArrayPlaseHolders->size();
		LONG lPersistIndex									= oElemInfo.m_lPersistIndex;

		if ((lPersistIndex >= 0) && (lPersistIndex < lCountPersistObjects))
		{
			CTextFullSettings* pSettings = &pArrayPlaseHolders->at(lPersistIndex);

			eTypePersist = (NSOfficePPT::TextType)pSettings->m_nTextType;
			strText = pSettings->ApplyProperties(pTextSettings);

			if ((0 != pSettings->m_arRanges.size()) && (0 == pShape->m_oActions.m_arRanges.size()))
			{
				pShape->m_oActions.m_bPresent = true;
				
				pShape->m_oActions.m_arRanges = pSettings->m_arRanges;
			}

			bIsPersistPresentSettings = ((NULL != pSettings->m_pTextStyleProp) && (0 < pSettings->m_pTextStyleProp->m_lCount));
		}
		//  ------------------------------------------------------------------------------

		//  own properties ---------------------------------------------------------------
		if (NULL != oElemInfo.m_pStream)
		{
			// теперь нужно загрузить продвинутые настройки текста из стрима.
			LONG lPosition = 0; StreamUtils::StreamPosition(lPosition, oElemInfo.m_pStream);

			if (-1 != oElemInfo.m_lOffsetTextStyle)
			{
				StreamUtils::StreamSeek(oElemInfo.m_lOffsetTextStyle - 8, oElemInfo.m_pStream);

				SRecordHeader oHeader;
				oHeader.ReadFromStream(oElemInfo.m_pStream) ;	

				if (RECORD_TYPE_STYLE_TEXTPROP_ATOM == oHeader.RecType)
				{			
					CRecordStyleTextPropAtom* pStyle = new CRecordStyleTextPropAtom();
					pStyle->m_lCount = strText.GetLength();

					pStyle->ReadFromStream(oHeader, oElemInfo.m_pStream);

					NSPresentationEditor::ConvertPPTTextToEditorStructure(pStyle->m_arrPFs, pStyle->m_arrCFs, strText, pShape->m_oShape.m_oText);

					bIsOwnPresentSettings = (0 < pStyle->m_lCount);

					RELEASEOBJECT(pStyle);
				}
			}

			if (-1 != oElemInfo.m_lOffsetTextProp)
			{
				StreamUtils::StreamSeek(oElemInfo.m_lOffsetTextProp - 8, oElemInfo.m_pStream);

				SRecordHeader oHeader;
				oHeader.ReadFromStream(oElemInfo.m_pStream) ;	

				if (RECORD_TYPE_TEXTSPECINFO_ATOM == oHeader.RecType)
				{			
					CRecordTextSpecInfoAtom* pSpecInfo = new CRecordTextSpecInfoAtom();
					pSpecInfo->m_lCount = strText.GetLength();

					pSpecInfo->ReadFromStream(oHeader, oElemInfo.m_pStream);
					pSpecInfo->ApplyProperties(&(pShape->m_oShape.m_oText));

					RELEASEOBJECT(pSpecInfo);
				}
			}

			StreamUtils::StreamSeek(lPosition, oElemInfo.m_pStream);
		}
		//  ------------------------------------------------------------------------------

		// теперь выставляем все настройки текста (стили)
		if (NULL == pSlide)
		{
			int nTextMasterType = (int)eTypeMaster;
			if (-1 != pShape->m_lPlaceholderType)
			{
				switch (oElemInfo.m_lMasterPlaceholderType)
				{
				case NSOfficePPT::Title:
				case NSOfficePPT::MasterTitle:
				case NSOfficePPT::VerticalTextTitle:
					{
						pTextSettings->m_lTextType = 1;

						if (NSOfficePPT::_Title != eTypeMaster)
						{
							if (0 <= nTextMasterType && nTextMasterType < 9)
							{
								if (pThemeWrapper->m_pStyles[nTextMasterType].is_init())
									pTextSettings->m_oStyles = pThemeWrapper->m_pStyles[nTextMasterType].get();
							}
						}
						break;
					}
				case NSOfficePPT::CenteredTitle:
				case NSOfficePPT::MasterCenteredTitle:
					{
						pTextSettings->m_lTextType = 1;

						if (NSOfficePPT::_Title != eTypeMaster)
						{
							if (0 <= nTextMasterType && nTextMasterType < 9)
							{
								if (pThemeWrapper->m_pStyles[nTextMasterType].is_init())
									pTextSettings->m_oStyles = pThemeWrapper->m_pStyles[nTextMasterType].get();
							}
						}
						break;
					}
				case NSOfficePPT::Body:
				case NSOfficePPT::MasterBody:
				case NSOfficePPT::NotesBody:
				case NSOfficePPT::MasterNotesBody:
				case NSOfficePPT::VerticalTextBody:
				case NSOfficePPT::MasterSubtitle:
				case NSOfficePPT::Subtitle:
					{
						pTextSettings->m_lTextType = 2;

						if (NSOfficePPT::_Body != eTypeMaster)
						{
							if (0 <= nTextMasterType && nTextMasterType < 9)
							{
								if (pThemeWrapper->m_pStyles[nTextMasterType].is_init())
									pTextSettings->m_oStyles = pThemeWrapper->m_pStyles[nTextMasterType].get();
							}
						}
						break;
					}
				default:
					{
						pTextSettings->m_lTextType = 3;

						if (NSOfficePPT::Other != eTypeMaster)
						{
							if (0 <= nTextMasterType && nTextMasterType < 9)
							{
								if (pThemeWrapper->m_pStyles[nTextMasterType].is_init())
									pTextSettings->m_oStyles = pThemeWrapper->m_pStyles[nTextMasterType].get();
							}
						}
						break;
					}
				}
			}
			else
			{
				pTextSettings->m_lTextType = 0;

				if (NSOfficePPT::Other != eTypeMaster)
				{
					if (0 <= nTextMasterType && nTextMasterType < 9)
					{
						if (pThemeWrapper->m_pStyles[nTextMasterType].is_init())
							pTextSettings->m_oStyles = pThemeWrapper->m_pStyles[nTextMasterType].get();
					}
				}
			}

			// теперь смотрим все остальные стили (persist и own) - просто применяем их к m_oStyles
			if (eTypePersist != NSOfficePPT::NoPresent && eTypePersist != eTypeMaster)
			{
				int nIndexType = (int)eTypePersist;
				if (0 <= nIndexType && nIndexType < 9)
				{
					if (pThemeWrapper->m_pStyles[nIndexType].is_init())
						pTextSettings->m_oStyles.ApplyAfter(pThemeWrapper->m_pStyles[nIndexType].get());
				}
			}
			if (eTypeOwn != NSOfficePPT::NoPresent && eTypeOwn != eTypePersist && eTypeOwn != eTypeMaster)
			{
				int nIndexType = (int)eTypeOwn;
				if (0 <= nIndexType && nIndexType < 9)
				{
					if (pThemeWrapper->m_pStyles[nIndexType].is_init())
						pTextSettings->m_oStyles.ApplyAfter(pThemeWrapper->m_pStyles[nIndexType].get());
				}
			}
		}
		else
		{
			if (-1 != pShape->m_lPlaceholderType)
			{
				if (NULL != pElementLayoutPH)
				{
					pTextSettings->m_oLayoutStyles = pElementLayoutPH->m_oShape.m_oText.m_oStyles;
					pTextSettings->m_lTextType = pElementLayoutPH->m_oShape.m_oText.m_lTextType;
				}
				else
				{	
					switch (oElemInfo.m_lMasterPlaceholderType)
					{
					case NSOfficePPT::Title:
					case NSOfficePPT::MasterTitle:
					case NSOfficePPT::VerticalTextTitle:
						{
							pTextSettings->m_lTextType = 1;
							break;
						}
					case NSOfficePPT::CenteredTitle:
					case NSOfficePPT::MasterCenteredTitle:
						{
							pTextSettings->m_lTextType = 1;
							break;
						}
					case NSOfficePPT::Body:
					case NSOfficePPT::MasterBody:
					case NSOfficePPT::NotesBody:
					case NSOfficePPT::MasterNotesBody:
					case NSOfficePPT::VerticalTextBody:
						{
							pTextSettings->m_lTextType = 2;
							break;
						}
					default:
						{
							pTextSettings->m_lTextType = 3;
							break;
						}
					}
				}
			}
			else
			{
				pTextSettings->m_lTextType = 0;
			}

			// теперь смотрим все остальные стили (persist и own) - просто применяем их к m_oStyles
			if (eTypePersist != NSOfficePPT::NoPresent && eTypePersist != eTypeMaster)
			{
				int nIndexType = (int)eTypePersist;
				if (0 <= nIndexType && nIndexType < 9)
				{
					if (pThemeWrapper->m_pStyles[nIndexType].is_init())
						pTextSettings->m_oStyles.ApplyAfter(pThemeWrapper->m_pStyles[nIndexType].get());
				}
			}
			if (eTypeOwn != NSOfficePPT::NoPresent && eTypeOwn != eTypePersist && eTypeOwn != eTypeMaster)
			{
				int nIndexType = (int)eTypeOwn;
                if (0 <= nIndexType && nIndexType < 9 && pLayout)
				{
                    if (pThemeWrapper->m_pStyles[nIndexType].IsInit())
						pTextSettings->m_oStyles.ApplyAfter(pThemeWrapper->m_pStyles[nIndexType].get());
				}
			}
		}

		if ((_T("") != strText) && 0 == pTextSettings->m_arParagraphs.size())
		{
			// значит никаких своих настроек нету. Значит просто пустые свои настройки
			std::vector<CTextPFRun_ppt> oArrayPF;
			
			CTextPFRun_ppt elm;
			
			elm.m_lCount = strText.GetLength();
			elm.m_lLevel = 0;
			
			oArrayPF.push_back(elm);

			std::vector<CTextCFRun_ppt> oArrayCF;
			
			CTextCFRun_ppt elm1;
			elm1.m_lCount = elm.m_lCount;
			
			oArrayCF.push_back(elm1);
			
			NSPresentationEditor::ConvertPPTTextToEditorStructure(oArrayPF, oArrayCF, strText, *pTextSettings);
		}

		NSPresentationEditor::CColor oColor;
		if ((NULL != pSlide) && !pSlide->m_bUseLayoutColorScheme)
		{
			oColor = pSlide->GetColor(11);
		}
        else if ((NULL != pLayout) && (!pLayout->m_bUseThemeColorScheme))
		{
			oColor = pLayout->GetColor(11);
		}
		else if (NULL != pTheme)
		{
			oColor = pTheme->GetColor(11);
		}
		oColor.m_lSchemeIndex = 11;

		if (pShape->m_oActions.m_bPresent)
		{
			ApplyHyperlink(pShape, oColor);
		}

		CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(pShape->m_oShape.m_pShape);

		if (NULL != pPPTShape)
		{
			// теперь смотрим - что за фигура создана
			switch (pPPTShape->m_eType)
			{
			case sptTextPlainText:    
			case sptTextStop:  
			case sptTextTriangle:   
			case sptTextTriangleInverted:
			case sptTextChevron:
			case sptTextChevronInverted:
			case sptTextRingInside:
			case sptTextRingOutside:
			case sptTextArchUpCurve:   
			case sptTextArchDownCurve: 
			case sptTextCircleCurve: 
			case sptTextButtonCurve: 
			case sptTextArchUpPour:  
			case sptTextArchDownPour: 
			case sptTextCirclePour:
			case sptTextButtonPour:  
			case sptTextCurveUp:  
			case sptTextCurveDown: 
			case sptTextCascadeUp:   
			case sptTextCascadeDown:
			case sptTextWave1:   
			case sptTextWave2:   
			case sptTextWave3:   
			case sptTextWave4: 
			case sptTextInflate:   
			case sptTextDeflate:    
			case sptTextInflateBottom:  
			case sptTextDeflateBottom:  
			case sptTextInflateTop:
			case sptTextDeflateTop:   
			case sptTextDeflateInflate:   
			case sptTextDeflateInflateDeflate:
			case sptTextFadeRight: 
			case sptTextFadeLeft:   
			case sptTextFadeUp:   
			case sptTextFadeDown:   
			case sptTextSlantUp:    
			case sptTextSlantDown:   
			case sptTextCanUp:   
			case sptTextCanDown:
				{
					pShape->m_oShape.m_oText.m_arParagraphs.clear();

					pShape->m_oShape.m_oText.m_oAttributes.m_oTextBrush = pShape->m_oShape.m_oBrush;

					pShape->m_oShape.m_oText.m_oAttributes.m_nTextAlignHorizontal	= 1;
					pShape->m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical		= 1;

					pShape->m_oShape.m_lDrawType = c_ShapeDrawType_Text;
					break;
				}
			default:
				break;
			};
		}
	}

	void ApplyHyperlink(CShapeElement* pShape, CColor& oColor)
	{
		std::vector<CTextRange>* pRanges						= &pShape->m_oActions.m_arRanges;
		CTextAttributesEx* pTextAttributes					= &pShape->m_oShape.m_oText;

		LONG lCountHyper	= (LONG)pRanges->size();

		if (0 == lCountHyper)
			return;

		size_t nCountPars = pTextAttributes->m_arParagraphs.size();
		for (LONG nIndexRange = 0; nIndexRange < lCountHyper; ++nIndexRange)
		{
			LONG lStart = (*pRanges)[nIndexRange].m_lStart;
			LONG lEnd	= (*pRanges)[nIndexRange].m_lEnd;

			LONG lCurrentStart = 0;
			for (size_t nIndexPar = 0; nIndexPar < nCountPars; ++nIndexPar)
			{
				CParagraph* pParagraph = &pTextAttributes->m_arParagraphs[nIndexPar];

				for (size_t nIndexSpan = 0; nIndexSpan < pParagraph->m_arSpans.size(); ++nIndexSpan)
				{
					LONG lCurrentEnd = lCurrentStart + pParagraph->m_arSpans[nIndexSpan].m_strText.GetLength() - 1;

					if (lCurrentStart > lEnd || lCurrentEnd < lStart)
					{
						lCurrentStart = lCurrentEnd + 1;
						continue;
					}

                    LONG lStart_	= (std::max)(lStart, lCurrentStart);
                    LONG lEnd_		= (std::min)(lEnd, lCurrentEnd);

					CSpan oRunProp = pParagraph->m_arSpans[nIndexSpan];

					CString strText = pParagraph->m_arSpans[nIndexSpan].m_strText;
					if (lStart_ > lCurrentStart)
					{
						pParagraph->m_arSpans.insert(pParagraph->m_arSpans.begin()  + nIndexSpan, oRunProp);
						pParagraph->m_arSpans[nIndexSpan].m_strText = strText.Mid(0, lStart_ - lCurrentStart);

						++nIndexSpan;
					}
					pParagraph->m_arSpans[nIndexSpan].m_oRun.Color = oColor;
                    pParagraph->m_arSpans[nIndexSpan].m_oRun.FontUnderline = (bool)true;
					pParagraph->m_arSpans[nIndexSpan].m_strText = strText.Mid(lStart_ - lCurrentStart, lEnd_ - lStart_ + 1);
					if (lEnd_ < lCurrentEnd)
					{
						pParagraph->m_arSpans.insert(pParagraph->m_arSpans.begin() + nIndexSpan + 1, oRunProp);
						++nIndexSpan;

						pParagraph->m_arSpans[nIndexSpan].m_strText = strText.Mid(lEnd_ - lCurrentStart + 1, lCurrentEnd - lEnd_);
					}

					lCurrentStart = lCurrentEnd + 1;
				}
			}			
		}
	}	
};
