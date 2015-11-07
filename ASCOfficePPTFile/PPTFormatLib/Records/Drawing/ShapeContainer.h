#pragma once

#include "Shape.h"
#include "ShapeProperties.h"

#include "../ExObjRefAtom.h"
#include "../TextBytesAtom.h"
#include "../TextCharsAtom.h"
#include "../TextHeaderAtom.h"
#include "../PlaceHolderAtom.h"
#include "../StyleTextPropAtom.h"
#include "../OutlineTextRefAtom.h"
#include "../InteractiveInfoAtom.h"
#include "../TextInteractiveInfoAtom.h"
#include "../MasterTextPropAtom.h"
#include "../HeadersFootersAtom.h"

#include "../../Reader/ClassesAtom.h"
#include "../../Reader/SlideInfo.h"

#include "../../../../ASCPresentationEditor/OfficeDrawing/Document.h"
#include "../../../../ASCPresentationEditor/OfficeDrawing/Shapes/BaseShape/PPTShape/ElementSettings.h"

#include "../../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../../Common/DocxFormat/Source/Base/Types_32.h"

const double EMU_MM = 36000;

#define		FIXED_POINT(val) (double)((short)(val >> 16) + ((short)(val) / 65536.0));


using namespace NSOfficeDrawing;
using namespace NSPresentationEditor;

class CPPTElement
{
public:

