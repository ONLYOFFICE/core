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

#include "ShapeContainer.h"

#include "../../Drawing/Document.h"
#include "../../../Common/Vml/PPTShape/ElementSettings.h"

#include "../../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../../OOXML/Base/Base.h"

#include "../../../../OfficeUtils/src/OfficeUtils.h"
#include "../../Enums/_includer.h"
#include "../RecordsIncluder.h"

#define FIXED_POINT_unsigned(val) (double)((WORD)(val >> 16) + ((WORD)(val) / 65536.0))

ULONG xmlName = 1;

using namespace ODRAW;
using namespace PPT;

bool CPPTElement::ChangeBlack2ColorImage(std::wstring image_path, int rgbColor1, int rgbColor2)
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
CColor CPPTElement::CorrectSysColor(int nColorCode, CElementPtr pElement, CTheme* pTheme)
{
    if (!pElement) return CColor();

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

    if (color.m_lSchemeIndex != -1 &&
            (int)pTheme->m_arColorScheme.size() > color.m_lSchemeIndex)
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
void CPPTElement::SetUpProperties(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pWrapper, CSlide* pSlide, CProperties* pProperties, bool reset_default)
{
    size_t lCount = pProperties->m_lCount;
    switch (pElement->m_etType)
    {
    case PPT::etVideo:
    {
        if (reset_default)
        {
            pElement->m_bLine = false;
        }
        for (size_t i = 0; i < lCount; ++i)
        {
            SetUpPropertyVideo(pElement, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
        }
        break;
    }
    case PPT::etPicture:
    {
        if (reset_default)
        {
            pElement->m_oBrush.Type = c_BrushTypeTexture;
            pElement->m_bLine = false;
        }
        for (size_t i = 0; i < lCount; ++i)
        {
            SetUpPropertyImage(pElement, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
        }
        break;
    }
    case PPT::etAudio:
    {
        if (reset_default)
        {
            pElement->m_bLine = false;
        }
        for (size_t i = 0; i < lCount; ++i)
        {
            SetUpPropertyAudio(pElement, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
        }
        break;
    }
    case PPT::etGroup:
    {
        if (reset_default)
        {
            pElement->m_bLine = false;
            pElement->m_bIsFilled = false;
        }
        for (size_t i = 0; i < lCount; ++i)
        {
            SetUpProperty(pElement, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
        }
    }break;
    case PPT::etShape:
    {
        CShapeElement* pShapeElem = dynamic_cast<CShapeElement*>(pElement.get());
        CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(pShapeElem->m_pShape->getBaseShape().get());

        if (NULL != pPPTShape)
        {
            pPPTShape->m_oCustomVML.SetAdjusts(&pPPTShape->m_arAdjustments);
        }

        for (size_t i = 0; i < lCount; ++i)
        {
            SetUpPropertyShape(pElement, pTheme, pWrapper, pSlide, &pProperties->m_arProperties[i]);
        }

        if (false == pElement->m_bIsFilled)
        {
            pElement->m_oBrush.Type = c_BrushTypeNoFill;
        }
        else if (pElement->m_oBrush.Type == c_BrushTypeNotSet &&
                 (pElement->m_lPlaceholderType == 0 && pElement->m_lPlaceholderID < 0 ))
        {
            pElement->m_oBrush.Type = c_BrushTypeSolid;
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

void CPPTElement::SetUpProperty(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
{
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
    case ePropertyId_rotation:
    {
        pElement->m_dRotate = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ePropertyId_fFlipH:
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
        _UINT32 dwType = pProperty->m_lValue;
        switch(dwType)
        {
        case ODRAW::fillPattern:
        {
            pElement->m_oBrush.Type = c_BrushTypePattern;
            //texture + change black to color2, white to color1
        }break;
        case ODRAW::fillTexture :
        case ODRAW::fillPicture :
        {
            pElement->m_oBrush.Type			= c_BrushTypeTexture;
            pElement->m_oBrush.TextureMode	= (ODRAW::fillPicture == dwType) ? c_BrushTextureModeStretch : c_BrushTextureModeTile;
        }break;
        case ODRAW::fillShadeCenter://1 color
        case ODRAW::fillShadeShape:
        {
            pElement->m_oBrush.Type = c_BrushTypeCenter;
        }break;//
        case ODRAW::fillShadeTitle://2 colors and more
        case ODRAW::fillShade :
        case ODRAW::fillShadeScale:
        {
            pElement->m_oBrush.Type = c_BrushTypePathGradient1;
        }break;
        case ODRAW::fillBackground:
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
    }break;
    case fillColor:
    {
        SColorAtom oAtom;
        oAtom.FromValue(pProperty->m_lValue);

        if(oAtom.bSysIndex)
            pElement->m_oBrush.Color1 = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
        else
            oAtom.ToColor(&pElement->m_oBrush.Color1);

        if (pElement->m_oBrush.Type == c_BrushTypeNotSet )
            pElement->m_oBrush.Type = c_BrushTypeSolid;

    }break;
    case fillBackColor:
    {
        SColorAtom oAtom;
        oAtom.FromValue(pProperty->m_lValue);

        if(oAtom.bSysIndex)
            pElement->m_oBrush.Color2 = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
        else
            oAtom.ToColor(&pElement->m_oBrush.Color2);

        if (pElement->m_bIsBackground && pElement->m_oBrush.Type == c_BrushTypeNotSet )
        {
            pElement->m_oBrush.Type = c_BrushTypeSolid;
        }
    }break;
    case fillOpacity:
    {
        pElement->m_oBrush.Alpha1 = (BYTE)(std::min)(255, (int)CDirectory::NormFixedPoint(pProperty->m_lValue, 255));
    }break;
    case fillBackOpacity:
    {
        pElement->m_oBrush.Alpha2 = (BYTE)(std::min)(255, (int)CDirectory::NormFixedPoint(pProperty->m_lValue, 255));
    }break;
    case fillAngle:
    {
        pElement->m_oBrush.LinearAngle = FixedPointToDouble(pProperty->m_lValue);
    }break;
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
    }break;
    case fillShadeType:
    {
        bool bShadeNone		= GETBIT(pProperty->m_lValue, 31);
        bool bShadeGamma	= GETBIT(pProperty->m_lValue, 30);
        bool bShadeSigma	= GETBIT(pProperty->m_lValue, 29);
        bool bShadeBand		= GETBIT(pProperty->m_lValue, 28);
        bool bShadeOneColor	= GETBIT(pProperty->m_lValue, 27);

    }break;
    case fillFocus://relative position of the last color in the shaded fill
    {
    }break;
    case fillShadePreset:
    {//value (int) from 0x00000088 through 0x0000009F or complex
    }break;
    case fillShadeColors:
    {
        unsigned short nElems = pProperty->m_lValue/8;
        _INT32* pCompl = (_INT32*)pProperty->m_pOptions;

        while(nElems--)
        {
            CColor color;
            SColorAtom oAtom;
            oAtom.FromValue(*pCompl);	pCompl++;
            oAtom.ToColor(&color);

            _UINT32 dwPosition = *pCompl; pCompl++;
            pElement->m_oBrush.ColorsPosition.push_back(std::pair<CColor, double>(color, 100. * FIXED_POINT_unsigned(dwPosition)));
        }
    }break;
    case fillStyleBooleanProperties:
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
            pElement->m_bIsFilled = bFilled;

        break;
    }
    case ODRAW::geometryBooleanProperties:
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
            pElement->m_bIsFilled = bFillOk;

        break;
    }
        // line --------------------------------------------------------
    case lineStyleBooleanProperties: //Line Style Boolean Properties
    {
        bool bUsefLineOpaqueBackColor = GETBIT(pProperty->m_lValue, 25);
        bool bUsefInsetPen			= GETBIT(pProperty->m_lValue, 22);
        bool bUsefInsetPenOK		= GETBIT(pProperty->m_lValue, 21);
        bool bUsefArrowheadsOK		= GETBIT(pProperty->m_lValue, 20);
        bool bUsefLine				= GETBIT(pProperty->m_lValue, 19);
        bool bUsefHitTestLine		= GETBIT(pProperty->m_lValue, 18);
        bool bUsefLineFillShape		= GETBIT(pProperty->m_lValue, 17);
        bool bUsefNoLineDrawDash	= GETBIT(pProperty->m_lValue, 16);

        bool bLineOpaqueBackColor	= GETBIT(pProperty->m_lValue, 9);
        bool bInsetPen				= GETBIT(pProperty->m_lValue, 6);
        bool bInsetPenOK			= GETBIT(pProperty->m_lValue, 5);
        bool bArrowheadsOK			= GETBIT(pProperty->m_lValue, 4);
        bool bLine					= GETBIT(pProperty->m_lValue, 3);
        bool bHitTestLine			= GETBIT(pProperty->m_lValue, 2);
        bool bLineFillShape			= GETBIT(pProperty->m_lValue, 1);
        bool bNoLineDrawDash		= GETBIT(pProperty->m_lValue, 0);

        if (bUsefLine)
            pElement->m_bLine = bLine;
    }break;
    case lineDashStyle://from Complex
    {
        pElement->m_bLine = true;
    }break;
    case lineColor:
    {
        pElement->m_bLine = true;

        SColorAtom oAtom;
        oAtom.FromValue(pProperty->m_lValue);

        if (oAtom.bSysIndex)
            pElement->m_oPen.Color = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
        else
            oAtom.ToColor(&pElement->m_oPen.Color);
    }break;
    case lineOpacity:
    {
        pElement->m_oPen.Alpha = (BYTE)(std::min)(255, (int)CDirectory::NormFixedPoint(pProperty->m_lValue, 255));
    }break;
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
        pElement->m_oPen.Size	= pProperty->m_lValue;
        pElement->m_bLine		= true;
    }break;
    case lineStyle:
    {
        pElement->m_bLine			= true;
        pElement->m_oPen.LineStyle	= pProperty->m_lValue;
    }break;
    case lineDashing:
    {
        pElement->m_bLine			= true;
        pElement->m_oPen.DashStyle	= pProperty->m_lValue;
    }break;
    case lineJoinStyle:
    {
        pElement->m_oPen.LineJoin = pProperty->m_lValue;
    }break;
    case lineStartArrowLength:
    {
        pElement->m_oPen.LineStartLength = pProperty->m_lValue;
    }break;
    case lineEndArrowLength:
    {
        pElement->m_oPen.LineEndLength = pProperty->m_lValue;
    }break;
    case lineStartArrowWidth:
    {
        pElement->m_oPen.LineStartWidth = pProperty->m_lValue;
    }break;
    case lineEndArrowWidth:
    {
        pElement->m_oPen.LineEndWidth = pProperty->m_lValue;
    }break;
    case lineStartArrowhead:
    {
        pElement->m_oPen.LineStartCap = pProperty->m_lValue;
    }break;
    case lineEndArrowhead:
    {
        pElement->m_oPen.LineEndCap = pProperty->m_lValue;
    }break;
    case shadowType:
    {
        pElement->m_oShadow.Type = pProperty->m_lValue;
    }break;
    case shadowOriginX://in emu, relative from center shape
    {
        pElement->m_oShadow.OriginX = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case shadowOriginY:
    {
        pElement->m_oShadow.OriginY = FixedPointToDouble(pProperty->m_lValue);
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
    {//signed
        pElement->m_oShadow.DistanceX = (_INT32)pProperty->m_lValue;
    }break;
    case shadowOffsetY:
    {//signed
        pElement->m_oShadow.DistanceY = (_INT32)pProperty->m_lValue;
    }break;
    case shadowScaleXToX:
    {
        pElement->m_oShadow.ScaleXToX = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case shadowScaleYToX:
    {
        pElement->m_oShadow.ScaleYToX = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case shadowScaleXToY:
    {
        pElement->m_oShadow.ScaleXToY = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case shadowScaleYToY:
    {
        pElement->m_oShadow.ScaleYToY = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case shadowPerspectiveX:
    {
        pElement->m_oShadow.PerspectiveX = pProperty->m_lValue;
    }break;
    case shadowPerspectiveY:
    {
        pElement->m_oShadow.PerspectiveY = pProperty->m_lValue;
    }break;
    case shadowStyleBooleanProperties:
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
    case shapeBoolean:
    {
        bool fUsefPolicyLabel	= GETBIT(pProperty->m_lValue, 25);
        bool fUsefPolicyBarcode	= GETBIT(pProperty->m_lValue, 24);
        bool fUsefFlipHOverride	= GETBIT(pProperty->m_lValue, 23);
        bool fUsefFlipVOverride	= GETBIT(pProperty->m_lValue, 22);
        bool fUsefOleIcon		= GETBIT(pProperty->m_lValue, 21);
        bool fUsefPreferRelativeResize	= GETBIT(pProperty->m_lValue, 20);
        bool fUsefLockShapeType		= GETBIT(pProperty->m_lValue, 19);
        bool fUsefInitiator		= GETBIT(pProperty->m_lValue, 18);
        bool fUsefBackground	= GETBIT(pProperty->m_lValue, 16);

        bool fPolicyLabel		= fUsefPolicyLabel		? GETBIT(pProperty->m_lValue, 9) : false;
        bool fPolicyBarcode		= fUsefPolicyBarcode	? GETBIT(pProperty->m_lValue, 8) : false;
        bool fFlipHOverride		= fUsefFlipHOverride	? GETBIT(pProperty->m_lValue, 7) : false;
        bool fFlipVOverride		= fUsefFlipVOverride	? GETBIT(pProperty->m_lValue, 6) : false;
        bool fOleIcon			= fUsefOleIcon			? GETBIT(pProperty->m_lValue, 5) : false;
        bool fPreferRelativeResize = fUsefPreferRelativeResize ? GETBIT(pProperty->m_lValue, 4) : false;
        bool fLockShapeType		= fUsefLockShapeType	? GETBIT(pProperty->m_lValue, 3) : false;
        bool fInitiator			= fUsefInitiator		? GETBIT(pProperty->m_lValue, 2) : false;
        bool fBackground		= fUsefBackground		? GETBIT(pProperty->m_lValue, 0) : false;
    }break;
    case groupShapeBooleanProperties:
    {
        bool fUsefLayoutInCell		= GETBIT(pProperty->m_lValue, 31);
        bool fUsefIsBullet			= GETBIT(pProperty->m_lValue, 30);
        bool fUsefStandardHR		= GETBIT(pProperty->m_lValue, 29);
        bool fUsefNoshadeHR			= GETBIT(pProperty->m_lValue, 28);
        bool fUsefHorizRule			= GETBIT(pProperty->m_lValue, 27);
        bool fUsefUserDrawn			= GETBIT(pProperty->m_lValue, 26);
        bool fUsefAllowOverlap		= GETBIT(pProperty->m_lValue, 25);
        bool fUsefReallyHidden		= GETBIT(pProperty->m_lValue, 24);
        bool fUsefScriptAnchor		= GETBIT(pProperty->m_lValue, 23);
        bool fUsefEditedWrap		= GETBIT(pProperty->m_lValue, 22);
        bool fUsefBehindDocument	= GETBIT(pProperty->m_lValue, 21);
        bool fUsefOnDblClickNotify	= GETBIT(pProperty->m_lValue, 20);
        bool fUsefIsButton			= GETBIT(pProperty->m_lValue, 19);
        bool fUsefOneD				= GETBIT(pProperty->m_lValue, 18);
        bool fUsefHidden			= GETBIT(pProperty->m_lValue, 17);
        bool fUsefPrint				= GETBIT(pProperty->m_lValue, 16);

        bool fLayoutInCell		= fUsefLayoutInCell	? GETBIT(pProperty->m_lValue, 15)	: true;
        bool fIsBullet			= fUsefIsBullet		? GETBIT(pProperty->m_lValue, 14)	: false;
        bool fStandardHR		= fUsefStandardHR	? GETBIT(pProperty->m_lValue, 13)	: false;
        bool fNoshadeHR			= fUsefNoshadeHR	? GETBIT(pProperty->m_lValue, 12)	: false;
        bool fHorizRule			= fUsefHorizRule	? GETBIT(pProperty->m_lValue, 11)	: false;
        bool fUserDrawn			= fUsefUserDrawn	? GETBIT(pProperty->m_lValue, 10)	: false;
        bool fAllowOverlap		= fUsefAllowOverlap	? GETBIT(pProperty->m_lValue, 9)	: true;
        bool fReallyHidden 		= fUsefReallyHidden		? GETBIT(pProperty->m_lValue, 8) : false;
        bool fScriptAnchor		= fUsefScriptAnchor		? GETBIT(pProperty->m_lValue, 7) : false;
        bool fEditedWrap		= fUsefEditedWrap		? GETBIT(pProperty->m_lValue, 6) : false;
        bool fBehindDocument	= fUsefBehindDocument	? GETBIT(pProperty->m_lValue, 5) : false;
        bool fOnDblClickNotify	= fUsefOnDblClickNotify ? GETBIT(pProperty->m_lValue, 4) : false;
        bool fIsButton			= fUsefIsButton		? GETBIT(pProperty->m_lValue, 3)	: false;
        bool fOneD				= fUsefOneD			? GETBIT(pProperty->m_lValue, 2)	: false;
        bool fHidden			= fUsefHidden		? GETBIT(pProperty->m_lValue, 1)	: false;
        bool fPrint				= fUsefPrint		? GETBIT(pProperty->m_lValue, 0)	: true;

        pElement->m_bHidden = fHidden || fIsBullet;
        //presentation_ticio_20100610.ppt
    }break;
    case tableProperties:
    case tableRowProperties:
    {
        pElement->m_etType = etTable;
        break;
    }
    default:
        break;
    }
}

void CPPTElement::SetUpPropertyVideo(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
{
    SetUpPropertyImage(pElement, pTheme, pInfo, pSlide, pProperty);
}
void CPPTElement::SetUpPropertyAudio(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
{
    SetUpPropertyImage(pElement, pTheme, pInfo, pSlide, pProperty);
}
void CPPTElement::SetUpPropertyImage(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
{
    SetUpProperty(pElement, pTheme, pInfo, pSlide, pProperty);

    CImageElement* image_element = dynamic_cast<CImageElement*>(pElement.get());

    switch(pProperty->m_ePID)
    {
    case pib:
    {
        int dwOffset = pInfo->GetIndexPicture(pProperty->m_lValue);
        if (dwOffset >=0)
        {
            image_element->m_strImageFileName	+=  pInfo->GetFileNamePicture(dwOffset);
            image_element->m_bImagePresent		= true;
        }
    }break;
    case pictureId://OLE identifier of the picture.
    {
        image_element->m_bOLE	= true;
    }break;
    case pibName:
    {
        image_element->m_sImageName = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue /2-1);
        // TextMining05.ppt, слайд 20  - некорректное имя ( - todooo потом подчистить его
    }break;
    case cropFromTop:
    {
        image_element->m_lcropFromTop = (_INT32)pProperty->m_lValue;
        image_element->m_bCropEnabled = true;
    }break;
    case cropFromBottom:
    {
        image_element->m_lcropFromBottom = (_INT32)pProperty->m_lValue;
        image_element->m_bCropEnabled = true;
    }break;
    case cropFromLeft:
    {
        image_element->m_lcropFromLeft = (_INT32)pProperty->m_lValue;
        image_element->m_bCropEnabled = true;
    }break;
    case cropFromRight:
    {
        image_element->m_lcropFromRight = (_INT32)pProperty->m_lValue;
        image_element->m_bCropEnabled = true;
    }break;
    case pibFlags:
    {
    }break;
    case pictureContrast:
        image_element->m_lpictureContrast = (_INT32)pProperty->m_lValue;
        break;
    case pictureBrightness:
        image_element->m_lpictureBrightness = (_INT32)pProperty->m_lValue;
        break;
    case fillBackColor:
        // TODO to fix 53541
        break;
    default:
        break;
    }
}
void CPPTElement::SetUpPropertyShape(CElementPtr pElement, CTheme* pTheme, CSlideInfo* pInfo, CSlide* pSlide, CProperty* pProperty)
{
    SetUpProperty(pElement, pTheme, pInfo, pSlide, pProperty);

    CShapeElement* shape_element = dynamic_cast<CShapeElement*>(pElement.get());

    CShapePtr pParentShape = shape_element->m_pShape;
    if (NULL == pParentShape)
        return;

    CPPTShape* pShape = dynamic_cast<CPPTShape*>(pParentShape->getBaseShape().get());

    if (NULL == pShape)
        return;

    switch (pProperty->m_ePID)
    {
    case ODRAW::metroBlob:
    {
        NSFile::CFileBinary file;

        std::wstring temp = NSDirectory::GetTempPath();

        std::wstring tempFileName = temp + FILE_SEPARATOR_STR + L"tempMetroBlob.zip";

        if (file.CreateFileW(tempFileName))
        {
            file.WriteFile(pProperty->m_pOptions, pProperty->m_lValue);
            file.CloseFile();
        }
        COfficeUtils officeUtils(NULL);

        BYTE *utf8Data = NULL;
        ULONG utf8DataSize = 0;
        if (S_OK != officeUtils.LoadFileFromArchive(tempFileName, L"drs/shapexml.xml", &utf8Data, utf8DataSize))
        {
            officeUtils.LoadFileFromArchive(tempFileName, L"drs/diagrams/drawing1.xml", &utf8Data, utf8DataSize);
        }

        if (utf8Data && utf8DataSize > 0)
        {
            pParentShape->m_strXmlString = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(utf8Data, utf8DataSize);
            //            std::string filename = std::to_string(nRTCounter++) + "_shape.xml";
            //            std::ofstream file("data/" + filename, std::ios::out);
            //            file.write((char*)utf8Data, utf8DataSize);
            //            file.close();

            delete []utf8Data;
        }
        NSFile::CFileBinary::Remove(tempFileName);
    }break;
    case ODRAW::geoRight:
    {
        if (0 < pProperty->m_lValue)
            pParentShape->m_dWidthLogic = (double)(pProperty->m_lValue);
    }break;
    case ODRAW::geoBottom:
    {
        if (0 < pProperty->m_lValue)
            pParentShape->m_dHeightLogic = (double)(pProperty->m_lValue);
    }break;
    case ODRAW::shapePath:
    {
        pShape->m_oCustomVML.SetPath((RulesType)pProperty->m_lValue);
        pShape->m_bCustomShape = true;
    }break;
    case ODRAW::pSegmentInfo:
    {
        if (pProperty->m_bComplex)
        {
            pShape->m_oCustomVML.LoadSegments(pProperty);
            pShape->m_bCustomShape = true;
        }
    }break;
    case ODRAW::pVertices:
    {
        if (pProperty->m_bComplex)
        {
            pShape->m_oCustomVML.LoadVertices(pProperty);
            pShape->m_bCustomShape = true;
        }
    }break;
    case ODRAW::pConnectionSites:
    {
        if (pProperty->m_bComplex)
        {
            pShape->m_oCustomVML.LoadConnectionSites(pProperty);
        }
    }break;
    case ODRAW::pConnectionSitesDir:
    {
        if (pProperty->m_bComplex)
        {
            pShape->m_oCustomVML.LoadConnectionSitesDir(pProperty);
        }
    }break;
    case ODRAW::pGuides:
    {
        if (pProperty->m_bComplex/* && pShape->m_eType != sptNotchedCircularArrow*/)
        {//Тікбұрышты үшбұрыштарды.ppt - slide 25
            pShape->m_oCustomVML.LoadGuides(pProperty);
        }
    }break;
    case ODRAW::pInscribe:
    {
        if (pProperty->m_bComplex)
        {
            pShape->m_oCustomVML.LoadInscribe(pProperty);
        }
    }break;
    case ODRAW::pAdjustHandles:
    {
        if (pProperty->m_bComplex)
        {
            pShape->m_oCustomVML.LoadAHs(pProperty);
        }
    }break;
    case ODRAW::adjustValue:
    case ODRAW::adjust2Value:
    case ODRAW::adjust3Value:
    case ODRAW::adjust4Value:
    case ODRAW::adjust5Value:
    case ODRAW::adjust6Value:
    case ODRAW::adjust7Value:
    case ODRAW::adjust8Value:
    case ODRAW::adjust9Value:
    case ODRAW::adjust10Value:
    {
        LONG lIndexAdj = pProperty->m_ePID - ODRAW::adjustValue;
        if (lIndexAdj >= 0 && lIndexAdj < (LONG)pShape->m_arAdjustments.size())
        {
            pShape->m_oCustomVML.LoadAdjusts(lIndexAdj, (LONG)pProperty->m_lValue);
        }
        else
        {
            pShape->m_oCustomVML.LoadAdjusts(lIndexAdj, (LONG)pProperty->m_lValue);
        }
    }break;
        //--------------------------------------------------------------------------------------------------------------------
    case lTxid:
    {
    }break;
    case ODRAW::dxTextLeft:
    {
        pParentShape->m_dTextMarginX = pProperty->m_lValue;
    }break;
    case ODRAW::dxTextRight:
    {
        pParentShape->m_dTextMarginRight = pProperty->m_lValue;
    }break;
    case ODRAW::dyTextTop:
    {
        pParentShape->m_dTextMarginY = pProperty->m_lValue;
    }break;
    case ODRAW::dyTextBottom:
    {
        pParentShape->m_dTextMarginBottom = pProperty->m_lValue;
    }break;
    case ODRAW::WrapText:
    {
        pParentShape->m_oText.m_lWrapMode = (LONG)pProperty->m_lValue;
    }break;
    case ODRAW::gtextUNICODE://word art text
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
                PPT::CParagraph p;
                PPT::CSpan s;
                s.m_strText = str.substr(0,length);
                p.m_arSpans.push_back(s);
                pParentShape->m_oText.m_arParagraphs.push_back(p);
            }
        }
    }break;
    case ODRAW::gtextFont:
    {
        if (pProperty->m_bComplex && 0 < pProperty->m_lValue)
        {
            std::wstring str = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)pProperty->m_pOptions, pProperty->m_lValue/2-1);
            pParentShape->m_oText.m_oAttributes.m_oFont.Name = str;
        }
    }break;
    case ODRAW::gtextSize:
    {
        pParentShape->m_oText.m_oAttributes.m_oFont.Size = (INT)((pProperty->m_lValue >> 16) & 0x0000FFFF);
        break;
    }
    case ODRAW::anchorText:
    {
        switch (pProperty->m_lValue)
        {
        case ODRAW::anchorTop:
        case ODRAW::anchorTopBaseline:
        {
            pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 0;
            break;
        }
        case ODRAW::anchorMiddle:
        {
            pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 1;
            break;
        }
        case ODRAW::anchorBottom:
        case ODRAW::anchorBottomBaseline:
        {
            pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 0;
            pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 2;
            break;
        }
        case ODRAW::anchorTopCentered:
        case ODRAW::anchorTopCenteredBaseline:
        {
            pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
            pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 0;
            break;
        }
        case ODRAW::anchorMiddleCentered:
        {
            pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
            pParentShape->m_oText.m_oAttributes.m_nTextAlignVertical = 1;
            break;
        }
        case ODRAW::anchorBottomCentered:
        case ODRAW::anchorBottomCenteredBaseline:
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
    case ODRAW::gtextAlign:
    {
        switch (pProperty->m_lValue)
        {
        case ODRAW::alignTextLeft:
        {
            pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 0;
        }break;
        case ODRAW::alignTextCenter:
        {
            pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
        }break;
        case ODRAW::alignTextRight:
        {
            pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 2;
        }break;
        default:
        {
            pParentShape->m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;
        }break;
        };
        break;
    }
    case ODRAW::geometryTextBooleanProperties:
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
    case ODRAW::cdirFont:
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
    case ODRAW::txflTextFlow:
    {
        pParentShape->m_oText.m_nTextFlow = pProperty->m_lValue;
    }break;
    case ODRAW::textBooleanProperties:
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
    case ODRAW::c3DSpecularAmt:
    {
        pShape->m_3dOptions.dSpecularAmt = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DDiffuseAmt:
    {
        pShape->m_3dOptions.dDiffuseAmt = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DShininess:
    {
        pShape->m_3dOptions.dShininess = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DEdgeThickness:
    {
        pShape->m_3dOptions.nEdgeThickness = pProperty->m_lValue;
    }break;
    case ODRAW::C3DExtrudeForward:
    {
        pShape->m_3dOptions.nExtrudeForward = pProperty->m_lValue;
    }break;
    case ODRAW::c3DExtrudeBackward:
    {
        pShape->m_3dOptions.nExtrudeBackward = pProperty->m_lValue;
    }break;
    case ODRAW::c3DExtrudePlane:
    {
        //ExtrudePlane = 0;
    }break;
    case ODRAW::c3DExtrusionColor:
    {
        SColorAtom oAtom;
        oAtom.FromValue(pProperty->m_lValue);

        CColor tmp;
        if(oAtom.bSysIndex)	tmp = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
        else				oAtom.ToColor(&tmp);

        pShape->m_3dOptions.oExtrusionColor = tmp;
    }break;
    case ODRAW::c3DCrMod:
    {
        SColorAtom oAtom;
        oAtom.FromValue(pProperty->m_lValue);

        CColor tmp;
        if(oAtom.bSysIndex)	tmp = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
        else				oAtom.ToColor(&tmp);

        pShape->m_3dOptions.oCrMod = tmp;
    }break;
    case ODRAW::c3DExtrusionColorExt:
    {
        SColorAtom oAtom;
        oAtom.FromValue(pProperty->m_lValue);

        CColor tmp;
        if(oAtom.bSysIndex)	tmp = CorrectSysColor(pProperty->m_lValue, pElement, pTheme);
        else				oAtom.ToColor(&tmp);
        pShape->m_3dOptions.oExtrusionColorExt = tmp;
    }break;
    case ODRAW::c3DExtrusionColorExtMod:
    {
        pShape->m_3dOptions.nTypeExtrusionColorExt = (pProperty->m_lValue & 0x00000300) >> 8;
    }break;
    case ODRAW::c3DBottomBevelWidth:
    {
        pShape->m_3dOptions.dBottomBevelWidth = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DBottomBevelHeight:
    {
        pShape->m_3dOptions.dBottomBevelHeight = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DBottomBevelType:
    {
        pShape->m_3dOptions.nBottomBevelType = pProperty->m_lValue;
    }break;
    case ODRAW::c3DTopBevelWidth:
    {
        pShape->m_3dOptions.dTopBevelWidth = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DTopBevelHeight:
    {
        pShape->m_3dOptions.dTopBevelHeight = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DTopBevelType:
    {
        pShape->m_3dOptions.nTopBevelType = pProperty->m_lValue;
    }break;
    case ODRAW::threeDObjectBooleanProperties:
    {
        bool fUsef3D					= GETBIT(pProperty->m_lValue, 19);
        bool fUsefc3DMetallic			= GETBIT(pProperty->m_lValue, 18);
        bool fUsefc3DUseExtrusionColor	= GETBIT(pProperty->m_lValue, 17);
        bool fUsefc3DLightFace			= GETBIT(pProperty->m_lValue, 16);

        pShape->m_3dOptions.bEnabled		= fUsef3D					? GETBIT(pProperty->m_lValue, 3)	: false;
        pShape->m_3dOptions.bMetallic		= fUsefc3DMetallic			? GETBIT(pProperty->m_lValue, 2)	: false;
        pShape->m_3dOptions.bExtrusionColor= fUsefc3DUseExtrusionColor	? GETBIT(pProperty->m_lValue, 1)	: false;
        pShape->m_3dOptions.bLightFace		= fUsefc3DLightFace			? GETBIT(pProperty->m_lValue, 0)	: true;

    }break;
    case ODRAW::c3DYRotationAngle:
    {
        double val = FixedPointToDouble(pProperty->m_lValue);
        if (val < 0) val += 360;
        pShape->m_3dOptions.dYRotationAngle = val;
    }break;
    case ODRAW::c3DXRotationAngle:
    {
        double val = FixedPointToDouble(pProperty->m_lValue);
        if (val < 0) val += 360;
        pShape->m_3dOptions.dXRotationAngle = val;
    }break;
    case ODRAW::c3DRotationAxisX:
    {
        pShape->m_3dOptions.dRotationAxisX = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DRotationAxisY:
    {
        pShape->m_3dOptions.dRotationAxisY = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DRotationAxisZ:
    {
        pShape->m_3dOptions.dRotationAxisZ = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DRotationAngle:
    {
        pShape->m_3dOptions.dRotationAngle = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DRotationCenterX:
    {
        pShape->m_3dOptions.dRotationCenterX = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DRotationCenterY:
    {
        pShape->m_3dOptions.dRotationCenterY = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DRotationCenterZ:
    {
        pShape->m_3dOptions.dRotationCenterZ = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DRenderMode:
    {
        pShape->m_3dOptions.nRenderMode = pProperty->m_lValue;
    }break;
    case ODRAW::c3DTolerance:
    {
        pShape->m_3dOptions.dTolerance = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DXViewpoint:
    {
        pShape->m_3dOptions.dXViewpoint = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DYViewpoint:
    {
        pShape->m_3dOptions.dYViewpoint = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DZViewpoint:
    {
        pShape->m_3dOptions.dZViewpoint = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DOriginX:
    {
        pShape->m_3dOptions.dOriginX = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DOriginY:
    {
        pShape->m_3dOptions.dOriginY = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DSkewAngle:
    {
        double val = FixedPointToDouble(pProperty->m_lValue);
        if (val <= 0) val += 360;
        pShape->m_3dOptions.dSkewAngle = val;
    }break;
    case ODRAW::c3DSkewAmount:
    {
        pShape->m_3dOptions.nSkewAmount = pProperty->m_lValue;
    }break;
    case ODRAW::c3DAmbientIntensity:
    {
        pShape->m_3dOptions.dAmbientIntensity = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DKeyX:
    {
        pShape->m_3dOptions.dKeyX = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DKeyY:
    {
        pShape->m_3dOptions.dKeyY = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DKeyZ:
    {
        pShape->m_3dOptions.dKeyZ = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DKeyIntensity:
    {
        pShape->m_3dOptions.dKeyIntensity = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DFillX:
    {
        pShape->m_3dOptions.dFillX = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DFillY:
    {
        pShape->m_3dOptions.dFillY = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DFillZ:
    {
        pShape->m_3dOptions.dFillZ = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::c3DFillIntensity:
    {
        pShape->m_3dOptions.dFillIntensity = FixedPointToDouble(pProperty->m_lValue);
    }break;
    case ODRAW::threeDStyleBooleanProperties:
    {
        bool fUsefc3DConstrainRotation	= GETBIT(pProperty->m_lValue, 20);
        bool fUsefc3DRotationCenterAuto	= GETBIT(pProperty->m_lValue, 19);
        bool fUsefc3DParallel			= GETBIT(pProperty->m_lValue, 18);
        bool fUsefc3DKeyHarsh			= GETBIT(pProperty->m_lValue, 17);
        bool fUsefc3DFillHarsh			= GETBIT(pProperty->m_lValue, 16);

        pShape->m_3dOptions.bConstrainRotation	= fUsefc3DConstrainRotation	? GETBIT(pProperty->m_lValue, 4)	: true;
        pShape->m_3dOptions.bRotationCenterAuto= fUsefc3DRotationCenterAuto? GETBIT(pProperty->m_lValue, 3)	: false;
        pShape->m_3dOptions.bParallel			= fUsefc3DParallel			? GETBIT(pProperty->m_lValue, 2)	: true;
        pShape->m_3dOptions.bKeyHarsh			= fUsefc3DKeyHarsh			? GETBIT(pProperty->m_lValue, 1)	: true;
        pShape->m_3dOptions.bFillHarsh			= fUsefc3DFillHarsh			? GETBIT(pProperty->m_lValue, 0)	: true;
    }break;
    default:
    {
        int unknown_value = pProperty->m_lValue;
        unknown_value = unknown_value;
    }break;
    }
}


CRecordShapeContainer::CRecordShapeContainer()
{
    bGroupShape = false;

    m_pStream = NULL;

}

CRecordShapeContainer::~CRecordShapeContainer()
{
    m_pStream = NULL;
}

void CRecordShapeContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_pStream = pStream;
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}

CElementPtr CRecordShapeContainer::GetElement (bool inGroup, CExMedia* pMapIDs,
                                               CTheme* pTheme, CLayout* pLayout,
                                               CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide)
{
    CElementPtr pElement;

    std::vector<CRecordShape*> oArrayShape;
    GetRecordsByType(&oArrayShape, true, true);

    if (0 == oArrayShape.size())
        return pElement;

    // PlaceHolder
    std::vector<CRecordPlaceHolderAtom*> oArrayPlaceHolder;
    GetRecordsByType(&oArrayPlaceHolder, true, true);

    std::vector<CRecordShapeProperties*> oArrayOptions;
    GetRecordsByType(&oArrayOptions, true, /*true*/false/*secondary & tetriary*/);

    PPTShapes::ShapeType eType = (PPTShapes::ShapeType)oArrayShape[0]->m_oHeader.RecInstance;
    PPT::ElementType elType = GetTypeElem((ODRAW::eSPT)oArrayShape[0]->m_oHeader.RecInstance);

    int lMasterID = -1;

    CElementPtr pElementLayout;

    if (NULL != pSlide)
    {
        bool bIsMaster = oArrayShape[0]->m_bHaveMaster;
        if (bIsMaster && elType !=etPicture)
        {
            for (size_t i = 0; i < oArrayOptions[0]->m_oProperties.m_lCount; ++i)
            {
                if (hspMaster == oArrayOptions[0]->m_oProperties.m_arProperties[i].m_ePID)
                {
                    lMasterID = oArrayOptions[0]->m_oProperties.m_arProperties[i].m_lValue;

                    if (pLayout && !oArrayPlaceHolder.empty())
                    {
                        int placeholder_type	= oArrayPlaceHolder[0]->m_nPlacementID;
                        int placeholder_id		= oArrayPlaceHolder[0]->m_nPosition;

                        size_t nIndexMem = pLayout->m_arElements.size();

                        for (size_t nIndex = 0; nIndex < nIndexMem; ++nIndex)
                        {
                            if ((placeholder_type == pLayout->m_arElements[nIndex]->m_lPlaceholderType )  &&
                                    (	placeholder_id < 0 || pLayout->m_arElements[nIndex]->m_lPlaceholderID < 0 ||
                                        placeholder_id == pLayout->m_arElements[nIndex]->m_lPlaceholderID))
                            {
                                if (pLayout->m_arElements[nIndex]->m_bPlaceholderSet == false)
                                {
                                    pElementLayout			= pLayout->m_arElements[nIndex]; //для переноса настроек
                                    pElementLayout->m_lID	= lMasterID;

                                    if (placeholder_id >= 0 && pLayout->m_arElements[nIndex]->m_lPlaceholderID < 0 )
                                        pLayout->m_arElements[nIndex]->m_lPlaceholderID = placeholder_id;
                                }

                                pElement = pLayout->m_arElements[nIndex]->CreateDublicate();

                                if (elType == etShape)
                                {
                                    CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElement.get());
                                    if (NULL != pShape)
                                        pShape->m_pShape->m_oText.m_arParagraphs.clear();
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
    if (!pElement)
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

                pVideoElem->m_strVideoFileName	= oInfo.m_strFilePath ;
                pVideoElem->m_strImageFileName	= oInfoDefault.m_strFilePath + FILE_SEPARATOR_STR;

                pElement = CElementPtr(pVideoElem);
            }
            else if (CExFilesInfo::eftAudio == exType)
            {
                CAudioElement* pAudioElem		= new CAudioElement();
                pElement = CElementPtr(pAudioElem);

                pAudioElem->m_strAudioFileName	= oInfo.m_strFilePath;
                pAudioElem->m_strImageFileName	= oInfoDefault.m_strFilePath + FILE_SEPARATOR_STR;

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
                        pLayout->m_arElements.push_back(pElement);
                }

            }
            else
            {
                CImageElement* pImageElem		= new CImageElement();
                pImageElem->m_strImageFileName	= oInfo.m_strFilePath + FILE_SEPARATOR_STR;

                pElement = CElementPtr(pImageElem);
            }
        }break;
        default:
        {
            if (isTable())
            {
                CTableElement* pTableElem = new CTableElement();
                pTableElem->m_xmlRawData = getTableXmlStr();
                pTableElem->m_etType = etGroup;
                pElement = CElementPtr(pTableElem);
            }
            else if (bGroupShape)
            {
                CGroupElement* pGroupElem = new CGroupElement();
                pElement = CElementPtr(pGroupElem);
            }
            else
            {
                // shape
                CShapeElement* pShape = new CShapeElement(NSBaseShape::ppt, eType);
                CPPTShape *ppt_shape = dynamic_cast<CPPTShape *>(pShape->m_pShape->getBaseShape().get());

                if ( (ppt_shape) && (OOXMLShapes::sptCustom == ppt_shape->m_eType))
                {
                    pShape->m_bShapePreset = true;
                }
                pElement = CElementPtr(pShape);
            }
        }break;
        }
    }

    if (!pElement)
        return pElement;

    pElement->m_lID			= oArrayShape[0]->m_nID;
    pElement->m_lLayoutID	= lMasterID;

    //---------внешние ссылки
    {
        CExFilesInfo::ExFilesType exType		= CExFilesInfo::eftNone;
        CExFilesInfo			* pTextureInfo	= pMapIDs->Lock(0xFFFFFFFF, exType);

        if (NULL != pTextureInfo)
        {
            pElement->m_oBrush.TexturePath = pTextureInfo->m_strFilePath + FILE_SEPARATOR_STR;
        }

        std::vector<CRecordExObjRefAtom*> oArrayEx;
        GetRecordsByType(&oArrayEx, true, true);
        if (0 != oArrayEx.size())
        {
            CExFilesInfo* pInfo = pMapIDs->Lock(oArrayEx[0]->m_nExObjID, exType);

            if (CExFilesInfo::eftHyperlink == exType && pInfo)
            {
                pElement->m_sHyperlink = pInfo->m_strFilePath;
            }
        }
    }
    std::wstring strShapeText;
    //------------------------------------------------------------------------------------------------
    // placeholders
    if (0 < oArrayPlaceHolder.size())
    {
        pElement->m_bLine					= false; //по умолчанию у них нет линий
        pElement->m_lPlaceholderID			= oArrayPlaceHolder[0]->m_nPosition;
        pElement->m_lPlaceholderType		= oArrayPlaceHolder[0]->m_nPlacementID;
        pElement->m_lPlaceholderSizePreset	= oArrayPlaceHolder[0]->m_nSize;

        if (pElementLayout)
            pElementLayout->m_lPlaceholderSizePreset	= oArrayPlaceHolder[0]->m_nSize;

        CorrectPlaceholderType(pElement->m_lPlaceholderType);
    }

    std::vector<RoundTripHFPlaceholder12Atom*> oArrayHFPlaceholder;
    GetRecordsByType(&oArrayHFPlaceholder, true, true);
    if (0 < oArrayHFPlaceholder.size())
    {
        pElement->m_lPlaceholderType	= oArrayHFPlaceholder[0]->m_nPlacementID;//PT_MasterDate, PT_MasterSlideNumber, PT_MasterFooter, or PT_MasterHeader
        CorrectPlaceholderType(pElement->m_lPlaceholderType);

        if (pLayout)
        {
            std::multimap<int, CElementPtr>::iterator it = pLayout->m_mapPlaceholders.find(pElement->m_lPlaceholderType);
            if (it != pLayout->m_mapPlaceholders.end())
            {
                pElement->m_lPlaceholderID = (it->second)->m_lPlaceholderID;
            }
        }
    }
    //meta placeholders
    std::vector<CRecordFooterMetaAtom*> oArrayFooterMeta;

    GetRecordsByType(&oArrayFooterMeta, true, true);
    if (0 < oArrayFooterMeta.size())
    {
        pElement->m_lPlaceholderType		= PT_MasterFooter;
        pElement->m_lPlaceholderUserStr	= oArrayFooterMeta[0]->m_nPosition;
    }
    std::vector<CRecordSlideNumberMetaAtom*> oArraySlideNumberMeta;
    GetRecordsByType(&oArraySlideNumberMeta, true, true);
    if (0 < oArraySlideNumberMeta.size())
    {
        pElement->m_lPlaceholderType = PT_MasterSlideNumber;
    }
    std::vector<CRecordGenericDateMetaAtom*> oArrayDateMeta;
    GetRecordsByType(&oArrayDateMeta, true, true);
    if (0 < oArrayDateMeta.size())
    {
        pElement->m_lPlaceholderType		= PT_MasterDate;

        CRecordDateTimeMetaAtom* format_data = dynamic_cast<CRecordDateTimeMetaAtom*>(oArrayDateMeta[0]);
        if (format_data)
        {
            pElement->m_nFormatDate			= 1;
            //todooo сделать форматированый вывод
        }
        else
        {
            pElement->m_lPlaceholderUserStr	= oArrayDateMeta[0]->m_nPosition;
            pElement->m_nFormatDate			= 2;
        }
    }
    //------------- привязки ---------------------------------------------------------------------------------
    std::vector<CRecordGroupShape*> oArrayGroupShape;
    this->GetRecordsByType(&oArrayGroupShape, true, true);

    if (0 != oArrayGroupShape.size())
    {
        pElement->m_rcGroupAnchor.left	= (LONG)oArrayGroupShape[0]->m_oBounds.left;
        pElement->m_rcGroupAnchor.top	= (LONG)oArrayGroupShape[0]->m_oBounds.top;
        pElement->m_rcGroupAnchor.right	= (LONG)oArrayGroupShape[0]->m_oBounds.right;
        pElement->m_rcGroupAnchor.bottom	= (LONG)oArrayGroupShape[0]->m_oBounds.bottom;

        pElement->m_bGroupAnchorEnabled	= true;
    }
    std::vector<CRecordClientAnchor*> oArrayAnchor;
    this->GetRecordsByType(&oArrayAnchor, true, true);

    if (0 != oArrayAnchor.size())
    {
        pElement->m_rcAnchor.left	= (LONG)oArrayAnchor[0]->m_oBounds.Left;
        pElement->m_rcAnchor.top	= (LONG)oArrayAnchor[0]->m_oBounds.Top;
        pElement->m_rcAnchor.right	= (LONG)oArrayAnchor[0]->m_oBounds.Right;
        pElement->m_rcAnchor.bottom	= (LONG)oArrayAnchor[0]->m_oBounds.Bottom;

        pElement->m_bAnchorEnabled	= true;
    }

    std::vector<CRecordChildAnchor*> oArrayChildAnchor;
    this->GetRecordsByType(&oArrayChildAnchor, true, true);

    if (0 != oArrayChildAnchor.size())
    {
        pElement->m_rcChildAnchor.left	= oArrayChildAnchor[0]->m_oBounds.left;
        pElement->m_rcChildAnchor.top	= oArrayChildAnchor[0]->m_oBounds.top;
        pElement->m_rcChildAnchor.right	= oArrayChildAnchor[0]->m_oBounds.right;
        pElement->m_rcChildAnchor.bottom	= oArrayChildAnchor[0]->m_oBounds.bottom;

        pElement->m_bChildAnchorEnabled	= true;
    }

    pElement->m_bFlipH = oArrayShape[0]->m_bFlipH;
    pElement->m_bFlipV = oArrayShape[0]->m_bFlipV;

    pElement->m_bIsBackground	=	(true == oArrayShape[0]->m_bBackground);
    pElement->m_bHaveAnchor		=	(true == oArrayShape[0]->m_bHaveAnchor);

    if (pElementLayout && (pElement->m_bAnchorEnabled || pElement->m_bChildAnchorEnabled))
    {
        pElementLayout->m_rcAnchor			= pElement->m_rcAnchor;
        pElementLayout->m_rcChildAnchor		= pElement->m_rcChildAnchor;

        pElementLayout->m_bPlaceholderSet		= true;
        pElementLayout->m_bAnchorEnabled		= pElement->m_bAnchorEnabled;
        pElementLayout->m_bChildAnchorEnabled	= pElement->m_bChildAnchorEnabled;
    }

    std::vector<CRecordOfficeArtClientData*> oArrayArtClient;
    GetRecordsByType(&oArrayArtClient, false, true);
    std::vector<CRecordMouseInteractiveInfoContainer*> oArrayInteractive;

    if (oArrayArtClient.size())
        oArrayArtClient[0]->GetRecordsByType(&oArrayInteractive, true, false);

    for (auto const* interactiveCont : oArrayInteractive)
    {
        CInteractiveInfo interactiveInfo;
        ConvertInteractiveInfo(interactiveInfo, interactiveCont, pMapIDs);

        pElement->m_arrActions.push_back(interactiveInfo);
    }


    //--------- наличие текста --------------------------------------------------------------------------
    CShapeElement* pShapeElem = dynamic_cast<CShapeElement*>(pElement.get());

    if (NULL != pShapeElem)
    {
        CElementInfo oElementInfo;

        oElementInfo.m_lMasterPlaceholderType = pElement->m_lPlaceholderType;

        pShapeElem->m_pShape->m_dWidthLogic  = ShapeSizeVML;
        pShapeElem->m_pShape->m_dHeightLogic = ShapeSizeVML;

        // проверка на textheader present
        std::vector<CRecordTextHeaderAtom*> oArrayTextHeader;
        GetRecordsByType(&oArrayTextHeader, true, true);

        if (0 < oArrayTextHeader.size())
        {
            pShapeElem->m_pShape->m_oText.m_lTextType		= oArrayTextHeader[0]->m_nTextType;
            pShapeElem->m_pShape->m_oText.m_lTextMasterType	= oArrayTextHeader[0]->m_nTextType;
            oElementInfo.m_lMasterTextType					= oArrayTextHeader[0]->m_nTextType;
        }
        else
        {
            pShapeElem->m_pShape->m_oText.m_lTextType		= NoPresent;
            pShapeElem->m_pShape->m_oText.m_lTextMasterType	= NoPresent;
            oElementInfo.m_lMasterTextType					= NoPresent;
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

        if (pElement->m_lPlaceholderType == PT_MasterSlideNumber && strShapeText.length() > 5)
        {
            int pos = strShapeText.find(L"*");
            if (pos < 0) pElement->m_lPlaceholderType = PT_MasterFooter; ///???? 1-(33).ppt
        }

        //------ shape properties ----------------------------------------------------------------------------------------
        CPPTElement oElement;
        for (size_t nIndexProp = 0; nIndexProp < oArrayOptions.size(); ++nIndexProp)
        {
            oElement.SetUpProperties(pElement, pTheme, pSlideWrapper, pSlide, &oArrayOptions[nIndexProp]->m_oProperties, (nIndexProp == 0));
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
            pShapeElem->m_pShape->m_oText.m_oRuler = oArrayTextRuler[0]->m_oTextRuler;
        }


        std::vector<CRecordOfficeArtClientTextbox*> oArrayTextBox;
        std::vector<CRecordMouseInteractiveInfoContainer*> oArrayInteractiveCont;
        std::vector<CRecordTextInteractiveInfoAtom*> oArrayTextInteractive;
        this->GetRecordsByType(&oArrayTextBox, false);

        if (oArrayTextBox.size())
        {
            // It's differance records
            oArrayTextBox[0]->GetRecordsByType(&oArrayTextInteractive, false);
            oArrayTextBox[0]->GetRecordsByType(&oArrayInteractiveCont, false);
        }

        for (const auto* pInerAtom : oArrayInteractiveCont)
        {
            CInteractiveInfo interactive;
            ConvertInteractiveInfo(interactive, pInerAtom, pMapIDs);
            pShapeElem->m_textHyperlinks.push_back(interactive);
        }

        if (!oArrayTextInteractive.empty())
        {
            pShapeElem->m_oTextActions.m_bPresent = true;

            int nSize = oArrayTextInteractive.size();
            for (int i = 0; i < nSize; ++i)
            {
                CTextRange oRange;

                oRange.m_lStart = oArrayTextInteractive[i]->m_lStart;
                oRange.m_lEnd	= oArrayTextInteractive[i]->m_lEnd;

                pShapeElem->m_oTextActions.m_arRanges.push_back(oRange);
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
            if (pShapeElem->m_bAnchorEnabled)
            {
                double dW = pShapeElem->m_rcAnchor.GetWidth();
                double dH = pShapeElem->m_rcAnchor.GetHeight();

                double dCx = (pShapeElem->m_rcAnchor.left + pShapeElem->m_rcAnchor.right) / 2.0;
                double dCy = (pShapeElem->m_rcAnchor.top + pShapeElem->m_rcAnchor.bottom) / 2.0;

                pShapeElem->m_rcAnchor.left		= dCx - dH / 2.0;
                pShapeElem->m_rcAnchor.right	= dCx + dH / 2.0;

                pShapeElem->m_rcAnchor.top		= dCy - dW / 2.0;
                pShapeElem->m_rcAnchor.bottom	= dCy + dW / 2.0;
            }
            if (pShapeElem->m_bChildAnchorEnabled)
            {
                double dW = pShapeElem->m_rcChildAnchor.GetWidth();
                double dH = pShapeElem->m_rcChildAnchor.GetHeight();

                double dCx = (pShapeElem->m_rcChildAnchor.left + pShapeElem->m_rcChildAnchor.right) / 2.0;
                double dCy = (pShapeElem->m_rcChildAnchor.top + pShapeElem->m_rcChildAnchor.bottom) / 2.0;

                pShapeElem->m_rcChildAnchor.left		= dCx - dH / 2.0;
                pShapeElem->m_rcChildAnchor.right	= dCx + dH / 2.0;

                pShapeElem->m_rcChildAnchor.top		= dCy - dW / 2.0;
                pShapeElem->m_rcChildAnchor.bottom	= dCy + dW / 2.0;
            }
        }

        std::vector<CRecordMasterTextPropAtom*> oArrayMasterTextProp;
        GetRecordsByType(&oArrayMasterTextProp, true);

        CRecordMasterTextPropAtom* master_level = NULL;
        if (!oArrayMasterTextProp.empty())
            master_level = oArrayMasterTextProp[0];

        pSlideWrapper->m_mapElements.insert(std::pair<LONG, CElementInfo>(pShapeElem->m_lID, oElementInfo));

        SetUpTextStyle(strShapeText, pTheme, pLayout, pElement, pThemeWrapper, pSlideWrapper, pSlide, master_level);
    }
    else
    {//image, audio, video ....
        CPPTElement oElement;
        for (size_t nIndexProp = 0; nIndexProp < oArrayOptions.size(); ++nIndexProp)
        {
            oElement.SetUpProperties(pElement, pTheme, pSlideWrapper, pSlide, &oArrayOptions[nIndexProp]->m_oProperties, (nIndexProp == 0));
        }

        pElement->m_lLayoutID = lMasterID;
    }
    //----------------------------------------------------------------------------------------------------
    if (NULL != pSlide)
    {
        pElement->m_dStartTime		= pSlide->m_dStartTime;
        pElement->m_dEndTime		= pSlide->m_dEndTime;
    }
    else
    {
        pElement->m_dStartTime		= 0;
        pElement->m_dEndTime		= 0;
    }

    return pElement;
}

PPT::ElementType CRecordShapeContainer::GetTypeElem(eSPT eType)
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

std::wstring CRecordShapeContainer::GetFileName(std::wstring strFilePath)
{
    int nIndex = strFilePath.rfind(wchar_t('\\'));
    if (-1 != nIndex)
        return strFilePath.substr(nIndex + 1);
    else
        return strFilePath;
}
bool CRecordShapeContainer::isTable() const
{
    std::vector<CRecordShapeProperties*> oArrayOptions;
    GetRecordsByType(&oArrayOptions, true, false);

    for (const auto* option : oArrayOptions)
    {
        for (const auto& prop : option->m_oProperties.m_arProperties)
        {
            if ((prop.m_ePID == tableProperties ||
                 prop.m_ePID == tableRowProperties) &&
                    bGroupShape)
            {
                return true;
            }
        }
    }
    return false;
}

std::wstring CRecordShapeContainer::getTableXmlStr() const
{
    std::vector<CRecordShapeProperties*> oArrayOptions;
    GetRecordsByType(&oArrayOptions, true, false);
    std::wstring xmlStr;

    if (oArrayOptions.size() > 1 &&
            oArrayOptions[1]->m_oProperties.m_arProperties.size() > 2)
    {
        COfficeUtils officeUtils(NULL);
        BYTE *utf8Data = NULL;
        ULONG utf8DataSize = 0;
        auto& xmlProp = oArrayOptions[1]->m_oProperties.m_arProperties[2]; //id == 0x2065 ?

        if (xmlProp.m_pOptions && xmlProp.m_lValue > 0) // file513.ppt
        {
            std::wstring temp = NSDirectory::GetTempPath();
            std::wstring tempFileName = temp + FILE_SEPARATOR_STR + L"tempMetroBlob.zip";

            NSFile::CFileBinary file;
            if (file.CreateFileW(tempFileName))
            {
                file.WriteFile(xmlProp.m_pOptions, xmlProp.m_lValue);
                file.CloseFile();
            }

            if (S_OK == officeUtils.LoadFileFromArchive(tempFileName, L"drs/e2oDoc.xml", &utf8Data, utf8DataSize))
            {
                xmlStr = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(utf8Data, utf8DataSize);
            }

            delete[] utf8Data;
            NSFile::CFileBinary::Remove(tempFileName);
        }
    }

    return xmlStr;
}

void CRecordShapeContainer::ApplyThemeStyle(CElementPtr pElem, CTheme* pTheme, CRecordMasterTextPropAtom* master_levels)
{
    CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem.get());
    if (NULL == pShape)
        return;

    CTextAttributesEx* pText = &(pShape->m_pShape->m_oText);


    if (master_levels)
    {
        for (size_t i = 0; i < pText->m_arParagraphs.size(); i++)
        {
            if (i >= master_levels->m_arrProps.size()) break;

            pText->m_arParagraphs[i].m_lTextLevel = master_levels->m_arrProps[i].lIndentLevel;
            pText->m_arParagraphs[i].m_oPFRun.leftMargin.reset();
            pText->m_arParagraphs[i].m_oPFRun.indent.reset();
        }
    }

    pText->ApplyThemeStyle(pTheme);
    // AutoNumbering and BulletBlip
    ConvertStyleTextProp9(pText);
}
void CRecordShapeContainer::SetUpTextStyle(std::wstring& strText, CTheme* pTheme, CLayout* pLayout, CElementPtr pElem, CSlideInfo* pThemeWrapper, CSlideInfo* pSlideWrapper, CSlide* pSlide, CRecordMasterTextPropAtom* master_levels)
{
    // сначала проверяем на shape
    // затем применяем все настройки по-очереди
    // 1) master + TextMasterStyles
    // 2) persist + TextMasterStyles
    // 3) свои настройки + TextMasterStyles
    // причем "свои настройки" - это чисто "продвинутые настройки"
    // потому что все общие ( через проперти ) - уже установлены

    if (NULL == pElem)
        return;

    if (etShape != pElem->m_etType)
        return;

    CShapeElement* pShape = dynamic_cast<CShapeElement*>(pElem.get());
    if (NULL == pShape)
        return;

    CTextAttributesEx* pTextSettings = &(pShape->m_pShape->m_oText);

    // сначала применим ссылки на masterstyle (для шаблонного элемента)
    // как узнать - просто есть ли массивы (т.к. они могли появиться пока только оттуда)
    // - теперь этого делать не нужно - т.к. в мастере тоже вызывается эта функция -
    // и там все это должно уже примениться
    bool bIsPersistPresentSettings	= false;
    bool bIsOwnPresentSettings		= false;

    TextTypeEnum eTypeMaster	= NoPresent;
    TextTypeEnum eTypePersist	= NoPresent;
    TextTypeEnum eTypeOwn		= (TextTypeEnum)pTextSettings->m_lTextType;

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
                CElementPtr & pPh = pLayout->m_arElements[i];
                if ((etShape == pPh->m_etType) && (ph_type == pPh->m_lPlaceholderType) && (/*ph_pos == pPh->m_lPlaceholderID*/true))
                {
                    pElementLayoutPH = dynamic_cast<CShapeElement*>(pPh.get());
                    eTypeMaster = (TextTypeEnum)pElementLayoutPH->m_pShape->m_oText.m_lTextMasterType;
                    break;
                }
            }
        }
    }
    else
    {
        eTypeMaster = (TextTypeEnum)pTextSettings->m_lTextMasterType;
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

        eTypePersist = (TextTypeEnum)pSettings->m_nTextType;
        strText = pSettings->ApplyProperties(pTextSettings);

        if ((0 != pSettings->m_arRanges.size()) && (0 == pShape->m_oTextActions.m_arRanges.size()))
        {
            pShape->m_oTextActions.m_bPresent = true;
            pShape->m_oTextActions.m_arRanges = pSettings->m_arRanges;
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

        if (RT_StyleTextPropAtom == oHeader.RecType)
        {
            CRecordStyleTextPropAtom* pStyle = new CRecordStyleTextPropAtom();
            pStyle->m_lCount = strText.length();

            pStyle->ReadFromStream(oHeader, oElemInfo.m_pStream);

            PPT::ConvertPPTTextToEditorStructure(pStyle->m_arrPFs, pStyle->m_arrCFs, strText, pShape->m_pShape->m_oText);

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
            case PT_Title:
            case PT_MasterTitle:
            case PT_VerticalTitle:
            {
                pTextSettings->m_lStyleThemeIndex = 1;

                if (Tx_TYPE_TITLE != eTypeMaster)
                {
                    if (0 <= nTextMasterType && nTextMasterType < 9)
                    {
                        if (pThemeWrapper->m_pStyles[nTextMasterType].is_init())
                            pTextSettings->m_oStyles = pThemeWrapper->m_pStyles[nTextMasterType].get();
                    }
                }
                break;
            }
            case PT_CenterTitle:
            case PT_MasterCenterTitle:
            {
                pTextSettings->m_lStyleThemeIndex = 1;

                if (Tx_TYPE_TITLE != eTypeMaster)
                {
                    if (0 <= nTextMasterType && nTextMasterType < 9)
                    {
                        if (pThemeWrapper->m_pStyles[nTextMasterType].is_init())
                            pTextSettings->m_oStyles = pThemeWrapper->m_pStyles[nTextMasterType].get();
                    }
                }
                break;
            }
            case PT_Body:
            case PT_MasterBody:
            case PT_NotesBody:
            case PT_MasterNotesBody:
            case PT_VerticalBody:
            case PT_MasterSubTitle:
            case PT_SubTitle:
            {
                pTextSettings->m_lStyleThemeIndex = 2;

                if ((Tx_TYPE_BODY != eTypeMaster) || !pLayout)
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

                if ((Tx_TYPE_OTHER != eTypeMaster) || !pLayout)
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

            //if (Other != eTypeMaster)
            //{
            //	if (0 <= nTextMasterType && nTextMasterType < 9)
            //	{
            //		if (pThemeWrapper->m_pStyles[nTextMasterType].is_init())
            //			pTextSettings->m_oStyles = pThemeWrapper->m_pStyles[nTextMasterType].get();
            //	}
            //}
        }

        // теперь смотрим все остальные стили (persist и own) - просто применяем их к m_oStyles
        if (eTypePersist != NoPresent && eTypePersist != eTypeMaster)
        {
            int nIndexType = (int)eTypePersist;
            if (0 <= nIndexType && nIndexType < 9)
            {
                if (pThemeWrapper->m_pStyles[nIndexType].is_init())
                    pTextSettings->m_oStyles.ApplyAfter(pThemeWrapper->m_pStyles[nIndexType].get());
            }
        }
        if (eTypeOwn != NoPresent && eTypeOwn != eTypePersist && eTypeOwn != eTypeMaster)
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
                pTextSettings->m_oLayoutStyles		= pElementLayoutPH->m_pShape->m_oText.m_oStyles;
                pTextSettings->m_lTextType			= pElementLayoutPH->m_pShape->m_oText.m_lTextType;
                pTextSettings->m_lStyleThemeIndex	= pElementLayoutPH->m_pShape->m_oText.m_lStyleThemeIndex;
            }
            else
            {
                switch (oElemInfo.m_lMasterPlaceholderType)
                {
                case PT_Title:
                case PT_MasterTitle:
                case PT_VerticalTitle:
                {
                    pTextSettings->m_lStyleThemeIndex = 1;
                    break;
                }
                case PT_CenterTitle:
                case PT_MasterCenterTitle:
                {
                    pTextSettings->m_lStyleThemeIndex = 1;
                    break;
                }
                case PT_Body:
                case PT_MasterBody:
                case PT_NotesBody:
                case PT_MasterNotesBody:
                case PT_VerticalBody:
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
            pTextSettings->m_lStyleThemeIndex = -1;
        }

        // теперь смотрим все остальные стили (persist и own) - просто применяем их к m_oStyles
        if (eTypePersist != NoPresent && eTypePersist != eTypeMaster)
        {
            int nIndexType = (int)eTypePersist;
            if (0 <= nIndexType && nIndexType < 9)
            {
                if (pThemeWrapper->m_pStyles[nIndexType].is_init())
                    pTextSettings->m_oStyles.ApplyAfter(pThemeWrapper->m_pStyles[nIndexType].get());
            }
        }
        if (eTypeOwn != NoPresent && eTypeOwn != Tx_TYPE_OTHER && eTypeOwn != eTypePersist && eTypeOwn != eTypeMaster)
        {//齐孟尧-2015年度职工考核报告.ppt
            int nIndexType = (int)eTypeOwn;

            if (0 <= nIndexType && nIndexType < 9 && pLayout)
            {
                if (eTypeOwn == Tx_TYPE_HALFBODY || eTypeOwn == Tx_TYPE_QUARTERBODY)
                {
                    if (pThemeWrapper->m_pStyles[1].IsInit())//body -> (560).ppt
                    {
                        pTextSettings->m_oStyles.ApplyAfter(pThemeWrapper->m_pStyles[1].get());
                    }
                }
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
        std::vector<CTextPFRunRecord> oArrayPF;

        CTextPFRunRecord elm;

        elm.m_lCount = strText.length();
        elm.m_lLevel = 0;

        oArrayPF.push_back(elm);

        std::vector<CTextCFRunRecord> oArrayCF;

        CTextCFRunRecord elm1;
        elm1.m_lCount = elm.m_lCount;

        oArrayCF.push_back(elm1);

        PPT::ConvertPPTTextToEditorStructure(oArrayPF, oArrayCF, strText, *pTextSettings);
    }

    if (NULL != oElemInfo.m_pStream && -1 != oElemInfo.m_lOffsetTextProp)
    {
        //языковые настройки текта
        LONG lPosition = 0; StreamUtils::StreamPosition(lPosition, oElemInfo.m_pStream);

        StreamUtils::StreamSeek(oElemInfo.m_lOffsetTextProp - 8, oElemInfo.m_pStream);

        SRecordHeader oHeader;
        oHeader.ReadFromStream(oElemInfo.m_pStream) ;

        if (RT_TextSpecialInfoAtom == oHeader.RecType)
        {
            CRecordTextSpecInfoAtom* pSpecInfo = new CRecordTextSpecInfoAtom();
            pSpecInfo->m_lCount = -1;

            pSpecInfo->ReadFromStream(oHeader, oElemInfo.m_pStream);
            pSpecInfo->ApplyProperties(&(pShape->m_pShape->m_oText));

            RELEASEOBJECT(pSpecInfo);
        }
        StreamUtils::StreamSeek(lPosition, oElemInfo.m_pStream);
    }
    pShape->m_pShape->m_oText.RecalcParagraphsPPT();

    ApplyThemeStyle(pElem, pTheme, master_levels);

    if (pShape->m_oTextActions.m_bPresent)
    {
        //todooo разобраться нужно ли менять цвет на гиперлинк - 1-(34).ppt
        ODRAW::CColor oColor;
        if ((NULL != pSlide) && !pSlide->m_bUseLayoutColorScheme)			oColor = pSlide->GetColor(11);
        else if ((NULL != pLayout) && (!pLayout->m_bUseThemeColorScheme))	oColor = pLayout->GetColor(11);
        else if (NULL != pTheme)											oColor = pTheme->GetColor(11);
        oColor.m_lSchemeIndex = 11;

        ApplyHyperlink(pShape, oColor);
    }

    CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(pShape->m_pShape->getBaseShape().get());

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
            pShape->m_pShape->m_oText.m_oAttributes.m_oTextBrush = pShape->m_oBrush;

            pShape->m_pShape->m_oText.m_oAttributes.m_nTextAlignHorizontal	= 1;
            pShape->m_pShape->m_oText.m_oAttributes.m_nTextAlignVertical	= 1;

            pShape->m_pShape->m_lDrawType = c_ShapeDrawType_Text;
            break;
        }
        default:
            break;
        };
    }
}

void CRecordShapeContainer::ApplyHyperlink(CShapeElement* pShape, CColor& oColor)
{
    auto& oTextAttributes = pShape->m_pShape->m_oText;
    const auto& originalText = oTextAttributes.m_originalText;


    // lenght these ones shoud be equal
    const auto& arrRanges	= pShape->m_oTextActions.m_arRanges;
    const auto arrSplitedInteractive = splitInteractive(pShape->m_textHyperlinks);

    // It cannot be changed
    if (arrRanges.empty() || arrSplitedInteractive.empty() || originalText.empty())
        return;

    size_t posOrigText(0);
    auto iterRange = arrRanges.begin();
    auto iterInteractive = arrSplitedInteractive.begin();
    for (auto& paragraph : oTextAttributes.m_arParagraphs)
    {
        auto& arrSpans = paragraph.m_arSpans;
        for (auto iterSpan = arrSpans.begin(); iterSpan != arrSpans.end(); iterSpan++)
        {
            // if there isn't needed changes in next spans & paragraps
            if (iterRange == arrRanges.end())
            {
                return;
            }

            const bool isHyperlink = iterRange->inRange(posOrigText);

            const int posBlockStart = posOrigText;
            const int posOrigSpanEnd = posBlockStart + iterSpan->m_strText.length();
            const int posBlockEnd = isHyperlink ?
                        (std::min)(posOrigSpanEnd, iterRange->m_lEnd)  :
                        (std::min)(posOrigSpanEnd, iterRange->m_lStart);
            const size_t blockLen = posBlockEnd - posBlockStart;

            const bool isNeedToSplit = posBlockEnd < posOrigSpanEnd && isHyperlink;

            // Skiping span with '\r'
            if (iterSpan->m_strText.size() && (int)iterSpan->m_strText.find(L"\r") != -1)
                continue;

            posOrigText += blockLen;
            // Skipping span
            if (iterRange->m_lStart > posBlockEnd)
            {
                continue;
            }

            // HYPERLINK
            if (isHyperlink)
            {
                // case: HYPERLINK & text(or next HYPERLINK inside original span)
                if (isNeedToSplit)
                {
                    // HYPERLINK
                    iterSpan->m_strText = originalText.substr(posBlockStart, blockLen);

                    const size_t nextBlockLen = posOrigSpanEnd - posBlockEnd;
                    iterSpan = arrSpans.insert(iterSpan, *iterSpan);
                    // Go to next new span and correct text
                    iterSpan++;
                    iterSpan->m_strText = originalText.substr(posBlockEnd, nextBlockLen);
                    iterSpan->m_arrInteractive.clear();
                    // Return to current span
                    iterSpan--;
                }

                if (iterSpan != arrSpans.end() && iterInteractive != arrSplitedInteractive.end())
                    addHyperlinkToSpan(*iterSpan, *iterInteractive, oColor);
                else
                    break; //GZoabli_PhD.ppt

                if (posBlockEnd == iterRange->m_lEnd)
                {
                    iterRange++;
                    iterInteractive++;
                }
            }
            // Hyperlink will be written in the next iteration
            // case: text & HYPERLINK
            else
            {
                iterSpan = arrSpans.insert(iterSpan, *iterSpan);
                iterSpan->m_strText = originalText.substr(posBlockStart, blockLen);
                iterSpan->m_arrInteractive.clear();

                // Go to next new span and correct text
                iterSpan++;
                const size_t nextBlockLen = posOrigSpanEnd - posBlockEnd;
                iterSpan->m_strText = originalText.substr(posBlockEnd, nextBlockLen);
                iterSpan->m_arrInteractive.clear();
                // Return to current span
                iterSpan--;
            }
        }
        // skipping '\r'
        posOrigText++;
    }
}

void CRecordShapeContainer::addHyperlinkToSpan(CSpan &oSpan, const std::vector<CInteractiveInfo> &arrInteractive, const CColor &oColor)
{
    if (isRealHyperlink(arrInteractive))
    {
        oSpan.m_oRun.Color = oColor;
        oSpan.m_oRun.FontUnderline = (bool)true;
        oSpan.m_arrInteractive = arrInteractive;
    }
}

bool CRecordShapeContainer::isRealHyperlink(const std::vector<CInteractiveInfo> &arrInteractive)
{
    bool isReal = false;
    for (const auto& interInfo : arrInteractive)
    {
        switch (interInfo.m_lHyperlinkType)
        {
        case LT_Url:
            if (interInfo.m_strHyperlink.size())
                isReal = true;
            break;
        default:
            isReal = true;
            break;
        }
    }

    return isReal;
}



std::vector<std::vector<CInteractiveInfo> > CRecordShapeContainer::splitInteractive(const std::vector<CInteractiveInfo> &arrInteractive)
{
    std::vector<std::vector<CInteractiveInfo> > splited;
    std::vector<CInteractiveInfo> inserting;
    for (const auto& interactive : arrInteractive)
    {
        if (interactive.m_eActivation == interactive.click && !inserting.empty())
            splited.push_back(std::move(inserting));
        inserting.push_back(interactive);
    }
    if (!inserting.empty())
        splited.push_back(std::move(inserting));

    return splited;
}

void CRecordShapeContainer::ConvertInteractiveInfo(CInteractiveInfo &interactiveInfo, const CRecordMouseInteractiveInfoContainer *interactiveCont, CExMedia *pMapIDs)
{

    auto& interactiveAtom = interactiveCont->interactiveInfoAtom;
    interactiveInfo.m_bPresent = true;
    interactiveInfo.m_eActivation = interactiveCont->isOver;

    if (pMapIDs)
    {
        CExFilesInfo* pInfo1 = pMapIDs->LockAudioFromCollection(interactiveAtom.m_nSoundIdRef);
        if (NULL != pInfo1)
        {
            interactiveInfo.m_strAudioFileName = pInfo1->m_strFilePath;
            interactiveInfo.m_strAudioName = pInfo1->m_name;
        }
        CExFilesInfo* pInfo2 = pMapIDs->LockSlide(interactiveAtom.m_nExHyperlinkIdRef);
        if (NULL != pInfo2)
        {
            interactiveInfo.m_strHyperlink = pInfo2->m_strFilePath;
        } else {
            pInfo2 = pMapIDs->LockHyperlink(interactiveAtom.m_nExHyperlinkIdRef);
            if (NULL != pInfo2)
            {
                interactiveInfo.m_strHyperlink = pInfo2->m_strFilePath;
            }
        }
    }
    if (interactiveCont->macroNameAtom.is_init())
        interactiveInfo.m_macro = interactiveCont->macroNameAtom->m_strText;

    interactiveInfo.m_lType				= interactiveAtom.m_nAction;
    interactiveInfo.m_lOleVerb			= interactiveAtom.m_nOleVerb;
    interactiveInfo.m_lJump				= interactiveAtom.m_nJump;
    interactiveInfo.m_lHyperlinkType	= interactiveAtom.m_nHyperlinkType;

    interactiveInfo.m_bAnimated			= interactiveAtom.m_bAnimated;
    interactiveInfo.m_bStopSound		= interactiveAtom.m_bStopSound;
    interactiveInfo.m_bCustomShowReturn	= interactiveAtom.m_bCustomShowReturn;
    interactiveInfo.m_bVisited			= interactiveAtom.m_bVisited;

}

void CRecordShapeContainer::ConvertStyleTextProp9(CTextAttributesEx *pText)
{

    std::vector<CRecordOfficeArtClientData*> arrOfficeData;
    GetRecordsByType(&arrOfficeData, false, true);

    if (arrOfficeData.empty())
        return;

    auto pUnknownBinaryTag =arrOfficeData[0]->getProgTag(___PPT9);
    if (pUnknownBinaryTag == nullptr)
        return;
    auto progTag9 = dynamic_cast<CRecordPP9ShapeBinaryTagExtension*>(pUnknownBinaryTag->m_pTagContainer);
    if (progTag9 == nullptr)
        return;

    const auto& arrStyleTextProp9 = progTag9->m_styleTextPropAtom.m_rgStyleTextProp9;
    if (arrStyleTextProp9.empty())
        return;

    WORD pp9rt = 0;
    auto& arrPars  = pText->m_arParagraphs;
    for (auto& par : arrPars)
    {
        if (par.m_arSpans.empty())
            continue;

        if (par.m_arSpans[0].m_oRun.pp9rt.is_init())
            pp9rt = par.m_arSpans[0].m_oRun.pp9rt.get();

        if (pp9rt >= arrStyleTextProp9.size())
            continue;

        auto& prop9 = arrStyleTextProp9[pp9rt];

        if (prop9.m_pf9.m_optBulletAutoNumberScheme.is_init() &&
                prop9.m_pf9.m_optfBulletHasAutoNumber.get_value_or(false))
        {
            auto* pBuAutoNum = new CBulletAutoNum;
            pBuAutoNum->type = prop9.m_pf9.m_optBulletAutoNumberScheme->SchemeToStr();
            pBuAutoNum->startAt = prop9.m_pf9.m_optBulletAutoNumberScheme->m_nStartNum;

            par.m_oPFRun.bulletAutoNum.reset(pBuAutoNum);
        }
        if (prop9.m_pf9.m_optBulletBlipRef.is_init())
        {
            auto* pBuBlip = new CBulletBlip;
            if (prop9.m_pf9.m_optBulletBlipRef.IsInit())
                pBuBlip->bulletBlipRef = prop9.m_pf9.m_optBulletBlipRef.get();
            else
                pBuBlip->bulletBlipRef = -1;

            par.m_oPFRun.bulletBlip.reset(pBuBlip);
        }
    }

}

//void CRecordShapeContainer::ConvertExtention9(CElement* pElement)
//{
//    if (pElement == nullptr)
//        return;

//    std::vector<CRecordOfficeArtClientData*> arrOfficeData;
//    GetRecordsByType(&arrOfficeData, false, true);

//    if (arrOfficeData.empty())
//        return;

//    auto pUnknownBinaryTag =arrOfficeData[0]->getProgTag(___PPT9);
//    if (pUnknownBinaryTag == nullptr)
//        return;
//    auto progTag9 = dynamic_cast<CRecordPP9ShapeBinaryTagExtension*>(pUnknownBinaryTag->m_pTagContainer);
//    if (progTag9 == nullptr)
//        return;

//    const auto& arrStyleTextProp9 = progTag9->m_styleTextPropAtom.m_rgStyleTextProp9;

//    for (const auto& textProp9 : arrStyleTextProp9)
//    {
//        CBulletBlip buBlip;
//        if (textProp9.m_pf9.m_optBulletBlipRef.IsInit())
//            buBlip.bulletBlipRef = textProp9.m_pf9.m_optBulletBlipRef.get();
//        else
//            buBlip.bulletBlipRef = -1;

//        pElement->m_arrBlip.push_back(buBlip);
//    }

//}

//void CRecordGroupShapeContainer::ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
//{
//    CRecordsContainer::ReadFromStream(oHeader, pStream);

//}