	bool ChangeBlack2ColorImage(std::wstring image_path, int rgbColor1, int rgbColor2)
	{
		CBgraFrame bgraFrame;

		if (bgraFrame.OpenFile(image_path))
		{
			int smpl = abs(bgraFrame.get_Stride() / bgraFrame.get_Width());

			BYTE * rgb = bgraFrame.get_Data();
			
			BYTE R1 = (BYTE)(rgbColor1);
			BYTE G1 = (BYTE)(rgbColor1 >> 8);
			BYTE B1 = (BYTE)(rgbColor1 >> 16);

			BYTE R2 = (BYTE)(rgbColor2);
			BYTE G2 = (BYTE)(rgbColor2 >> 8);
			BYTE B2 = (BYTE)(rgbColor2 >> 16);

			for (int i = 0 ; i < bgraFrame.get_Width() * bgraFrame.get_Height(); i++)
			{
				if (rgb[i * smpl + 0 ] == 0x00 && rgb[i * smpl + 1 ] == 0x00 && rgb[i * smpl + 2 ] == 0x00)
				{
					rgb[i * smpl + 0 ] = R1;
					rgb[i * smpl + 1 ] = G1;
					rgb[i * smpl + 2 ] = B1;
				}
				else
				{
					rgb[i * smpl + 0 ] = R2;
					rgb[i * smpl + 1 ] = G2;
					rgb[i * smpl + 2 ] = B2;
				}
			}
			bgraFrame.SaveFile(image_path, 1); 
			return true;
		}
		return false;
	}
	CColor CorrectSysColor(int nColorCode, IElement* pElement, CTheme* pTheme)
	{
		if (pElement == NULL) return CColor();

		CColor  color;

		unsigned short nParameter		= (unsigned short)(( nColorCode >> 16 ) & 0x00ff);  // the HiByte of nParameter is not zero, an exclusive AND is helping :o
		unsigned short nFunctionBits	= (unsigned short)(( nColorCode	& 0x00000f00 ) >> 8 );
		unsigned short nAdditionalFlags = (unsigned short)(( nColorCode	& 0x0000f000) >> 8 );
		unsigned short nColorIndex		= (unsigned short) ( nColorCode	& 0x00ff);
		unsigned short nPropColor		= 0;

		switch (nColorIndex)
		{
		case 0xF0:	color = pElement->m_oBrush.Color1;	break;
		case 0xF1:
		{
			if (pElement->m_bLine)	color = pElement->m_oPen.Color;
			else					color = pElement->m_oBrush.Color1;
		}break;
		case 0xF2:	color = pElement->m_oPen.Color;		break;
		case 0xF3:	color = pElement->m_oShadow.Color;	break;
		case 0xF4:	break; ///this
		case 0xF5:	color = pElement->m_oBrush.Color2;	break; 
		case 0xF6:	break; //lineBackColor
		case 0xF7:	//FillThenLine
		case 0xF8:	//colorIndexMask
			color = pElement->m_oBrush.Color1;	break;
		default:
			//from table 
			break;
		}

		if (color.m_lSchemeIndex != -1)
		{
			//вытащить цвет (

			color = pTheme->m_arColorScheme[color.m_lSchemeIndex];
		}

        //if ( nCProp && ( nPropColor & 0x10000000 ) == 0 )       // beware of looping recursive
        //    color = CorrectSysColor( nPropColor, pElement);

        if( nAdditionalFlags & 0x80 )           // make color gray
        {
            BYTE nZwi = 0x7f;//= aColor.GetLuminance();
			color.SetRGB(nZwi, nZwi, nZwi);
        }
        switch( nFunctionBits )
        {
            case 0x01 :     // darken color by parameter
            {
                color.SetR	( static_cast<BYTE>	( ( nParameter * color.GetR() ) >> 8 ) );
                color.SetG	( static_cast<BYTE>	( ( nParameter * color.GetG() ) >> 8 ) );
                color.SetB	( static_cast<BYTE> ( ( nParameter * color.GetB() ) >> 8 ) );
            }
            break;
            case 0x02 :     // lighten color by parameter
            {
                unsigned short nInvParameter = ( 0x00ff - nParameter ) * 0xff;
                color.SetR( static_cast<BYTE>( ( nInvParameter + ( nParameter * color.GetR() ) ) >> 8 ) );
                color.SetG( static_cast<BYTE>( ( nInvParameter + ( nParameter * color.GetG() ) ) >> 8 ) );
                color.SetB( static_cast<BYTE>( ( nInvParameter + ( nParameter * color.GetB() ) ) >> 8 ) );
            }
            break;
            case 0x03 :     // add grey level RGB(p,p,p)
            {
                short nR = (short)color.GetR()	+ (short)nParameter;
                short nG = (short)color.GetG()	+ (short)nParameter;
                short nB = (short)color.GetB()	+ (short)nParameter;

                if ( nR > 0x00ff )	nR = 0x00ff;
                if ( nG > 0x00ff )	nG = 0x00ff;
                if ( nB > 0x00ff )	nB = 0x00ff;

               color.SetRGB( (BYTE)nR, (BYTE)nG, (BYTE)nB );
            }
            break;
            case 0x04 :     // substract grey level RGB(p,p,p)
            {
                short nR = (short)color.GetR() - (short)nParameter;
                short nG = (short)color.GetG() - (short)nParameter;
                short nB = (short)color.GetB() - (short)nParameter;
                if ( nR < 0 ) nR = 0;
                if ( nG < 0 ) nG = 0;
                if ( nB < 0 ) nB = 0;
                color.SetRGB( (BYTE)nR, (BYTE)nG, (BYTE)nB );
            }
            break;
            case 0x05 :     // substract from gray level RGB(p,p,p)
            {
                short nR = (short)nParameter - (short)color.GetR();
                short nG = (short)nParameter - (short)color.GetG();
                short nB = (short)nParameter - (short)color.GetB();
                if ( nR < 0 ) nR = 0;
                if ( nG < 0 ) nG = 0;
                if ( nB < 0 ) nB = 0;
                color.SetRGB( (BYTE)nR, (BYTE)nG, (BYTE)nB );
            }
            break;
            case 0x06 :     // per component: black if < p, white if >= p
            {
                color.SetR( color.GetR() < nParameter ? 0x00 : 0xff );
                color.SetG( color.GetG() < nParameter ? 0x00 : 0xff );
                color.SetB( color.GetB() < nParameter ? 0x00 : 0xff );
            }
            break;
        }
        if ( nAdditionalFlags & 0x40 )                  // top-bit invert
            color.SetRGB( color.GetR() ^ 0x80, color.GetG() ^ 0x80, color.GetB() ^ 0x80 );

        if ( nAdditionalFlags & 0x20 )                  // invert color
            color.SetRGB(0xff - color.GetR(), 0xff - color.GetG(), 0xff - color.GetB());

		return color;
	}
	inline void SetUpProperties(IElement* pElement, CTheme* pTheme, CSlideInfo* pWrapper, CSlide* pSlide, CProperties* pProperties)
	{
		long lCount = pProperties->m_lCount;
		switch (pElement->m_etType)
		{
		case NSPresentationEditor::etVideo:
			{
				//default -> line = false
				pElement->m_bLine = false;
				for (long i = 0; i < lCount; ++i)
				{
					SetUpPropertyVideo((CVideoElement*)pElement, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
				}
				break;
			}
		case NSPresentationEditor::etPicture:
			{
				//default -> line = false
				pElement->m_bLine = false;
				for (long i = 0; i < lCount; ++i)
				{
					SetUpPropertyImage((CImageElement*)pElement, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
				}
				break;
			}
		case NSPresentationEditor::etAudio:
			{
				//default -> line = false
				pElement->m_bLine = false;
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

	inline void SetUpProperty(IElement* pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
	{
		bool bIsFilled	= true;

		switch (pProperty->m_ePID)
		{
		case wzName:
			{
				pElement->m_sName = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue /2 - 1); 
			}break;
		case wzDescription:
			{
				pElement->m_sDescription = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue /2 - 1); 
			}break;
		case hspMaster:
			{
				pElement->m_lLayoutID = (LONG)pProperty->m_lValue; 
			}break;
		case rotation:
			{
				pElement->m_dRotate = FIXED_POINT(pProperty->m_lValue);
			}break;
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
			}break;
		case fillType:
			{
				DWORD dwType = pProperty->m_lValue;
				switch(dwType)
				{
					case NSOfficeDrawing::fillPattern:
					{
						pElement->m_oBrush.Type = c_BrushTypePattern;
						//texture + change black to color2, white to color1
					}break;
					case NSOfficeDrawing::fillTexture :
					case NSOfficeDrawing::fillPicture :
					{
						pElement->m_oBrush.Type			= c_BrushTypeTexture;
						pElement->m_oBrush.TextureMode	= (NSOfficeDrawing::fillPicture == dwType) ? c_BrushTextureModeStretch : c_BrushTextureModeTile;
					}break;
					case NSOfficeDrawing::fillShadeCenter://1 color
					case NSOfficeDrawing::fillShadeShape:
					{
						pElement->m_oBrush.Type = c_BrushTypeCenter;
					}break;//
					case NSOfficeDrawing::fillShadeTitle://2 colors and more
					case NSOfficeDrawing::fillShade : 
					case NSOfficeDrawing::fillShadeScale: 
					{
						pElement->m_oBrush.Type = c_BrushTypePathGradient1;
					}break;
					case NSOfficeDrawing::fillBackground:
					{
						pElement->m_oBrush.Type = c_BrushTypeNoFill;
					}break;				
				}
			}break;
		case fillBlip:
			{
				int dwOffset = 0 ;
					
				if (pProperty->m_bComplex)
				{
					//inline 
					dwOffset = -1;
				}
				else
				{
					dwOffset = pInfo->GetIndexPicture(pProperty->m_lValue);
				}
				int nLen	= pElement->m_oBrush.TexturePath.length() - 1;
				int nIndex	= pElement->m_oBrush.TexturePath.rfind(FILE_SEPARATOR_CHAR);
				if (nLen != nIndex)
				{
					pElement->m_oBrush.TexturePath.erase(nIndex + 1, nLen - nIndex);
				}				
				
				pElement->m_oBrush.TexturePath = pElement->m_oBrush.TexturePath + pInfo->GetFileNamePicture(dwOffset);
				if (pElement->m_oBrush.Type == c_BrushTypePattern)
				{
					int rgbColor1 = 0xFFFFFF;
					int rgbColor2 = 0;

					if (pElement->m_oBrush.Color1.m_lSchemeIndex == -1)
					{
						rgbColor1 = pElement->m_oBrush.Color1.GetLONG_RGB();
					}
					else
					{
						if ((pSlide) && (pSlide->m_arColorScheme.size() > 0))
						{
							rgbColor1 = pSlide->m_arColorScheme[pElement->m_oBrush.Color1.m_lSchemeIndex].GetLONG_RGB();
						}
						else if ((pTheme) && (pTheme->m_arColorScheme.size() > 0))
						{
							rgbColor1 = pTheme->m_arColorScheme[pElement->m_oBrush.Color1.m_lSchemeIndex].GetLONG_RGB();
						}
					}
					if (pElement->m_oBrush.Color2.m_lSchemeIndex == -1)
					{
						rgbColor2 = pElement->m_oBrush.Color2.GetLONG_RGB();
					}
					else
					{
						if ((pSlide) && (pSlide->m_arColorScheme.size() > 0))
						{
							rgbColor2 = pSlide->m_arColorScheme[pElement->m_oBrush.Color2.m_lSchemeIndex].GetLONG_RGB();
						}
						else if ((pTheme) && (pTheme->m_arColorScheme.size() > 0))
						{
							rgbColor2 = pTheme->m_arColorScheme[pElement->m_oBrush.Color2.m_lSchemeIndex].GetLONG_RGB();
						}
					}
					ChangeBlack2ColorImage(pElement->m_oBrush.TexturePath, rgbColor2, rgbColor1);
					
					pElement->m_oBrush.Type			= c_BrushTypeTexture;
					pElement->m_oBrush.TextureMode	= c_BrushTextureModeTile;
				}
				break;
			}
		case fillColor:
			{
				SColorAtom oAtom;
				oAtom.FromValue(pProperty->m_lValue);

				if(oAtom.bSysIndex)
					pElement->m_oBrush.Color1 = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
				else
					oAtom.ToColor(&pElement->m_oBrush.Color1);

				if (pElement->m_oBrush.Type == c_BrushTypeNoFill )
					pElement->m_oBrush.Type = c_BrushTypeSolid;

				break;
			}
		case fillBackColor:
			{
				SColorAtom oAtom;
				oAtom.FromValue(pProperty->m_lValue);				

				if(oAtom.bSysIndex)
					pElement->m_oBrush.Color2 = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
				else
					oAtom.ToColor(&pElement->m_oBrush.Color2);
			}break;
		case fillOpacity:
			{
                pElement->m_oBrush.Alpha1 = (BYTE)(std::min)(255, (int)CDirectory::NormFixedPoint(pProperty->m_lValue, 255));
				break;
			}
		case fillBackOpacity:
			{
                pElement->m_oBrush.Alpha2 = (BYTE)(std::min)(255, (int)CDirectory::NormFixedPoint(pProperty->m_lValue, 255));
				break;
			}
		case fillRectLeft:
			{
				pElement->m_oBrush.Rect.X	= pProperty->m_lValue;
				pElement->m_oBrush.Rectable = true;
			}break;
		case fillRectRight:
			{
				pElement->m_oBrush.Rect.Width	= pProperty->m_lValue - pElement->m_oBrush.Rect.X;
				pElement->m_oBrush.Rectable		= true;
			}break;
		case fillRectTop:
			{
				pElement->m_oBrush.Rect.Y	= pProperty->m_lValue;
				pElement->m_oBrush.Rectable = true;
			}break;
		case fillRectBottom:
			{
				pElement->m_oBrush.Rect.Height	= pProperty->m_lValue - pElement->m_oBrush.Rect.Y;
				pElement->m_oBrush.Rectable		= true;
			}break;
		case fillBackground:
			{
				//bIsFilled = false;
				break;
			}
		case fillShadeType:
			{
				bool bShadeNone		= GETBIT(pProperty->m_lValue, 31);
				bool bShadeGamma	= GETBIT(pProperty->m_lValue, 30);
				bool bShadeSigma	= GETBIT(pProperty->m_lValue, 29);
				bool bShadeBand		= GETBIT(pProperty->m_lValue, 28);
				bool bShadeOneColor	= GETBIT(pProperty->m_lValue, 27);

			}break;
		case fillAngle:
			{
			}break;
		case fillFocus://relative position of the last color in the shaded fill
			{
			}break;
		case fillShadePreset:
			{//value (int) from 0x00000088 through 0x0000009F or complex
			}break;
		case fillShadeColors:
			{//array of colors
				struct _a
				{
					_INT32	color;
					_INT32 position;
				};
				std::vector<_a> colors;
				
				unsigned short nElems;
				
				nElems = pProperty->m_lValue;
                _INT32* pCompl = (_INT32*)pProperty->m_pOptions;
				while(nElems--)
				{
					_a a1;
                    a1.color	= *pCompl; pCompl++;
                    a1.position = *pCompl; pCompl++;
					colors.push_back(a1);
				}

			}break;
		case fNoFillHitTest:
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

				//if (bUseLineOk)
				//	pElement->m_bLine = bLineOk;//?? todooo проверить - не сраюатывает ! 1 (82).ppt

				if (bUseFillOk)
					bIsFilled = bFillOk;

				break;
			}
// line --------------------------------------------------------
		case fNoLineDrawDash: //Line Style Boolean Properties
			{
				bool bNoLineDrawDash		= GETBIT(pProperty->m_lValue, 0);
				bool bLineFillShape			= GETBIT(pProperty->m_lValue, 1);
				bool bHitTestLine			= GETBIT(pProperty->m_lValue, 2);
				bool bLine					= GETBIT(pProperty->m_lValue, 3);
				bool bArrowheadsOK			= GETBIT(pProperty->m_lValue, 4);
				bool bInsetPenOK			= GETBIT(pProperty->m_lValue, 5);
				bool bInsetPen				= GETBIT(pProperty->m_lValue, 6);
				bool bLineOpaqueBackColor	= GETBIT(pProperty->m_lValue, 9);

				bool bUsefNoLineDrawDash	= GETBIT(pProperty->m_lValue, 16);
				bool bUsefLineFillShape		= GETBIT(pProperty->m_lValue, 17);
				bool bUsefHitTestLine		= GETBIT(pProperty->m_lValue, 18);
				bool bUsefLine				= GETBIT(pProperty->m_lValue, 19);
				bool bUsefArrowheadsOK		= GETBIT(pProperty->m_lValue, 20);
				bool bUsefInsetPenOK		= GETBIT(pProperty->m_lValue, 21);
				bool bUsefInsetPen			= GETBIT(pProperty->m_lValue, 22);
				bool bUsefLineOpaqueBackColor = GETBIT(pProperty->m_lValue, 25);

				if (bUsefLine)
					pElement->m_bLine = bLine;				
			}break;
		case lineStyle:
		case lineDashStyle://from Complex
			{
				pElement->m_bLine = true;	
			}break;
		case lineColor:
			{
				SColorAtom oAtom;
				oAtom.FromValue(pProperty->m_lValue);
				
				if (oAtom.bSysIndex)
					pElement->m_oPen.Color = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
				else
					oAtom.ToColor(&pElement->m_oPen.Color);
				break;
			}
		case lineOpacity:
			{
                pElement->m_oPen.Alpha = (BYTE)(std::min)(255, (int)CDirectory::NormFixedPoint(pProperty->m_lValue, 255));
				break;
			}
		case lineBackColor:
			{
				SColorAtom oAtom;
				oAtom.FromValue(pProperty->m_lValue);
				
				if (oAtom.bSysIndex)
					pElement->m_oPen.Color2 = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
				else 
					oAtom.ToColor(&pElement->m_oPen.Color2);

			}break;
		case lineWidth:
			{
				pElement->m_oPen.Size	= (double)pProperty->m_lValue / EMU_MM;
				pElement->m_bLine		= true;
				break;
			}
		case lineDashing:
			{
				BYTE nDashStyle = 0;
				switch (pProperty->m_lValue)
				{
				case 0: 
					{ 
						nDashStyle = 0; // solid 						 
					}break;
				case 1: 
				case 6:
				case 7:
					{ 
						nDashStyle = 1; // dash 						
					}break; 
				case 2:
				case 5:
					{ 
						nDashStyle = 2; // dot 						 
					}break;
				case 3:
				case 8:
				case 9:
					{ 
						nDashStyle = 3; // dashdot 						
					}break; 
				case 4:
				case 10:
					{ 
						nDashStyle = 4;// dashdotdot 						
					}break; 
				default:
						break; 
				};
				pElement->m_bLine				= true;
				pElement->m_oPen.DashStyle	= nDashStyle;
				break;
			}
		case lineJoinStyle:
			{
				BYTE nLineJoin = 2;
				switch (pProperty->m_lValue)
				{
				case 0: 
					{ 
						nLineJoin = 1; // bevel 						 
					}break;
				case 1: 
					{ 
						nLineJoin = 1; // Miter 						 
					}break;
				case 2:
					{ 
						nLineJoin = 2; // round 						 
					}break;	
				default:
						break; 
				};

				pElement->m_oPen.LineJoin = nLineJoin;
				break;
			}
		case lineStartArrowhead:
			{
				BYTE nStartCap = 0;
				switch (pProperty->m_lValue)
				{
				case 1: 
				case 2:
				case 5:
					{ 
						nStartCap = 0x14; 
					}break;
				case 3:
				case 4:

					{ 
						nStartCap = 2; 
					}break; 
				default:
						break; 
				};

				pElement->m_oPen.LineStartCap = nStartCap;

				break;
			}
		case lineEndArrowhead:
			{
				BYTE nEndCap = 0;
				switch (pProperty->m_lValue)
				{
				case 1: 
				case 2:
				case 5:
					{ 
						nEndCap = 0x14; 						
					}break; 
				case 3:
				case 4:

					{ 
						nEndCap = 2; 
					}break; 
				default:
					break; 
				};

				pElement->m_oPen.LineEndCap = nEndCap;
				break;
			}
		case shadowType:
			{
				pElement->m_oShadow.Type = pProperty->m_lValue;

			}break;
		case shadowOriginX://in emu, relative from center shape
			{
				pElement->m_oShadow.OriginX = FIXED_POINT(pProperty->m_lValue);
			}break;
		case shadowOriginY:
			{
				pElement->m_oShadow.OriginY = FIXED_POINT(pProperty->m_lValue);
			}break;
		case shadowColor:
			{
				SColorAtom oAtom;
				oAtom.FromValue(pProperty->m_lValue);

				if (oAtom.bSysIndex)
					pElement->m_oShadow.Color = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
				else
					oAtom.ToColor(&pElement->m_oShadow.Color);

			}break;
		case shadowWeight:
			{
			}break;
		case shadowOpacity:
			{
                pElement->m_oShadow.Alpha = (BYTE)(std::min)(255, (int)CDirectory::NormFixedPoint(pProperty->m_lValue, 255));
			}break;
		case shadowHighlight:
			{
				//оттенок двойной тени
			}break;
		case shadowOffsetX:
			{
				pElement->m_oShadow.DistanceX = ((int)pProperty->m_lValue) / EMU_MM;
			}break;
		case shadowOffsetY:
			{
				pElement->m_oShadow.DistanceY = ((int)pProperty->m_lValue) / EMU_MM;
			}break;
		case shadowScaleXToX:
			{
				pElement->m_oShadow.ScaleXToX = FIXED_POINT(pProperty->m_lValue);
			}break;
		case shadowScaleYToX:
			{
				pElement->m_oShadow.ScaleYToX = FIXED_POINT(pProperty->m_lValue);
			}break;
		case shadowScaleXToY:
			{
				pElement->m_oShadow.ScaleXToY = FIXED_POINT(pProperty->m_lValue);
			}break;
		case shadowScaleYToY:
			{
				pElement->m_oShadow.ScaleYToY = FIXED_POINT(pProperty->m_lValue);
			}break;
		case shadowPerspectiveX:
			{
				pElement->m_oShadow.PerspectiveX = ((int)pProperty->m_lValue);// / EMU_MM;//FIXED_POINT(pProperty->m_lValue);
			}break;
		case shadowPerspectiveY:
			{
				pElement->m_oShadow.PerspectiveY = ((int)pProperty->m_lValue) ;// EMU_MM;//FIXED_POINT(pProperty->m_lValue);
			}break;
		case fshadowObscured:
			{
				bool fshadowObscured		= GETBIT(pProperty->m_lValue, 0);
				bool fShadow				= GETBIT(pProperty->m_lValue, 1);
				bool fUsefshadowObscured	= GETBIT(pProperty->m_lValue, 16);
				bool fUsefShadow			= GETBIT(pProperty->m_lValue, 17);

				if (fUsefShadow)
					pElement->m_oShadow.Visible = fShadow;
				
				if (!fUsefShadow && fUsefshadowObscured)
				{
					//контурная
					pElement->m_oShadow.Visible = fshadowObscured;
				}
			}break;
		default:
			break;
		}

		if (!bIsFilled)
		{
			pElement->m_oBrush.Type = c_BrushTypeNoFill;
		}
	}

	inline void SetUpPropertyVideo(CVideoElement* pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
	{
		SetUpPropertyImage((CImageElement*)pElement, pTheme, pInfo, pSlide, pProperty);
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
				int dwOffset = pInfo->GetIndexPicture(pProperty->m_lValue);
				if (dwOffset >=0)
				{
					pElement->m_strImageFileName	+=  pInfo->GetFileNamePicture(dwOffset);
					pElement->m_bImagePresent		= true;
				}
			}break;
		case pictureId://OLE identifier of the picture.
			{
				pElement->m_bOLE	= true;
			}break;
		case pibName:
			{
				pElement->m_sImageName = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue /2-1);
				// TextMining05.ppt, слайд 20  - некорректное имя ( - todooo потом подчистить его
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
		SetUpProperty((IElement*)pElement, pTheme, pInfo, pSlide, pProperty);

		CShape* pParentShape	= &pElement->m_oShape;
		CPPTShape* pShape		= dynamic_cast<CPPTShape*>(pParentShape->m_pShape);

		CElementProperties* pElemProps = &pElement->m_oProperties;

		if (NULL == pShape)
			return;

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
					pShape->m_oCustomVML.LoadAdjusts(lIndexAdj, (LONG)pProperty->m_lValue);
				}
				else
				{
					pShape->m_oCustomVML.LoadAdjusts(lIndexAdj, (LONG)pProperty->m_lValue);
				}
				break;
			}
//--------------------------------------------------------------------------------------------------------------------
		case lTxid:
			{
			}break;
		case NSOfficeDrawing::dxTextLeft:
			{
				pParentShape->m_dTextMarginX = (double)pProperty->m_lValue / EMU_MM;
				
			}break;
		case NSOfficeDrawing::dxTextRight:
			{
				pParentShape->m_dTextMarginRight = (double)pProperty->m_lValue / EMU_MM;
			}break;
		case NSOfficeDrawing::dyTextTop:
			{
				pParentShape->m_dTextMarginY = (double)pProperty->m_lValue / EMU_MM;
			}break;
		case NSOfficeDrawing::dyTextBottom:
			{
				pParentShape->m_dTextMarginBottom = (double)pProperty->m_lValue / EMU_MM;
			}break;
		case NSOfficeDrawing::WrapText:
			{
				pParentShape->m_oText.m_lWrapMode = (LONG)pProperty->m_lValue;
				
			}break;
		case NSOfficeDrawing::gtextUNICODE://word art text
			{
				if (pProperty->m_bComplex && 0 < pProperty->m_lValue)
				{
					std::wstring str = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue/2-1);

					if (!str.empty() && pParentShape->m_oText.m_arParagraphs.empty())
					{
						int length = str.length();

						for (int i = length-1; i>=0; i--)
						{
							if (str.at(i) > 13 ) break;
							length--;
						}
						NSPresentationEditor::CParagraph p;
						NSPresentationEditor::CSpan s;
						s.m_strText = str.substr(0,length);
						p.m_arSpans.push_back(s);
						pParentShape->m_oText.m_arParagraphs.push_back(p);
					}
				}				
			}break;
		case NSOfficeDrawing::gtextFont:
			{
				if (pProperty->m_bComplex && 0 < pProperty->m_lValue)
				{
					std::wstring str = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue/2-1);
					pParentShape->m_oText.m_oAttributes.m_oFont.Name = std_string2string(str);
				}
				break;
			}
		case NSOfficeDrawing::gtextSize:
			{
				pParentShape->m_oText.m_oAttributes.m_oFont.Size = (INT)((pProperty->m_lValue >> 16) & 0x0000FFFF);
				break;
			}
		case NSOfficeDrawing::anchorText:
			{
				switch (pProperty->m_lValue)
				{
				case NSOfficeDrawing::anchorTop:
				case NSOfficeDrawing::anchorTopBaseline:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 0;
						break;
					}
				case NSOfficeDrawing::anchorMiddle:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 1;
						break;
					}
				case NSOfficeDrawing::anchorBottom:
				case NSOfficeDrawing::anchorBottomBaseline:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 0;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 2;
						break;
					}
				case NSOfficeDrawing::anchorTopCentered:
				case NSOfficeDrawing::anchorTopCenteredBaseline:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 0;
						break;
					}
				case NSOfficeDrawing::anchorMiddleCentered:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 1;
						break;
					}
				case NSOfficeDrawing::anchorBottomCentered:
				case NSOfficeDrawing::anchorBottomCenteredBaseline:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 2;
						break;
					}
				default:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = -1; // not set
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
						break;
					}
				case NSOfficeDrawing::alignTextCenter:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
						break;
					}
				case NSOfficeDrawing::alignTextRight:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 2;
						break;
					}
				default:
					{
						pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
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
				}
				if (bUseShadow)
				{
					pParentShape->m_oText.m_oAttributes.m_oTextShadow.Visible = true;
				}
				if (bUseUnderline)
				{
                    pParentShape->m_oText.m_oAttributes.m_oFont.Underline = (BYTE)bUnderline;
				}
				if (bUseItalic)
				{
                    pParentShape->m_oText.m_oAttributes.m_oFont.Italic = bItalic;
				}
				if (bUseBold)
				{
                    pParentShape->m_oText.m_oAttributes.m_oFont.Bold = bBold;
				}
				if (bUseVertical)
				{
                    pParentShape->m_oText.m_bVertical = (true == bVertical) ? true : false;
				}				
			}break;
		case NSOfficeDrawing::cdirFont:
			{
				switch (pProperty->m_lValue)
				{
				case 1:	
					pParentShape->m_oText.m_bVertical = true;		
					break;
				case 2: 
					pParentShape->m_oText.m_oAttributes.m_dTextRotate = 180;	
					break;
				case 3: 
					pParentShape->m_oText.m_bVertical = true;
					pParentShape->m_oText.m_oAttributes.m_dTextRotate = 180;	
					break;
				}
			}break;
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
					}
				}	
				if (bUseFitShapeToText)
					pParentShape->m_oText.m_bAutoFit = bFitShapeToText;

			}break;

		default:
			break;
		}
	}
};

class CRecordShapeContainer : public CRecordsContainer
{
private:
	POLE::Stream* m_pStream;

public:

	bool bGroupShape;

	RECT* m_pGroupBounds;
	RECT* m_pGroupClientAnchor;

	CRecordShapeContainer()
	{
		bGroupShape = false;

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


	virtual void GetElement (IElement** ppElement, CExMedia* pMapIDs,
							long lSlideWidth, long lSlideHeight, CTheme* pTheme, CLayout* pLayout, 
							CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide = NULL)
	{
		if (NULL == ppElement)	
			return;

		if (bGroupShape) return;

		*ppElement = NULL;

		std::vector<CRecordShape*> oArrayShape;
		GetRecordsByType(&oArrayShape, true, true);

		if (0 == oArrayShape.size())
			return;

		std::vector<CRecordShapeProperties*> oArrayOptions;
		GetRecordsByType(&oArrayOptions, true, /*true*/false/*secondary & tetriary*/);

		PPTShapes::ShapeType eType = (PPTShapes::ShapeType)oArrayShape[0]->m_oHeader.RecInstance;
		ElementType			elType = GetTypeElem((NSOfficeDrawing::SPT)oArrayShape[0]->m_oHeader.RecInstance); 

		int lMasterID = -1;

		IElement * pElementLayout = NULL;
		
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
                                if (elType == pLayout->m_arElements[nIndex]->m_etType)
                                {

									if (pLayout->m_arElements[nIndex]->m_bPlaceholderSet == false)
									{
										pElementLayout = pLayout->m_arElements[nIndex]; //для переноса настроек
										pElementLayout->m_lID = lMasterID;
									}                
									if (lMasterID == pLayout->m_arElements[nIndex]->m_lID)
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
			case sptMax:
			case sptNil:
						break;
			case sptPictureFrame:
				{
					std::vector<CRecordExObjRefAtom*> oArrayEx;
					GetRecordsByType(&oArrayEx, true, true);

					CExFilesInfo oInfo;
					CExFilesInfo oInfoDefault;
					// по умолчанию картинка (или оле объект)
					CExFilesInfo::ExFilesType exType = CExFilesInfo::eftNone;
					CExFilesInfo* pInfo = pMapIDs->Lock(0xFFFFFFFF, exType);
					if (NULL != pInfo)
					{
						oInfo			= *pInfo;
						oInfoDefault	= oInfo;
					}

					if (0 != oArrayEx.size())
					{
						pInfo = pMapIDs->Lock(oArrayEx[0]->m_nExObjID, exType);
						if (NULL != pInfo)
						{
							oInfo = *pInfo;
						}
					}

					if (CExFilesInfo::eftVideo == exType)
					{
						CVideoElement* pVideoElem		= new CVideoElement();
						
						pVideoElem->m_strVideoFileName	= oInfo.m_strFilePath			+ FILE_SEPARATOR_STR;
						pVideoElem->m_strImageFileName	= oInfoDefault.m_strFilePath	+ FILE_SEPARATOR_STR;

						pElem							= (IElement*)pVideoElem;
					}
					else if (CExFilesInfo::eftAudio == exType)
					{
						CAudioElement* pAudioElem		= new CAudioElement();
						
						pAudioElem->m_strAudioFileName	= oInfo.m_strFilePath + FILE_SEPARATOR_STR;

						pAudioElem->m_dClipStartTime	= oInfo.m_dStartTime;
						pAudioElem->m_dClipEndTime		= oInfo.m_dEndTime;

						pAudioElem->m_bLoop				= oInfo.m_bLoop;						

						if (NULL != pSlide)
						{
							pAudioElem->m_dStartTime	= pSlide->m_dStartTime;
							pAudioElem->m_dEndTime		= pSlide->m_dEndTime;

						}
						else
                        {
                            if (pLayout)
                                pLayout->m_arElements.push_back(pAudioElem);
                        }

						pElem = (IElement*)pAudioElem;
					}
					else 
					{
						CImageElement* pImageElem		= new CImageElement();
						pImageElem->m_strImageFileName	= oInfo.m_strFilePath + FILE_SEPARATOR_STR;

						pElem = (IElement*)pImageElem;
					}					
				}break;
			default:
				{
					// shape
					CShapeElement* pShape = new CShapeElement(NSBaseShape::ppt, eType);
					CPPTShape *ppt_shape = dynamic_cast<CPPTShape *>(pShape->m_oShape.m_pShape);
					if ( (ppt_shape) && (OOXMLShapes::sptCustom == ppt_shape->m_eType))
					{
						pShape->m_bShapePreset = true;
					}
					pElem = (IElement*)pShape;					
				}break;
			}
		}

		if (NULL == pElem)
			return;

		pElem->m_lID		= oArrayShape[0]->m_nID;
		pElem->m_lLayoutID	= lMasterID;

//---------внешние ссылки 
		{
			CExFilesInfo::ExFilesType exType		= CExFilesInfo::eftNone;
			CExFilesInfo			* pTextureInfo	= pMapIDs->Lock(0xFFFFFFFF, exType);

			if (NULL != pTextureInfo)
			{
				pElem->m_oBrush.TexturePath = pTextureInfo->m_strFilePath + FILE_SEPARATOR_STR;
			}

			std::vector<CRecordExObjRefAtom*> oArrayEx;
			GetRecordsByType(&oArrayEx, true, true);
			if (0 != oArrayEx.size())
			{
				CExFilesInfo* pInfo = pMapIDs->Lock(oArrayEx[0]->m_nExObjID, exType);

				if (CExFilesInfo::eftHyperlink == exType && pInfo)
				{
					pElem->m_sHyperlink = pInfo->m_strFilePath;
				}
			}
		}
		std::wstring strShapeText;
//------------------------------------------------------------------------------------------------		
		// placeholders
		std::vector<CRecordPlaceHolderAtom*> oArrayPlaceHolder;
		GetRecordsByType(&oArrayPlaceHolder, true, true);
		if (0 < oArrayPlaceHolder.size())
		{
			pElem->m_bLine					= false; //по умолчанию у них нет линий
			pElem->m_lPlaceholderID			= oArrayPlaceHolder[0]->m_nPosition;
			pElem->m_lPlaceholderType		= oArrayPlaceHolder[0]->m_nPlacementID;
			pElem->m_lPlaceholderSizePreset	= oArrayPlaceHolder[0]->m_nSize;

			if (pElementLayout) 
				pElementLayout->m_lPlaceholderSizePreset	= oArrayPlaceHolder[0]->m_nSize;

			CorrectPlaceholderType(pElem->m_lPlaceholderType);
		}

		std::vector<CRecordRoundTripHFPlaceholder12Atom*> oArrayHFPlaceholder;
		GetRecordsByType(&oArrayHFPlaceholder, true, true);
		if (0 < oArrayHFPlaceholder.size())
		{
			pElem->m_lPlaceholderType	= oArrayHFPlaceholder[0]->m_nPlacementID;//PT_MasterDate, PT_MasterSlideNumber, PT_MasterFooter, or PT_MasterHeader
			
			CorrectPlaceholderType(pElem->m_lPlaceholderType);
		}
		//meta placeholders
		std::vector<CRecordFooterMetaAtom*> oArrayFooterMeta;
		GetRecordsByType(&oArrayFooterMeta, true, true);
		if (0 < oArrayFooterMeta.size())
		{
			pElem->m_lPlaceholderType = PT_MasterFooter;

			DWORD posText = oArrayFooterMeta[0]->m_nPosition;
			if (posText < pTheme->m_oFootersHeaderString.size())
				strShapeText = pTheme->m_oFootersHeaderString[posText];
		}
		std::vector<CRecordSlideNumberMetaAtom*> oArraySlideNumberMeta;
		GetRecordsByType(&oArraySlideNumberMeta, true, true);
		if (0 < oArraySlideNumberMeta.size())
		{
			pElem->m_lPlaceholderType = PT_MasterSlideNumber;
			DWORD posText = oArraySlideNumberMeta[0]->m_nPosition;

			if (posText < pTheme->m_oFootersHeaderString.size())
				strShapeText = pTheme->m_oFootersHeaderString[posText];
		}
//------------- привязки ---------------------------------------------------------------------------------
		std::vector<CRecordClientAnchor*> oArrayAnchor;
		this->GetRecordsByType(&oArrayAnchor, true, true);

		bool bAnchor = false;

		if (0 != oArrayAnchor.size())
		{
			pElem->m_rcBoundsOriginal.left		= (LONG)oArrayAnchor[0]->m_oBounds.Left;
			pElem->m_rcBoundsOriginal.top		= (LONG)oArrayAnchor[0]->m_oBounds.Top;
			pElem->m_rcBoundsOriginal.right		= (LONG)oArrayAnchor[0]->m_oBounds.Right;
			pElem->m_rcBoundsOriginal.bottom	= (LONG)oArrayAnchor[0]->m_oBounds.Bottom;
			
			pElem->m_bBoundsEnabled	= true;
			bAnchor = true;
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
			
				pElem->m_bBoundsEnabled	= true;
				bAnchor = true;
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
					pElem->m_rcBoundsOriginal.right		= 0;
					pElem->m_rcBoundsOriginal.bottom	= 0;
				}
			}
		}

		double dScaleX = c_dMasterUnitsToMillimetreKoef;
		double dScaleY = c_dMasterUnitsToMillimetreKoef;

		pElem->NormalizeCoords(dScaleX, dScaleY);

		pElem->m_bFlipH = oArrayShape[0]->m_bFlipH;
		pElem->m_bFlipV = oArrayShape[0]->m_bFlipV;


		if (pElementLayout && bAnchor)
		{
			pElementLayout->m_rcBoundsOriginal	= pElem->m_rcBoundsOriginal;
			pElementLayout->m_rcBounds			= pElem->m_rcBounds;

			pElementLayout->m_bPlaceholderSet	= true;
			pElementLayout->m_bBoundsEnabled	= true;
		}
//--------- наличие текста --------------------------------------------------------------------------
		CShapeElement* pShapeElem = dynamic_cast<CShapeElement*>(pElem);
		if (NULL != pShapeElem)
		{
			CElementInfo oElementInfo;

			oElementInfo.m_lMasterPlaceholderType = pElem->m_lPlaceholderType;

			pShapeElem->m_oShape.m_dWidthLogic  = ShapeSizeVML;
			pShapeElem->m_oShape.m_dHeightLogic = ShapeSizeVML;

			// проверка на textheader present
			std::vector<CRecordTextHeaderAtom*> oArrayTextHeader;
			GetRecordsByType(&oArrayTextHeader, true, true);
			
			if (0 < oArrayTextHeader.size())
			{
				pShapeElem->m_oShape.m_oText.m_lTextType		= oArrayTextHeader[0]->m_nTextType;
				pShapeElem->m_oShape.m_oText.m_lTextMasterType	= oArrayTextHeader[0]->m_nTextType;
				oElementInfo.m_lMasterTextType					= oArrayTextHeader[0]->m_nTextType;
			}
			else
			{
				pShapeElem->m_oShape.m_oText.m_lTextType		= NSOfficePPT::NoPresent;
				pShapeElem->m_oShape.m_oText.m_lTextMasterType	= NSOfficePPT::NoPresent;
				oElementInfo.m_lMasterTextType					= NSOfficePPT::NoPresent;
			}

			// проверка на ссылку в персист
			std::vector<CRecordOutlineTextRefAtom*> oArrayTextRefs;
			GetRecordsByType(&oArrayTextRefs, true, true);
			
			if (0 < oArrayTextRefs.size())
			{
				oElementInfo.m_lPersistIndex = oArrayTextRefs[0]->m_nIndex;
			}

			// сам текст...
			std::vector<CRecordTextBytesAtom*> oArrayTextBytes;
			GetRecordsByType(&oArrayTextBytes, true, true);
			if (0 < oArrayTextBytes.size() && strShapeText.empty())
			{
				strShapeText = oArrayTextBytes[0]->m_strText;
			}
			
			std::vector<CRecordTextCharsAtom*> oArrayTextChars;
			GetRecordsByType(&oArrayTextChars, true, true);

			if (0 < oArrayTextChars.size() && strShapeText.empty())
			{
				strShapeText = oArrayTextChars[0]->m_strText;
			}

			if (pElem->m_lPlaceholderType == PT_MasterSlideNumber && strShapeText.length() > 5)
				pElem->m_lPlaceholderType = PT_MasterFooter; ///???? 1-(33).ppt

//------ shape properties ----------------------------------------------------------------------------------------
			for (int nIndexProp = 0; nIndexProp < oArrayOptions.size(); ++nIndexProp)
			{
				CPPTElement oElement;
				oElement.SetUpProperties(pElem, pTheme, pSlideWrapper, pSlide, &oArrayOptions[nIndexProp]->m_oProperties);
			}

			std::vector<CRecordStyleTextPropAtom*> oArrayTextStyle;
			this->GetRecordsByType(&oArrayTextStyle, true, true);
			if (0 != oArrayTextStyle.size())
			{
				oElementInfo.m_pStream				= m_pStream;
				oElementInfo.m_lOffsetTextStyle		= oArrayTextStyle[0]->m_lOffsetInStream;
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
			double dAngle = pShapeElem->m_dRotate;
			if (0 <= dAngle)
			{
				int lCount = (int)dAngle / 360;
				dAngle -= (lCount * 360.0);
			}
			else
			{
				int lCount = (int)dAngle / 360;
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
			
			std::vector<CRecordMasterTextPropAtom*> oArrayMasterTextProp;
			GetRecordsByType(&oArrayMasterTextProp, true);

			CRecordMasterTextPropAtom* master_level = NULL;
			if (!oArrayMasterTextProp.empty())
				master_level = oArrayMasterTextProp[0];

			pSlideWrapper->m_mapElements.insert(std::pair<LONG, CElementInfo>(pShapeElem->m_lID, oElementInfo));
			
			SetUpTextStyle(strShapeText, pTheme, pLayout, pElem, pThemeWrapper, pSlideWrapper, pSlide, master_level);
		}
		else
		{//image, audio, video ....
			for (int nIndexProp = 0; nIndexProp < oArrayOptions.size(); ++nIndexProp)
			{
				CPPTElement oElement;
				oElement.SetUpProperties(pElem, pTheme, pSlideWrapper, pSlide, &oArrayOptions[nIndexProp]->m_oProperties);
			}

			pElem->m_lLayoutID = lMasterID;
		}
//----------------------------------------------------------------------------------------------------
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

		pElem->m_bIsBackground	=	(true == oArrayShape[0]->m_bBackground);
		pElem->m_bHaveAnchor	=	(true == oArrayShape[0]->m_bHaveAnchor);

		*ppElement = pElem;
	}

	void RecalcGroupShapeAnchor(CDoubleRect& rcChildAnchor)
	{

		if ((NULL == m_pGroupBounds) || (NULL == m_pGroupClientAnchor))
		{
			rcChildAnchor.left		= 0;///= dScaleX;
			rcChildAnchor.top		= 0;//= dScaleY;
			rcChildAnchor.bottom	= 0;//= dScaleY;
			rcChildAnchor.right 	= 0;//= dScaleX;
			return;
		}

		// здесь переводим координаты, чтобы они не зависили от группы
		long lWidthClient	= m_pGroupClientAnchor->right	- m_pGroupClientAnchor->left;
		long lHeightClient	= m_pGroupClientAnchor->bottom	- m_pGroupClientAnchor->top;
		
		long lWidthGroup	= m_pGroupBounds->right			- m_pGroupBounds->left;
		long lHeightGroup	= m_pGroupBounds->bottom		- m_pGroupBounds->top;

		double dScaleX = (double)(lWidthClient) / (lWidthGroup);
		double dScaleY = (double)(lHeightClient) / (lHeightGroup);
		
		rcChildAnchor.left		= m_pGroupClientAnchor->left	+ (long)(dScaleX * (rcChildAnchor.left	- m_pGroupBounds->left));
		rcChildAnchor.right		= m_pGroupClientAnchor->left	+ (long)(dScaleX * (rcChildAnchor.right - m_pGroupBounds->left));

		rcChildAnchor.top		= m_pGroupClientAnchor->top		+ (long)(dScaleY * (rcChildAnchor.top	- m_pGroupBounds->top));
		rcChildAnchor.bottom	= m_pGroupClientAnchor->top		+ (long)(dScaleY * (rcChildAnchor.bottom - m_pGroupBounds->top));
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

	void ApplyThemeStyle(IElement* pElem, CTheme* pTheme, CRecordMasterTextPropAtom* master_levels)
	{
		CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem);
		if (NULL == pShape)
			return;
		
		CTextAttributesEx* pText = &(pShape->m_oShape.m_oText);

		
		if (master_levels)
		{
			for (int i = 0; i < pText->m_arParagraphs.size(); i++)
			{
				if (i >= master_levels->m_arrProps.size()) break;
				
				pText->m_arParagraphs[i].m_lTextLevel = master_levels->m_arrProps[i].lIndentLevel;
				pText->m_arParagraphs[i].m_oPFRun.leftMargin.reset();
				pText->m_arParagraphs[i].m_oPFRun.indent.reset();
			}
		}

		pText->ApplyThemeStyle(pTheme);

	}
	void SetUpTextStyle(std::wstring& strText, CTheme* pTheme, CLayout* pLayout, IElement* pElem, CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide, CRecordMasterTextPropAtom* master_levels)
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
			int ph_type		= pShape->m_lPlaceholderType;
			int ph_pos		= pShape->m_lPlaceholderID;

			pTextSettings->m_lPlaceholderType = pShape->m_lPlaceholderType;

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
		std::vector<CTextFullSettings>* pArrayPlaceHolders	= &pSlideWrapper->m_arTextPlaceHolders;
		int lCountPersistObjects							= pArrayPlaceHolders->size();
		int lPersistIndex									= oElemInfo.m_lPersistIndex;

		if ((lPersistIndex >= 0) && (lPersistIndex < lCountPersistObjects))
		{
			CTextFullSettings* pSettings = &pArrayPlaceHolders->at(lPersistIndex);

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

		if (NULL != oElemInfo.m_pStream && -1 != oElemInfo.m_lOffsetTextStyle)
		{
			// теперь нужно загрузить стили текста из стрима.
			LONG lPosition = 0; StreamUtils::StreamPosition(lPosition, oElemInfo.m_pStream);

			StreamUtils::StreamSeek(oElemInfo.m_lOffsetTextStyle - 8, oElemInfo.m_pStream);

			SRecordHeader oHeader;
			oHeader.ReadFromStream(oElemInfo.m_pStream) ;	

			if (RECORD_TYPE_STYLE_TEXTPROP_ATOM == oHeader.RecType)
			{			
				CRecordStyleTextPropAtom* pStyle = new CRecordStyleTextPropAtom();
				pStyle->m_lCount = strText.length();

				pStyle->ReadFromStream(oHeader, oElemInfo.m_pStream);

				NSPresentationEditor::ConvertPPTTextToEditorStructure(pStyle->m_arrPFs, pStyle->m_arrCFs, strText, pShape->m_oShape.m_oText);

				bIsOwnPresentSettings = (0 < pStyle->m_lCount);

				RELEASEOBJECT(pStyle);
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
						pTextSettings->m_lStyleThemeIndex = 1;

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
						pTextSettings->m_lStyleThemeIndex = 1;

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
						pTextSettings->m_lStyleThemeIndex = 2;

						if ((NSOfficePPT::_Body != eTypeMaster) || !pLayout) 
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
						pTextSettings->m_lStyleThemeIndex = 3;

						if ((NSOfficePPT::Other != eTypeMaster) || !pLayout) 
						{
							if (0 <= nTextMasterType && nTextMasterType < 9)
							{
								if (pThemeWrapper->m_pStyles[nTextMasterType].is_init())
								{
									pTextSettings->m_oStyles = pThemeWrapper->m_pStyles[nTextMasterType].get();
								}
							}
						}
						break;
					}
				}
			}
			else
			{
				//pTextSettings->m_lTextType = 0;

				//if (NSOfficePPT::Other != eTypeMaster)
				//{
				//	if (0 <= nTextMasterType && nTextMasterType < 9)
				//	{
				//		if (pThemeWrapper->m_pStyles[nTextMasterType].is_init())
				//			pTextSettings->m_oStyles = pThemeWrapper->m_pStyles[nTextMasterType].get();
				//	}
				//}
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
					pTextSettings->m_oLayoutStyles		= pElementLayoutPH->m_oShape.m_oText.m_oStyles;
					pTextSettings->m_lTextType			= pElementLayoutPH->m_oShape.m_oText.m_lTextType;
					pTextSettings->m_lStyleThemeIndex	= pElementLayoutPH->m_oShape.m_oText.m_lStyleThemeIndex;
				}
				else
				{	
					switch (oElemInfo.m_lMasterPlaceholderType)
					{
					case NSOfficePPT::Title:
					case NSOfficePPT::MasterTitle:
					case NSOfficePPT::VerticalTextTitle:
						{
							pTextSettings->m_lStyleThemeIndex = 1;
							break;
						}
					case NSOfficePPT::CenteredTitle:
					case NSOfficePPT::MasterCenteredTitle:
						{
							pTextSettings->m_lStyleThemeIndex = 1;
							break;
						}
					case NSOfficePPT::Body:
					case NSOfficePPT::MasterBody:
					case NSOfficePPT::NotesBody:
					case NSOfficePPT::MasterNotesBody:
					case NSOfficePPT::VerticalTextBody:
						{
							pTextSettings->m_lStyleThemeIndex = 2;
							break;
						}
					default:
						{
							pTextSettings->m_lStyleThemeIndex = 3;
							break;
						}
					}
				}
			}
			else
			{
				pTextSettings->m_lStyleThemeIndex = 0; //???
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
			if (eTypeOwn != NSOfficePPT::NoPresent && eTypeOwn != NSOfficePPT::Other && eTypeOwn != eTypePersist && eTypeOwn != eTypeMaster)
			{
				int nIndexType = (int)eTypeOwn;
				
				if (0 <= nIndexType && nIndexType < 9 && pLayout)
				{
					if (pThemeWrapper->m_pStyles[nIndexType].IsInit())
					{
						pTextSettings->m_oStyles.ApplyAfter(pThemeWrapper->m_pStyles[nIndexType].get());
					}
				}
			}
		}

		if ((_T("") != strText) && 0 == pTextSettings->m_arParagraphs.size())
		{
			// значит никаких своих настроек нету. Значит просто пустые свои настройки
			std::vector<CTextPFRun_ppt> oArrayPF;
			
			CTextPFRun_ppt elm;
			
			elm.m_lCount = strText.length();
			elm.m_lLevel = 0;
			
			oArrayPF.push_back(elm);

			std::vector<CTextCFRun_ppt> oArrayCF;
			
			CTextCFRun_ppt elm1;
			elm1.m_lCount = elm.m_lCount;
			
			oArrayCF.push_back(elm1);
			
			NSPresentationEditor::ConvertPPTTextToEditorStructure(oArrayPF, oArrayCF, strText, *pTextSettings);
		}
	
		if (NULL != oElemInfo.m_pStream && -1 != oElemInfo.m_lOffsetTextProp)
		{
			//языковые настройки текта
			LONG lPosition = 0; StreamUtils::StreamPosition(lPosition, oElemInfo.m_pStream);

			StreamUtils::StreamSeek(oElemInfo.m_lOffsetTextProp - 8, oElemInfo.m_pStream);

			SRecordHeader oHeader;
			oHeader.ReadFromStream(oElemInfo.m_pStream) ;	

			if (RECORD_TYPE_TEXTSPECINFO_ATOM == oHeader.RecType)
			{			
				CRecordTextSpecInfoAtom* pSpecInfo = new CRecordTextSpecInfoAtom();
				pSpecInfo->m_lCount = -1;

				pSpecInfo->ReadFromStream(oHeader, oElemInfo.m_pStream);
				pSpecInfo->ApplyProperties(&(pShape->m_oShape.m_oText));

				RELEASEOBJECT(pSpecInfo);
			}
			StreamUtils::StreamSeek(lPosition, oElemInfo.m_pStream);
		}
		pShape->m_oShape.m_oText.RecalcParagraphsPPT();
		
		ApplyThemeStyle(pElem, pTheme, master_levels);

		if (pShape->m_oActions.m_bPresent)
		{
			//todooo разобраться нужно ли менять цвет на гиперлинк - 1-(34).ppt
			NSPresentationEditor::CColor oColor;
			if ((NULL != pSlide) && !pSlide->m_bUseLayoutColorScheme)			oColor = pSlide->GetColor(11);
			else if ((NULL != pLayout) && (!pLayout->m_bUseThemeColorScheme))	oColor = pLayout->GetColor(11);
			else if (NULL != pTheme)											oColor = pTheme->GetColor(11);
			oColor.m_lSchemeIndex = 11;

			ApplyHyperlink(pShape, oColor);
		}

		CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(pShape->m_oShape.m_pShape);

		if (NULL != pPPTShape)		// проверка на wordart
		{
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
					pShape->m_oShape.m_oText.m_oAttributes.m_oTextBrush = pShape->m_oBrush;

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
		std::vector<CTextRange>* pRanges					= &pShape->m_oActions.m_arRanges;
		CTextAttributesEx* pTextAttributes					= &pShape->m_oShape.m_oText;

		int lCountHyper	= pRanges->size();

		if (0 == lCountHyper)
			return;

		size_t nCountPars = pTextAttributes->m_arParagraphs.size();
		for (int nIndexRange = 0; nIndexRange < lCountHyper; ++nIndexRange)
		{
			int lStart = (*pRanges)[nIndexRange].m_lStart;
			int lEnd	= (*pRanges)[nIndexRange].m_lEnd;

			int lCurrentStart = 0;
			for (size_t nIndexPar = 0; nIndexPar < nCountPars; ++nIndexPar)
			{
				CParagraph* pParagraph = &pTextAttributes->m_arParagraphs[nIndexPar];

				for (size_t nIndexSpan = 0; nIndexSpan < pParagraph->m_arSpans.size(); ++nIndexSpan)
				{
					int lCurrentEnd = lCurrentStart + pParagraph->m_arSpans[nIndexSpan].m_strText.length() - 1;

					if (lCurrentStart > lEnd || lCurrentEnd < lStart)
					{
						lCurrentStart = lCurrentEnd + 1;
						continue;
					}

                    int lStart_	= (std::max)(lStart, lCurrentStart);
                    int lEnd_	= (std::min)(lEnd, lCurrentEnd);

					CSpan oRunProp = pParagraph->m_arSpans[nIndexSpan];

					std::wstring strText = pParagraph->m_arSpans[nIndexSpan].m_strText;
					if (lStart_ > lCurrentStart)
					{
						pParagraph->m_arSpans.insert(pParagraph->m_arSpans.begin()  + nIndexSpan, oRunProp);
						pParagraph->m_arSpans[nIndexSpan].m_strText = strText.substr(0, lStart_ - lCurrentStart);

						++nIndexSpan;
					}
					pParagraph->m_arSpans[nIndexSpan].m_oRun.Color = oColor;
                    pParagraph->m_arSpans[nIndexSpan].m_oRun.FontUnderline = (bool)true;
					pParagraph->m_arSpans[nIndexSpan].m_strText = strText.substr(lStart_ - lCurrentStart, lEnd_ - lStart_ + 1);
					if (lEnd_ < lCurrentEnd)
					{
						pParagraph->m_arSpans.insert(pParagraph->m_arSpans.begin() + nIndexSpan + 1, oRunProp);
						++nIndexSpan;

						pParagraph->m_arSpans[nIndexSpan].m_strText = strText.substr(lEnd_ - lCurrentStart + 1, lCurrentEnd - lEnd_);
					}

					lCurrentStart = lCurrentEnd + 1;
				}
			}			
		}
	}	

};
