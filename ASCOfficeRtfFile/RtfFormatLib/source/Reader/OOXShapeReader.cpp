﻿/*
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
#include "OOXShapeReader.h"
#include "OOXTextItemReader.h"

#include "../../../../ASCOfficePPTXFile/Editor/Drawing/Shapes/Shape.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTree.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Shape.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SchemeClr.h"

#include "../../../../ASCOfficeOdfFile/src/odf/svg_parser.h"
#include <boost/algorithm/string.hpp>

#ifndef RGB
    #define RGB(r,g,b) ((_UINT32)(((BYTE)(r)|((_UINT16)((BYTE)(g))<<8))|(((_UINT32)(BYTE)(b))<<16)))
#endif

bool ParseVmlStyle(RtfShapePtr pShape, SimpleTypes::Vml::CCssProperty* prop)
{
	if (pShape == NULL)	return false;
	if (prop == NULL)	return false;

	switch(prop->get_Type())
	{
		case SimpleTypes::Vml::cssptUnknown : 
			break;
		case SimpleTypes::Vml::cssptFlip : 
			switch(pShape->m_bFlipH = prop->get_Value().eFlip)
			{
				case SimpleTypes::Vml::cssflipX:	pShape->m_bFlipH = true; break; 
				case SimpleTypes::Vml::cssflipY:	pShape->m_bFlipV = true; break;
				case SimpleTypes::Vml::cssflipXY:	pShape->m_bFlipH = true; pShape->m_bFlipV = true; break; 
				case SimpleTypes::Vml::cssflipYX :	pShape->m_bFlipH = true; pShape->m_bFlipV = true; break; 
                default: break;
			}
			break;
		case SimpleTypes::Vml::cssptWidth : 
			{
				if (prop->get_Value().oValue.eType == SimpleTypes::Vml::cssunitstypeUnits)
					pShape->m_nRelRight = (int)(20 * prop->get_Value().oValue.dValue);
				else
					pShape->m_nRelRight = (int)(prop->get_Value().oValue.dValue);

				pShape->m_nRight = pShape->m_nRelRight;
				
				if( PROP_DEF != pShape->m_nLeft )
					pShape->m_nRight += pShape->m_nLeft;
				if( PROP_DEF != pShape->m_nRelLeft)
					pShape->m_nRelRight += pShape->m_nRelLeft;
			}break;
		case SimpleTypes::Vml::cssptHeight : 
			{
				if (prop->get_Value().oValue.eType == SimpleTypes::Vml::cssunitstypeUnits)
					pShape->m_nRelBottom = (int)(20 * prop->get_Value().oValue.dValue); //pt
				else
					pShape->m_nRelBottom = (int)(prop->get_Value().oValue.dValue); //absolute

				pShape->m_nBottom  = pShape->m_nRelBottom;
				
				if( PROP_DEF != pShape->m_nTop )
					pShape->m_nBottom += pShape->m_nTop;
				if( PROP_DEF != pShape->m_nRelTop)
					pShape->m_nRelBottom += pShape->m_nRelTop;
			}break;
		case SimpleTypes::Vml::cssptLeft :
			{
				pShape->m_nRelLeft	= (int) prop->get_Value().oValue.dValue;	//absolute		
				
				if( PROP_DEF != pShape->m_nRelRight)
					pShape->m_nRelRight += pShape->m_nRelLeft;
			}break;
		case SimpleTypes::Vml::cssptTop : 
			{
				pShape->m_nRelTop = (int) prop->get_Value().oValue.dValue;	//absolute
				if( PROP_DEF != pShape->m_nRelBottom)
					pShape->m_nRelBottom += pShape->m_nRelTop;
			}break;
		case SimpleTypes::Vml::cssptMarginBottom: 
			{
				pShape->m_nBottom	=  (int)(20 * prop->get_Value().oValue.dValue );	
			}break;
		case SimpleTypes::Vml::cssptMarginLeft :
			{
				pShape->m_nLeft		=  (int)(20 * prop->get_Value().oValue.dValue );	
			}break;//pt tp twips
		case SimpleTypes::Vml::cssptMarginRight : 
			{
				pShape->m_nRight	=  (int)(20 * prop->get_Value().oValue.dValue );		
			}break;
		case SimpleTypes::Vml::cssptMarginTop:
			{
				pShape->m_nTop		=  (int)(20 * prop->get_Value().oValue.dValue );	
			}break;
		case SimpleTypes::Vml::cssptMsoPositionHorizontal: 
			{
				pShape->m_nPositionH = prop->get_Value().eMsoPosHor;	
			}break;
		case SimpleTypes::Vml::cssptMsoPositionHorizontalRelative  :
			{
				pShape->m_nPositionHRelative = prop->get_Value().eMsoPosHorRel;	
				switch(prop->get_Value().eMsoPosHorRel)
				{
				case SimpleTypes::Vml::cssmsoposhorrelMargin:	pShape->m_eXAnchor = RtfShape::ax_margin; break;
				case SimpleTypes::Vml::cssmsoposhorrelPage:		pShape->m_eXAnchor = RtfShape::ax_page; break;
				case SimpleTypes::Vml::cssmsoposhorrelText:
				case SimpleTypes::Vml::cssmsoposhorrelChar:		break;//inline	
				}
			}break;
		case SimpleTypes::Vml::cssptMsoPositionVertical: 
			{
				pShape->m_nPositionV = prop->get_Value().eMsoPosVer;		
			}break;
		case SimpleTypes::Vml::cssptMsoPositionVerticalRelative    :
			{
				pShape->m_nPositionVRelative = prop->get_Value().eMsoPosVerRel;		
				switch(prop->get_Value().eMsoPosVerRel)
				{
				case SimpleTypes::Vml::cssmsoposverrelMargin:	pShape->m_eYAnchor = RtfShape::ay_margin; break;
				case SimpleTypes::Vml::cssmsoposverrelPage:		pShape->m_eYAnchor = RtfShape::ay_page; break;
				case SimpleTypes::Vml::cssmsoposverrelText:
				case SimpleTypes::Vml::cssmsoposverrelLine:		break;//inline	
				}
			}break;
		case SimpleTypes::Vml::cssptMsoWrapDistanceBottom:
			{
				pShape->m_nWrapDistBottom =  (int)(20 * prop->get_Value().dValue );	
			}break;
		case SimpleTypes::Vml::cssptMsoWrapDistanceLeft : 
			{
				pShape->m_nWrapDistLeft = (int)(20 * prop->get_Value().dValue );		
			}break;
		case SimpleTypes::Vml::cssptMsoWrapDistanceRight:
			{
				pShape->m_nWrapDistRight = (int)(20 * prop->get_Value().dValue );	
			}break;
		case SimpleTypes::Vml::cssptMsoWrapDistanceTop : 
			{
				pShape->m_nWrapDistTop =  (int)(20 * prop->get_Value().dValue );		
			}break;
		case SimpleTypes::Vml::cssptMsoWrapEdited: 
			break;
		case SimpleTypes::Vml::cssptMsoWrapStyle: 
			if (prop->get_Value().eMsoWrapStyle == SimpleTypes::Vml::cssmsowrapstyleNone)
				pShape->m_nWrapType = 3;
			else if ( pShape->m_nZOrderRelative == PROP_DEF)
				pShape->m_nWrapType = 2;
			break;
		case SimpleTypes::Vml::cssptPosition: 
			break;
		case SimpleTypes::Vml::cssptRotation: 
			{
				pShape->m_nRotation = (int)(65536 * prop->get_Value().dValue);
				if(	PROP_DEF != pShape->m_nRelRight 
					||	PROP_DEF != pShape->m_nRelLeft 
					||	PROP_DEF != pShape->m_nRelTop 
					||	PROP_DEF != pShape->m_nRelBottom )
					pShape->m_nRelRotation = pShape->m_nRotation;
			}break;
		case SimpleTypes::Vml::cssptVisibility :  
			break;
		case SimpleTypes::Vml::cssptZIndex : 
			{
				pShape->m_eAnchorTypeShape	= RtfShape::st_anchor;

				int nValue = prop->get_Value().oZIndex.nOrder;
				pShape->m_nZOrder = nValue;
				if( nValue > 0 )
				{
					pShape->m_nWrapType			= 3;
					pShape->m_nZOrderRelative	= 0;
				}
				else if( nValue < 0 )
				{
					pShape->m_nWrapType			= 3;
					pShape->m_nZOrderRelative	= 1;
				}
			}break;
		case SimpleTypes::Vml::cssptFontFamily:
			{
				pShape->m_sGtextFont = std::wstring(prop->get_Value().wsValue);
			}break;
		case SimpleTypes::Vml::cssptFontSize:
			{
				pShape->m_nGtextSize = (int)prop->get_Value().oValue.dValue;
			}break;
		case SimpleTypes::Vml::cssptVTextAnchor:
			{
				pShape->m_nAnchorText		= prop->get_Value().eVTextAnchor;//совпдает
				pShape->m_bFitShapeToText	= 0;
			}break;
		case SimpleTypes::Vml::csspctMsoWidthPercent:
			{
				pShape->m_nPctWidth	= (int)prop->get_Value().dValue;
			}break;
		case SimpleTypes::Vml::csspctMsoHeightPercent:
			{
				pShape->m_nPctHeight	= (int)prop->get_Value().dValue;
			}break;
		case SimpleTypes::Vml::cssptVRotateLetters:
			{
				pShape->m_bGtextFVertical	= prop->get_Value().bValue;
			}break;
		case SimpleTypes::Vml::cssptVTextKern:
			{
				pShape->m_bGtextFKern		= prop->get_Value().bValue;
			}break;
		case SimpleTypes::Vml::cssptDirection                      : 			break;
		case SimpleTypes::Vml::cssptLayoutFlow                     : 			break;
		case SimpleTypes::Vml::cssptMsoDirectionAlt                : 			break;
		case SimpleTypes::Vml::cssptMsoFitShapeToText              : 			break;
		case SimpleTypes::Vml::cssptMsoFitTextToShape              : 			break;
		case SimpleTypes::Vml::cssptMsoLayoutFlowAlt               : 			break;
		case SimpleTypes::Vml::cssptMsoNextTextbox                 :			break;
		case SimpleTypes::Vml::cssptMsoRotate                      : 			break;
		case SimpleTypes::Vml::cssptMsoTextScale                   : 			break;
		case SimpleTypes::Vml::cssptFont                           :			break;
		case SimpleTypes::Vml::cssptFontStyle                      : 			break;
		case SimpleTypes::Vml::cssptFontVariant                    :			break;
		case SimpleTypes::Vml::cssptFontWeight                     :			break;
		case SimpleTypes::Vml::cssptMsoTextShadow                  : 			break;
		case SimpleTypes::Vml::cssptTextDecoration                 : 			break;
		case SimpleTypes::Vml::cssptVSameLetterHeights             : 			break;
		case SimpleTypes::Vml::cssptVTextAlign                     : 			break;
		case SimpleTypes::Vml::cssptVTextReverse                   : 			break;
		case SimpleTypes::Vml::cssptVTextSpacingMode               : 			break;
		case SimpleTypes::Vml::cssptVTextSpacing                   : 			break;		
        default: break;
    }
	return true;
}


OOXShapeReader::OOXShapeReader(OOX::WritingElementWithChilds<OOX::WritingElement> * elem)
{
    m_arrElement = elem;

    m_vmlElement	= dynamic_cast<OOX::Vml::CVmlCommonElements*>(elem);
	m_ooxShape		= dynamic_cast<PPTX::Logic::Shape*>(elem);

    if (m_vmlElement == NULL && m_ooxShape == NULL)
    {
        OOX::Logic::CPicture* pict = dynamic_cast<OOX::Logic::CPicture*>(elem);

        if (pict)
        {
                 if (pict->m_oShape.IsInit())           m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShape.GetPointer());
            else if (pict->m_oShapeArc.IsInit())        m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeArc.GetPointer());
            else if (pict->m_oShapeCurve.IsInit())      m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeCurve.GetPointer());
            else if (pict->m_oShapeLine.IsInit())       m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeLine.GetPointer());
            else if (pict->m_oShapeOval.IsInit())       m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeOval.GetPointer());
            else if (pict->m_oShapePolyLine.IsInit())   m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapePolyLine.GetPointer());
            else if (pict->m_oShapeRect.IsInit())       m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeRect.GetPointer());
            else if (pict->m_oShapeRoundRect.IsInit())  m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeRoundRect.GetPointer());
            else if (pict->m_oShapeType.IsInit())       m_vmlElement = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pict->m_oShapeType.GetPointer());
        }

    }
}

bool OOXShapeReader::ParseVmlChild( ReaderParameter oParam , RtfShapePtr& pOutput)
{
	if (m_arrElement == NULL) return false;

    for (std::vector<OOX::WritingElement*>::iterator it = m_arrElement->m_arrItems.begin(); it != m_arrElement->m_arrItems.end(); ++it)
	{
		if ((*it) == NULL) continue;

		switch((*it)->getType())
		{
			case OOX::et_v_fill:
			{
				OOX::Vml::CFill* fill = dynamic_cast<OOX::Vml::CFill*>(*it); 
				if (!fill) break;
				
				std::wstring srId = fill->m_sId.IsInit() ? fill->m_sId.get2() : L"" ;

				if (srId.empty())
                    srId = fill->m_rId.IsInit() ? fill->m_rId->GetValue() : L"" ;
				
				if (!srId.empty() && oParam.oReader->m_currentContainer)
				{        
					smart_ptr<OOX::File> oFile = oParam.oReader->m_currentContainer->Find(srId);
				
					if ( oFile.IsInit() && (OOX::FileTypes::Image == oFile->type()))
					{
						OOX::Image* pImage = (OOX::Image*)oFile.operator->();
						std::wstring sImagePath = pImage->filename().GetPath();

						pOutput->m_oPicture = RtfPicturePtr( new RtfPicture() );
						WriteDataToPicture( sImagePath, *pOutput->m_oPicture, oParam.oReader->m_sPath );

						pOutput->m_nFillType = 2;
					}
				}
				if (fill->m_oColor.IsInit())
					pOutput->m_nFillColor = (fill->m_oColor->Get_B() << 16) + (fill->m_oColor->Get_G() << 8) + fill->m_oColor->Get_R();

				if (fill->m_oColor2.IsInit())
				{
					pOutput->m_nFillColor2	= (fill->m_oColor2->Get_B() << 16) + (fill->m_oColor2->Get_G() << 8) + fill->m_oColor2->Get_R();
				}

				if (fill->m_oOpacity.IsInit())
					pOutput->m_nFillOpacity = fill->m_oOpacity->GetValue() * 100;

				switch(fill->m_oType.GetValue())
				{
					case SimpleTypes::filltypeBackground:		pOutput->m_nFillType = 9;	break;
					case SimpleTypes::filltypeFrame :			pOutput->m_nFillType = 3;	break;
					case SimpleTypes::filltypeGradient:			pOutput->m_nFillType = 4;	break;
					case SimpleTypes::filltypeGradientCenter:	pOutput->m_nFillType = 4;	break;
					case SimpleTypes::filltypeGradientRadial:	pOutput->m_nFillType = 6;	break;
					case SimpleTypes::filltypeGradientUnscaled:	pOutput->m_nFillType = 4;	break;
					case SimpleTypes::filltypePattern:			pOutput->m_nFillType = 1;	break;
					case SimpleTypes::filltypeTile:				pOutput->m_nFillType = 2;	break;
					case SimpleTypes::filltypeSolid:
					default:
						break;
				}
				if (fill->m_oAngle.IsInit())
				{
					pOutput->m_nFillAngle = fill->m_oAngle->GetValue();
					if (pOutput->m_nFillType == 4) 
						pOutput->m_nFillType = 7;
				}
				if (fill->m_oFocus.IsInit())
				{
					pOutput->m_nFillFocus = fill->m_oFocus->GetValue();
				}
				if (fill->m_oFocusPosition.IsInit())
				{
					if (fill->m_oFocusPosition->GetY() > 0.99 || fill->m_oFocusPosition->GetX() > 0.99)
						pOutput->m_nFillType = 5;

					int toBottom	= (int)((1. - fill->m_oFocusPosition->GetY())	* 65535);
					int toTop		= (int)((fill->m_oFocusPosition->GetY())		* 65535);
					int toRight		= (int)((1. - fill->m_oFocusPosition->GetX())	* 65535);
					int toLeft		= (int)((fill->m_oFocusPosition->GetX())		* 65535);

					if (toBottom > 0)	pOutput->m_nFillToBottom	= toBottom;
					if (toTop > 0)		pOutput->m_nFillToTop		= toTop;
					if (toRight > 0)	pOutput->m_nFillToRight		= toRight;
					if (toLeft > 0)		pOutput->m_nFillToLeft		= toLeft;
				}
			}break;
			case OOX::et_v_stroke:
			{
				OOX::Vml::CStroke* stroke = dynamic_cast<OOX::Vml::CStroke*>(*it); 
				if (!stroke) break;
				pOutput->m_nLineDashing = stroke->m_oDahsStyle.GetValue(); //совпадают значения

				if (stroke->m_oColor.IsInit())
					pOutput->m_nLineColor = (stroke->m_oColor->Get_B() << 16) + (stroke->m_oColor->Get_G() << 8) + stroke->m_oColor->Get_R();
			}break;
			case OOX::et_v_imagedata:
			{
				OOX::Vml::CImageData* image_data = dynamic_cast<OOX::Vml::CImageData*>(*it);
				if (!image_data) break;

				std::wstring srId = image_data->m_oId.IsInit() ? image_data->m_oId.get2() : L"" ;

				if (srId.empty())
                    srId = image_data->m_rId.IsInit() ? image_data->m_rId->GetValue() : L"" ;

				if (oParam.oReader->m_currentContainer)
				{        
					smart_ptr<OOX::File> oFile = oParam.oReader->m_currentContainer->Find(srId);
				
					if ( oFile.IsInit() && (OOX::FileTypes::Image == oFile->type()))
					{
						pOutput->m_oPicture	= RtfPicturePtr( new RtfPicture() );

						OOX::Image* pImage = (OOX::Image*)oFile.operator->();
						std::wstring sImagePath = pImage->filename().GetPath();
						
						WriteDataToPicture( sImagePath, *pOutput->m_oPicture, oParam.oReader->m_sPath );
					}
				}
				int nCropedWidthGoal = pOutput->m_oPicture->m_nWidthGoal;
				if( PROP_DEF != nCropedWidthGoal )
				{
					//делаем crop
					if( image_data->m_oCropLeft.IsInit() )
					{
						float nCropLeft = image_data->m_oCropLeft->GetValue();
						//pOutput->m_oPicture->m_nCropL = nCropLeft * pOutput->m_oPicture->m_nWidthGoal * pOutput->m_oPicture->m_nScaleX / 100;
						pOutput->m_oPicture->m_nCropL = (int)(nCropLeft * pOutput->m_oPicture->m_nWidthGoal);
						pOutput->m_nCropFromLeft = (int)(nCropLeft * 65536);
						nCropedWidthGoal -= pOutput->m_oPicture->m_nCropL;
					}
					if( image_data->m_oCropRight.IsInit())
					{
						float nCropRight =image_data->m_oCropRight->GetValue();
						//pOutput->m_oPicture->m_nCropR = nCropRight * pOutput->m_oPicture->m_nWidthGoal * pOutput->m_oPicture->m_nScaleX / 100;
						pOutput->m_oPicture->m_nCropR = (int)(nCropRight * pOutput->m_oPicture->m_nWidthGoal);
						pOutput->m_nCropFromRight = (int)(nCropRight * 65536);
						nCropedWidthGoal -= pOutput->m_oPicture->m_nCropR;
					}
				}
				int nCropedHeightGoal = pOutput->m_oPicture->m_nHeightGoal;
				if( PROP_DEF != nCropedHeightGoal )
				{
					if( image_data->m_oCropTop.IsInit() )
					{
						float nCropTop = image_data->m_oCropTop->GetValue();
						//pOutput->m_oPicture->m_nCropT = nCropTop * pOutput->m_oPicture->m_nHeightGoal * pOutput->m_oPicture->m_dScaleY / 100;
						pOutput->m_oPicture->m_nCropT = (int)(nCropTop * pOutput->m_oPicture->m_nHeightGoal);
						pOutput->m_nCropFromTop = (int)(nCropTop * 65536);
						nCropedHeightGoal -= pOutput->m_oPicture->m_nCropT;
					}
					if( image_data->m_oCropBottom.IsInit())
					{
						float nCropBottom = image_data->m_oCropBottom->GetValue();
						//pOutput->m_oPicture->m_nCropT = nCropTop * pOutput->m_oPicture->m_nHeightGoal * pOutput->m_oPicture->m_dScaleY / 100;
						pOutput->m_oPicture->m_nCropB = (int)(nCropBottom * pOutput->m_oPicture->m_nHeightGoal);
						pOutput->m_nCropFromBottom = (int)(nCropBottom * 65536);
						nCropedHeightGoal -= pOutput->m_oPicture->m_nCropB;
					}
				}
				//устанавливаем scale
				if( PROP_DEF != pOutput->m_nLeft && PROP_DEF != pOutput->m_nRight && PROP_DEF != nCropedWidthGoal && 0 != nCropedWidthGoal )
				{
					int nWidth = pOutput->m_nRight - pOutput->m_nLeft;
					double dNewScale = 100 * ( 1.0 * nWidth / nCropedWidthGoal );
					pOutput->m_oPicture->m_dScaleX = dNewScale;
				}
				if( PROP_DEF != pOutput->m_nTop && PROP_DEF != pOutput->m_nBottom && PROP_DEF != nCropedHeightGoal && 0 != nCropedHeightGoal )
				{
					int nHeight = pOutput->m_nBottom - pOutput->m_nTop;
					double dNewScale = 100 * ( 1.0 * nHeight / nCropedHeightGoal );
					pOutput->m_oPicture->m_dScaleY = dNewScale;
				}
			}break;
			case OOX::et_wd_wrap:
			{
				OOX::VmlWord::CWrap *wrap = dynamic_cast<OOX::VmlWord::CWrap*>(*it);

				if (wrap->m_oType.IsInit() && pOutput->m_nZOrderRelative == PROP_DEF)
				{
					switch(wrap->m_oType->GetValue())
					{
					case SimpleTypes::wraptypeNone :	pOutput->m_nWrapType = 3;	break;
					case SimpleTypes::wraptypeSquare:	pOutput->m_nWrapType = 2;	break;
					case SimpleTypes::wraptypeThrough:	pOutput->m_nWrapType = 5;	break;
					case SimpleTypes::wraptypeTight:	pOutput->m_nWrapType = 4;	break;
					case SimpleTypes::wraptypeTopAndBottom:	pOutput->m_nWrapType = 1;	break;
					}
				}
				if (wrap->m_oSide.IsInit() && pOutput->m_nZOrderRelative == PROP_DEF)
				{
					switch(wrap->m_oSide->GetValue())
					{
					case SimpleTypes::wrapsideBoth:    pOutput->m_nWrapSideType = 0; break;		
					case SimpleTypes::wrapsideLargest: pOutput->m_nWrapSideType = 3; break;					
					case SimpleTypes::wrapsideLeft:    pOutput->m_nWrapSideType = 1; break;					
					case SimpleTypes::wrapsideRight:   pOutput->m_nWrapSideType = 2; break;		
					}
				}
			}break;
			case OOX::et_v_textbox:
			{
				OOX::Vml::CTextbox *text_box= dynamic_cast<OOX::Vml::CTextbox*>(*it);

				if ((text_box) && (text_box->m_oTxtbxContent.IsInit()))
				{
					OOXTextItemReader oTextItemReader;

                    for (std::vector<OOX::WritingElement*>::iterator	it = text_box->m_oTxtbxContent->m_arrItems.begin();
																	it != text_box->m_oTxtbxContent->m_arrItems.end(); ++it)
					{
						oTextItemReader.Parse( *it, oParam );
					}

					if( oTextItemReader.m_oTextItems->GetCount() > 0 )
						pOutput->m_aTextItems = oTextItemReader.m_oTextItems;
				}
			}break;
			case OOX::et_v_textpath:
			{
				OOX::Vml::CTextPath *text_path= dynamic_cast<OOX::Vml::CTextPath*>(*it);
				if (text_path)
				{
					pOutput->m_bGtext = 1;
					if (text_path->m_sString.IsInit())
					{
						pOutput->m_sGtextUNICODE	= text_path->m_sString.get();
					}
					if ((text_path->m_oFitPath.IsInit()) && (text_path->m_oFitPath->GetValue()))
					{
						pOutput->m_bGtextFStretch	= 1;
						pOutput->m_bGtextFBestFit	= 1;
						pOutput->m_bGtextFShrinkFit	= 1;
					}
					if (text_path->m_oTrim.IsInit())
					{
					}
					if (text_path->m_oStyle.IsInit())
					{
						ParseVmlStyles( pOutput, text_path->m_oStyle->m_arrProperties );
					}
				}

			}break;
			case OOX::et_o_signatureline:
			{
				OOX::VmlOffice::CSignatureLine *signature = dynamic_cast<OOX::VmlOffice::CSignatureLine*>(*it);
				if ( signature )
				{
					if (signature->m_oIsSignatureLine.IsInit() && signature->m_oIsSignatureLine->ToBool())
						pOutput->m_bIsSignatureLine = 1;

					if (signature->m_oAllowComments.IsInit() && signature->m_oAllowComments->ToBool())
						pOutput->m_bSigSetupAllowComments = 1;

					if (signature->m_oProvId.IsInit())
						pOutput->m_sSigSetupProvId = signature->m_oProvId->ToString();

					if (signature->m_oId.IsInit())
						pOutput->m_sSigSetupId = signature->m_oId->ToString();

					if (signature->m_sSuggestedSigner.IsInit())
						pOutput->m_sSigSetupSuggSigner = signature->m_sSuggestedSigner.get();
					
					if (signature->m_sSuggestedSigner2.IsInit())
						pOutput->m_sSigSetupSuggSigner2 = signature->m_sSuggestedSigner2.get();
					
					if (signature->m_sSuggestedSignerEmail.IsInit())
						pOutput->m_sSigSetupSuggSignerEmail = signature->m_sSuggestedSignerEmail.get();

			//nullable<std::wstring>							m_sAddXml;
			//nullable<SimpleTypes::CExt<>>						m_oExt;
			//SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>	m_oShowSignDate;
			//nullable<std::wstring>								m_sSigningInstructions;
			//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>	m_oSigningInstructionsSet;
			//nullable<std::wstring>								m_sSigProvUrl;

				}
			}break;
            default: break;
        }
	}
	//проверяем на inline
	if((PROP_DEF == pOutput->m_nLeft/* || 0 == pOutput->m_nLeft */) && ( PROP_DEF == pOutput->m_nTop/* || 0 == pOutput->m_nTop */) && 
		PROP_DEF == pOutput->m_nPositionH && PROP_DEF == pOutput->m_nPositionV )
	{
		pOutput->m_eAnchorTypeShape = RtfShape::st_inline;
	}

	if( (PROP_DEF == pOutput->m_nLeft || 0 == pOutput->m_nLeft ) && ( PROP_DEF == pOutput->m_nTop || 0 == pOutput->m_nTop ) && PROP_DEF == pOutput->m_nPositionH && PROP_DEF == pOutput->m_nPositionV )
	{
		pOutput->m_nLeft = 0;
		pOutput->m_nTop = 0;
		pOutput->m_nPositionHRelative = 3;
		pOutput->m_nPositionVRelative = 3;
	}
	return true;
}
//bool OOXShapeReader::Parse(ReaderParameter oParam, int indexSchemeColor, BYTE& ucA, BYTE& ucG, BYTE& ucB, BYTE& ucR)
//{
//	OOX::CTheme * theme= oParam.oDocx->GetTheme();
//	if (!theme)return false;
//
//	bool result = false;
//
//	switch(indexSchemeColor)
//	{
//		case SimpleTypes::shemecolorvalLt1:
//		case SimpleTypes::shemecolorvalBg1:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oLt1.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalLt2:
//		case SimpleTypes::shemecolorvalBg2:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oLt2.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalDk1:
//		case SimpleTypes::shemecolorvalTx1:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oDk1.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalDk2:
//		case SimpleTypes::shemecolorvalTx2:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oDk2.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalAccent1:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent1.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalAccent2:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent2.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalAccent3:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent3.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalAccent4:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent4.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalAccent5:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent5.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalAccent6:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent6.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalFolHlink:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oFolHlink.tryGetRgb(ucR, ucG, ucB, ucA); break;
//		case SimpleTypes::shemecolorvalHlink:
//			result = theme->m_oThemeElements.m_oClrScheme.m_oHlink.tryGetRgb(ucR, ucG, ucB, ucA); break;
//        default: break;
//    }
//	return result;
//}
//void OOXShapeReader::Parse(ReaderParameter oParam, OOX::Drawing::CSchemeColor *oox_ShemeClr, unsigned int & nColor, _CP_OPT(double) &opacity)
//{
//	if (!oox_ShemeClr)return;
//
//	int theme_ind = oox_ShemeClr->m_oVal.GetValue();
//
//	BYTE ucA = 0, ucG = 0, ucB = 0, ucR = 0;
//	bool result = Parse(oParam, theme_ind, ucA, ucG, ucB, ucR);
//
//	if (result == true)
//	{
//		oox_ShemeClr->SetRGBA(ucR, ucG, ucB, ucA);
//		oox_ShemeClr->GetRGBA(ucR, ucG, ucB, ucA);
//
//		nColor = RGB(ucR, ucG, ucB);
//		if ( ucA != 0xff ) opacity = (ucA / 255.) * 100.;
//	}
//}
//
void OOXShapeReader::Parse(ReaderParameter oParam, PPTX::Logic::ColorBase *oox_color, unsigned int & nColor , _CP_OPT(double) &opacity)
{
	if (!oox_color) return;

	nColor = oox_color->GetARGB(0);
	BYTE alpha = nColor >> 24;
	if (alpha != 0xff)
		opacity = alpha;
	nColor = nColor & 0xff000000;
	//switch( oox_color->m_eType )
	//{
	//	case OOX::Drawing::colorSheme:	Parse(oParam, &oox_color->m_oShemeClr,	nColor, opacity);		break;
	//	case OOX::Drawing::colorHsl:	Parse(oParam, &oox_color->m_oHslClr,	nColor, opacity);		break;
	//	case OOX::Drawing::colorPrst:	Parse(oParam, &oox_color->m_oPrstClr,	nColor, opacity);		break;
	//	case OOX::Drawing::colorScRgb:	Parse(oParam, &oox_color->m_oScrgbClr,	nColor, opacity);		break;
	//	case OOX::Drawing::colorSRgb:	Parse(oParam, &oox_color->m_oSrgbClr,	nColor, opacity);		break;
	//	case OOX::Drawing::colorSys:	Parse(oParam, &oox_color->m_oSysClr,	nColor, opacity);		break;		
 //       default: break;
 //   }
}
//void OOXShapeReader::Parse(ReaderParameter oParam, OOX::Drawing::Colors::CColorTransform *oox_Clr, unsigned int & nColor, _CP_OPT(double) &opacity)
//{
//	if (!oox_Clr)return;
//	BYTE ucA=0, ucG=0, ucB=0, ucR =0;
//	oox_Clr->GetRGBA(ucR, ucG, ucB, ucA);
//
//	nColor = RGB(ucR,  ucG, ucB);
//	if (ucA !=255)opacity = (ucA/255.)* 100.;
//}
void OOXShapeReader::Parse(ReaderParameter oParam, PPTX::Logic::SolidFill *oox_solid_fill, unsigned int & nColor , _CP_OPT(double) &opacity)
{
	if (!oox_solid_fill) return;
	Parse(oParam, oox_solid_fill->Color.Color.operator ->(), nColor, opacity);
	//switch( oox_solid_fill->m_eType )
	//{
	//	case OOX::Drawing::colorSheme:	Parse(oParam, &oox_solid_fill->m_oShemeClr,	nColor, opacity);		break;
	//	case OOX::Drawing::colorHsl:	Parse(oParam, &oox_solid_fill->m_oHslClr,	nColor, opacity);		break;
	//	case OOX::Drawing::colorPrst:	Parse(oParam, &oox_solid_fill->m_oPrstClr,	nColor, opacity);		break;
	//	case OOX::Drawing::colorScRgb:	Parse(oParam, &oox_solid_fill->m_oScrgbClr,	nColor, opacity);		break;
	//	case OOX::Drawing::colorSRgb:	Parse(oParam, &oox_solid_fill->m_oSrgbClr,	nColor, opacity);		break;
	//	case OOX::Drawing::colorSys:	Parse(oParam, &oox_solid_fill->m_oSysClr,	nColor, opacity);		break;		
 //       default: break;
 //   }
}
//-----------------------------------------------------------------------------------------------------------------
void OOXShapeReader::Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::SolidFill *oox_solid_fill, std::wstring *change_sheme_color)
{
	if (!oox_solid_fill)return;

	unsigned int nColor = 0xffffff; //white
	_CP_OPT(double) opacity;
	
	if (change_sheme_color && oox_solid_fill->Color.getType() == OOX::et_a_schemeClr)
	{
		//oox_solid_fill->m_oShemeClr.m_oVal.FromString(*change_sheme_color);
	}
	
	Parse(oParam, oox_solid_fill, nColor, opacity);

	pOutput->m_nFillColor = nColor;

	if (opacity)
	{
		pOutput->m_nFillOpacity = *opacity;
	}
}
bool OOXShapeReader::Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::BlipFill *oox_bitmap_fill)
{
	if (oox_bitmap_fill == NULL)return false;

	if (!pOutput->m_oPicture)
		pOutput->m_oPicture = RtfPicturePtr( new RtfPicture() );

	bool result = false;
	
	if (oox_bitmap_fill->blip.IsInit())
	{
		std::wstring sID;

		if (oox_bitmap_fill->blip->embed.IsInit() && oParam.oReader->m_currentContainer)
		{
			sID = oox_bitmap_fill->blip->embed->get();
			smart_ptr<OOX::File> oFile = oParam.oReader->m_currentContainer->Find(sID);
			
			if ( oFile.IsInit() && (OOX::FileTypes::Image == oFile->type()))
			{
				OOX::Image* pImage = (OOX::Image*)oFile.operator->();

				std::wstring sImagePath = pImage->filename().GetPath();
				result = WriteDataToPicture( sImagePath, *pOutput->m_oPicture, oParam.oReader->m_sPath);
			}
		}
		else if (oox_bitmap_fill->blip->link.IsInit())
		{
			sID = oox_bitmap_fill->blip->link->get();
			//...
		}
		//for (size_t i = 0 ; i < oox_bitmap_fill->m_oBlip->m_arrEffects.size(); i++)
		//	convert(oox_bitmap_fill->m_oBlip->m_arrEffects[i]);
	}
	if (oox_bitmap_fill->srcRect.IsInit())//часть изображения
	{
		int nCropedWidthGoal = pOutput->m_oPicture->m_nWidthGoal;
		if( PROP_DEF != nCropedWidthGoal )
		{
			//делаем crop
			if (oox_bitmap_fill->srcRect->l.IsInit())
			{
				double nCropLeft = XmlUtils::GetInteger( oox_bitmap_fill->srcRect->l.get()) / 100. ;
				//pOutput->m_oPicture->m_nCropL = nCropLeft * pOutput->m_oPicture->m_nWidthGoal * pOutput->m_oPicture->m_nScaleX / 100;
				pOutput->m_oPicture->m_nCropL = (int)(nCropLeft * pOutput->m_oPicture->m_nWidthGoal);
				pOutput->m_nCropFromLeft = (int)(nCropLeft* 65536);
				nCropedWidthGoal -= pOutput->m_oPicture->m_nCropL;
			}

			if (oox_bitmap_fill->srcRect->r.IsInit())
			{
				double nCropRight = XmlUtils::GetInteger( oox_bitmap_fill->srcRect->r.get()) / 100. ;
				//pOutput->m_oPicture->m_nCropR = nCropRight * pOutput->m_oPicture->m_nWidthGoal * pOutput->m_oPicture->m_nScaleX / 100;
				pOutput->m_oPicture->m_nCropR = (int)(nCropRight * pOutput->m_oPicture->m_nWidthGoal);
				pOutput->m_nCropFromRight = (int)(nCropRight * 65536);
				nCropedWidthGoal -= pOutput->m_oPicture->m_nCropR;
			}
		}
		int nCropedHeightGoal = pOutput->m_oPicture->m_nHeightGoal;
		if( PROP_DEF != nCropedHeightGoal )
		{
			if (oox_bitmap_fill->srcRect->t.IsInit())
			{
				double nCropTop = XmlUtils::GetInteger( oox_bitmap_fill->srcRect->t.get()) / 100. ;
				//pOutput->m_oPicture->m_nCropT = nCropTop * pOutput->m_oPicture->m_nHeightGoal * pOutput->m_oPicture->m_dScaleY / 100;
				pOutput->m_oPicture->m_nCropT = (int)(nCropTop * pOutput->m_oPicture->m_nHeightGoal);
				pOutput->m_nCropFromTop = (int)(nCropTop * 65536);
				nCropedHeightGoal -= pOutput->m_oPicture->m_nCropT;
			}

			if (oox_bitmap_fill->srcRect->b.IsInit())
			{
				double nCropBottom = XmlUtils::GetInteger( oox_bitmap_fill->srcRect->b.get()) / 100. ;
				//pOutput->m_oPicture->m_nCropT = nCropTop * pOutput->m_oPicture->m_nHeightGoal * pOutput->m_oPicture->m_dScaleY / 100;
				pOutput->m_oPicture->m_nCropB = (int)(nCropBottom * pOutput->m_oPicture->m_nHeightGoal);
				pOutput->m_nCropFromBottom = (int)(nCropBottom * 65536);
				nCropedHeightGoal -= pOutput->m_oPicture->m_nCropB;
			}
		}
		//устанавливаем scale
		if( PROP_DEF != pOutput->m_nLeft && PROP_DEF != pOutput->m_nRight && PROP_DEF != nCropedWidthGoal && 0 != nCropedWidthGoal )
		{
			int nWidth = pOutput->m_nRight - pOutput->m_nLeft;
			double dNewScale = 100 * ( 1.0 * nWidth / nCropedWidthGoal );
			pOutput->m_oPicture->m_dScaleX = dNewScale;
		}
		if( PROP_DEF != pOutput->m_nTop && PROP_DEF != pOutput->m_nBottom && PROP_DEF != nCropedHeightGoal && 0 != nCropedHeightGoal )
		{
			int nHeight = pOutput->m_nBottom - pOutput->m_nTop;
			double dNewScale = 100 * ( 1.0 * nHeight / nCropedHeightGoal );
			pOutput->m_oPicture->m_dScaleY = dNewScale;
		}
		//int w = pOutput->m_oPicture->m_nWidthGoal * 4 / 3;
		//int h = pOutput->m_oPicture->m_nHeightGoal * 4 / 3;
		//pOutput->m_oPicture->m_nCropL = (int)(oox_bitmap_fill->m_oSrcRect->m_oL.GetValue() / 10. * w) ;
		//pOutput->m_oPicture->m_nCropT = (int)(oox_bitmap_fill->m_oSrcRect->m_oT.GetValue() / 10. * h) ;
		//pOutput->m_oPicture->m_nCropR = (int)(oox_bitmap_fill->m_oSrcRect->m_oR.GetValue() / 10. * w) ;
		//pOutput->m_oPicture->m_nCropB = (int)(oox_bitmap_fill->m_oSrcRect->m_oB.GetValue() / 10. * h) ;
	}
	pOutput->m_nFillType = 3;
	if (oox_bitmap_fill->tile.IsInit())
	{
		pOutput->m_nFillType = 2;
	}
	if (oox_bitmap_fill->stretch.IsInit())
	{
		pOutput->m_nFillType = 3;
	}
	return result;
}

void OOXShapeReader::Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::GradFill *oox_grad_fill, std::wstring *change_sheme_color)
{
	if (!oox_grad_fill)return;

	pOutput->m_nFillType	= 4;
	pOutput->m_nFillFocus	= 100;

	if (oox_grad_fill->lin.IsInit() && oox_grad_fill->lin->ang.IsInit())
	{
		pOutput->m_nFillAngle = oox_grad_fill->lin->ang.get();
	}
	if (oox_grad_fill->path.IsInit())
	{
		switch(oox_grad_fill->path->path->GetBYTECode())
		{
			case SimpleTypes::pathshadetypeCircle:	
				pOutput->m_nFillFocus	= 50;
				pOutput->m_nFillType	= 5;			
				break;
			case SimpleTypes::pathshadetypeRect:	pOutput->m_nFillType = 4;		break;
			case SimpleTypes::pathshadetypeShape:	pOutput->m_nFillType = 6;		break;
		}	
		if (oox_grad_fill->path->rect.IsInit())
		{
			pOutput->m_nFillToBottom	= XmlUtils::GetInteger(oox_grad_fill->path->rect->b.get());
			pOutput->m_nFillToTop		= XmlUtils::GetInteger(oox_grad_fill->path->rect->t.get());
			pOutput->m_nFillToRight		= XmlUtils::GetInteger(oox_grad_fill->path->rect->r.get());
			pOutput->m_nFillToLeft		= XmlUtils::GetInteger(oox_grad_fill->path->rect->l.get());
		}
	}	

	if (oox_grad_fill->GsLst.size() > 1)
	{
		bool bColorsSet = false;
		if (oox_grad_fill->GsLst.size() > 2)
		{
			pOutput->m_nFillType		= 7;
			pOutput->m_nFillShadeType	= 0;
			bColorsSet					= true;
		}
		
		for (size_t i = 0; i < oox_grad_fill->GsLst.size(); i++)
		{
			unsigned int hexColor;
			_CP_OPT(double) opacity;
	
			if (change_sheme_color && oox_grad_fill->GsLst[i].color.getType() == OOX::et_a_schemeClr)
			{
				//oox_grad_fill->GsLst[i]->m_oShemeClr.m_oVal.FromString(*change_sheme_color);
			}
			Parse(oParam, oox_grad_fill->GsLst[i].color.Color.operator->(), hexColor, opacity);

			if (i == 0)
			{
				pOutput->m_nFillColor = hexColor;
				if (opacity)
					pOutput->m_nFillOpacity = (int)(*opacity * 100);		
			}
			else if (i == oox_grad_fill->GsLst.size() - 1 && i > 0)
			{
				pOutput->m_nFillColor2 = hexColor;
				if (opacity)
					pOutput->m_nFillOpacity2 = (int)(*opacity * 100);		
			}

			if (bColorsSet)
			{
				pOutput->m_aFillShadeColors.push_back(std::make_pair((int)hexColor, oox_grad_fill->GsLst[i].pos));
			}
		}
	}
}
void OOXShapeReader::Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::PattFill *oox_pattern_fill, std::wstring *change_sheme_color)
{
	if (!oox_pattern_fill)return;

}


void OOXShapeReader::Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::Ln *oox_line_prop, std::wstring *change_sheme_color)
{
	if (!oox_line_prop)return;

	pOutput->m_bLine = true;
	
	if (oox_line_prop->Fill.is_init())
	{
		NSCommon::smart_ptr<PPTX::Logic::SolidFill> fill = oox_line_prop->Fill.Fill.smart_dynamic_cast<PPTX::Logic::SolidFill>();
		NSCommon::smart_ptr<PPTX::Logic::NoFill> no_fill = oox_line_prop->Fill.Fill.smart_dynamic_cast<PPTX::Logic::NoFill>();
		if (fill.IsInit())
		{
			if (change_sheme_color && fill->Color.getType() == OOX::et_a_schemeClr)
			{
				//fill->Color.FromString(*change_sheme_color);
			}			
			unsigned int nColor = 0; //black
			_CP_OPT(double) opacity;
			
			Parse(oParam, fill.operator->(), nColor, opacity);
			pOutput->m_nLineColor = nColor;
		}
		else if (no_fill.IsInit())
		{
			pOutput->m_bLine = false;
		}
	}
	if (oox_line_prop->w.IsInit())
	{
		pOutput->m_nLineWidth = oox_line_prop->w.get();
	}
	if (oox_line_prop->headEnd.IsInit())
	{
		if (oox_line_prop->headEnd->len.IsInit())	pOutput->m_nLineEndArrowLength	= oox_line_prop->headEnd->len->GetBYTECode();
		if (oox_line_prop->headEnd->type.IsInit())	pOutput->m_nLineEndArrow		= oox_line_prop->headEnd->type->GetBYTECode();
		if (oox_line_prop->headEnd->w.IsInit())		pOutput->m_nLineEndArrowWidth	= oox_line_prop->headEnd->w->GetBYTECode();
	}
	if (oox_line_prop->tailEnd.IsInit())
	{
		if (oox_line_prop->tailEnd->len.IsInit())	pOutput->m_nLineStartArrowLength	= oox_line_prop->tailEnd->len->GetBYTECode();
		if (oox_line_prop->tailEnd->type.IsInit())	pOutput->m_nLineStartArrow			= oox_line_prop->tailEnd->type->GetBYTECode();
		if (oox_line_prop->tailEnd->w.IsInit())		pOutput->m_nLineStartArrowWidth		= oox_line_prop->tailEnd->w->GetBYTECode();
	}

	if (oox_line_prop->prstDash.IsInit() && oox_line_prop->prstDash->val.IsInit())
	{
		switch(oox_line_prop->prstDash->val->GetBYTECode())
		{
			case SimpleTypes::presetlinedashvalDash			:	pOutput->m_nLineDashing = 6; break;
			case SimpleTypes::presetlinedashvalDashDot		:	pOutput->m_nLineDashing = 8; break;
			case SimpleTypes::presetlinedashvalDot			:	pOutput->m_nLineDashing = 5; break;
			case SimpleTypes::presetlinedashvalLgDash 		:	pOutput->m_nLineDashing = 7; break;
			case SimpleTypes::presetlinedashvalLgDashDot	:	pOutput->m_nLineDashing = 9; break;
			case SimpleTypes::presetlinedashvalLgDashDotDot	:	pOutput->m_nLineDashing = 10; break;
			case SimpleTypes::presetlinedashvalSysDash		:	pOutput->m_nLineDashing = 1; break;
			case SimpleTypes::presetlinedashvalSysDashDot	:	pOutput->m_nLineDashing = 3; break;
			case SimpleTypes::presetlinedashvalSysDashDotDot:	pOutput->m_nLineDashing = 4; break;
			case SimpleTypes::presetlinedashvalSysDot		:	pOutput->m_nLineDashing = 2; break;
			case SimpleTypes::presetlinedashvalSolid		:	break;
            default: break;
        }
	}
	//nullable<SimpleTypes::CPenAlignment<>>            m_oAlgn;
	//nullable<SimpleTypes::CLineCap<>>                 m_oCap;
	//nullable<SimpleTypes::CCompoundLine<>>            m_oCmpd;


	//ELineJoinType                                     m_eJoinType;   // Тип соединения линий
	//nullable<OOX::Drawing::CLineJoinBevel>            m_oBevel;
	//nullable<OOX::Drawing::CLineJoinMiterProperties>  m_oMiter;
	//nullable<OOX::Drawing::CLineJoinRound>            m_oRound;
}
void OOXShapeReader::Parse( ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::StyleRef *style_ref, int type)
{
	if (!style_ref) return;
	if (style_ref->idx.IsInit() == false) return;

	int fmt_index = style_ref->idx.get() -1;

	if (!oParam.oDocx->m_pTheme || fmt_index <0) return;

	if (style_ref->Color.is<PPTX::Logic::SchemeClr>() == false) return;

	PPTX::Logic::SchemeClr & schemeClr = style_ref->Color.as<PPTX::Logic::SchemeClr>();

	std::wstring color = schemeClr.val.get();
	
	if (type == 1)
	{
		if (fmt_index < 1000 && fmt_index < oParam.oDocx->m_pTheme->themeElements.fmtScheme.fillStyleLst.size()) 
		{
			Parse(oParam, pOutput, &oParam.oDocx->m_pTheme->themeElements.fmtScheme.fillStyleLst[fmt_index], &color);
		}
		else if (fmt_index > 1000 && ((fmt_index-1000) < oParam.oDocx->m_pTheme->themeElements.fmtScheme.bgFillStyleLst.size()))
		{
			fmt_index -= 1000; 

			Parse(oParam, pOutput, &oParam.oDocx->m_pTheme->themeElements.fmtScheme.bgFillStyleLst[fmt_index], &color);	
		}
	}

	if (type == 2 && fmt_index < oParam.oDocx->m_pTheme->themeElements.fmtScheme.lnStyleLst.size())
	{
		Parse(oParam, pOutput, &oParam.oDocx->m_pTheme->themeElements.fmtScheme.lnStyleLst[fmt_index], &color);
	}

	//if (style_matrix_ref->getType() == OOX::et_a_effectRef && fmt_index < theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle.size())
	//{
	//	if (theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle[fmt_index])
	//	{
	//		Parse(oParam, theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle[fmt_index]->m_oEffectList.GetPointer(), &color);
	//	}
	//	//todooo
	//	//Parse(theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle[fmt_index].m_oEffectDag.GetPointer(), &color);
	//	//Parse(theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle[fmt_index].m_oScene3D.GetPointer(), &color);
	//	//Parse(theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle[fmt_index].m_oSp3D.GetPointer(), &color);
	//}
}
//-----------------------------------------------------------------------------------------------------------------------------
void OOXShapeReader::Parse( ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::Xfrm *xfrm)
{
	if (!xfrm) return;
	
	double rot = xfrm->rot.IsInit() ? xfrm->rot.get() / 60000. : 0;
	if (rot > 0.01)
		pOutput->m_nRotation = rot * 65535;

	if (xfrm->flipH.get_value_or(false))	pOutput->m_bFlipH = 1;	
	if (xfrm->flipV.get_value_or(false))	pOutput->m_bFlipV = 1;

	if (pOutput->m_bInGroup)
	{
		if (xfrm->offX.IsInit() && xfrm->offY.IsInit())
		{
			pOutput->m_nRelLeft	= (int)xfrm->offX.get();
			pOutput->m_nRelTop	= (int)xfrm->offY.get();
		}
		else
		{
			pOutput->m_nRelLeft	= 0;
			pOutput->m_nRelTop	= 0;
		}
		if (xfrm->extX.IsInit() && xfrm->extY.IsInit())
		{
			pOutput->m_nRelRight	= (int)pOutput->m_nRelLeft + xfrm->extX.get();
			pOutput->m_nRelBottom	= (int)pOutput->m_nRelTop + xfrm->extY.get();
		}
		else
		{
			pOutput->m_nRelRight	= 0;
			pOutput->m_nRelBottom	= 0;
		}
		pOutput->m_nRelRotation = pOutput->m_nRotation;
	}
}

bool OOXShapeReader::ParseShape( ReaderParameter oParam, RtfShapePtr& pOutput)
{
	PPTX::Logic::Shape	* ooxShape	= dynamic_cast<PPTX::Logic::Shape*>	(m_ooxShape);
	
	if (!ooxShape) return false;
	
	if (ooxShape->oTextBoxBodyPr.IsInit())
	{
		if (ooxShape->oTextBoxBodyPr->fromWordArt.get_value_or(false))
		{
			pOutput->m_bGtext = 1;
			if (ooxShape->oTextBoxBodyPr->prstTxWarp.IsInit())
			{
				SimpleTypes::ETextShapeType type = (SimpleTypes::ETextShapeType)ooxShape->oTextBoxBodyPr->prstTxWarp->prst.GetBYTECode();

				pOutput->m_nShapeType	= OOX::PrstTx2VmlShapeType(type);
			}
			else
				pOutput->m_nShapeType	= SimpleTypes::Vml::sptTextPlainText;

			//int wordart_type = OoxConverter::Parse(m_ooxShape->m_oTxBodyProperties->m_oPrstTxWrap.GetPointer());
			//if (wordart_type >=0)type = wordart_type;
		}
	}	
	
	std::wstring strXml;
	if (ooxShape->spPr.Geometry.getType() == OOX::et_a_custGeom)
	{
		PPTX::Logic::CustGeom& geometry = ooxShape->spPr.Geometry.as<PPTX::Logic::CustGeom>();
		pOutput->m_nShapeType = SimpleTypes::Vml::sptNotPrimitive;
		strXml = geometry.toXML();
	}
	if (ooxShape->spPr.Geometry.getType() == OOX::et_a_prstGeom)
	{
		PPTX::Logic::PrstGeom& geometry = ooxShape->spPr.Geometry.as<PPTX::Logic::PrstGeom>();
		
		SimpleTypes::CShapeType<> prst_type(geometry.prst.get());		
		SimpleTypes::EShapeType type = prst_type.GetValue();
			
		pOutput->m_nShapeType = OOX::PrstGeom2VmlShapeType(type);
		if (pOutput->m_nShapeType == SimpleTypes::Vml::sptNotPrimitive)
			strXml = geometry.prst.get();
	}
	if (pOutput->m_nShapeType == SimpleTypes::Vml::sptNotPrimitive && !strXml.empty())
	{
		XmlUtils::CXmlNode xmlNode;
		xmlNode.FromXmlString(strXml);
		PPTX::Logic::Geometry geom(xmlNode);

		std::wstring strVmlPath, strVmlRect;

		LONG lW = 0, lH = 0;

		//if ((ooxShape->spPr.xfrm->IsInit()) && (ooxShape->spPr.xfrm->m_oExt.IsInit()))
		//{
		//	lW = ooxShape->spPr.xfrm->m_oExt->m_oCx.GetValue();
		//	lH = ooxShape->spPr.xfrm->m_oExt->m_oCy.GetValue();
		//}

		COOXToVMLGeometry *renderer = new COOXToVMLGeometry();
		geom.ConvertToCustomVML(renderer, strVmlPath, strVmlRect, lW, lH);
		delete renderer;

		if (!strVmlPath.empty())
			ParseVmlPath(pOutput, strVmlPath);

		pOutput->m_nShapePath = 4; //complex
		
		pOutput->m_nGeoLeft		= 0;	
		pOutput->m_nGeoTop		= 0;		
		pOutput->m_nGeoRight	= 100000;
		pOutput->m_nGeoBottom	= 100000;
	}

	Parse(oParam, pOutput, ooxShape->spPr.xfrm.GetPointer());

	PPTX::Logic::ShapeStyle* oox_sp_style = ooxShape->style.GetPointer();
	
	bool use_fill_from_style = false;
	pOutput->m_bFilled = true;	

	switch(ooxShape->spPr.Fill.m_type)
	{
		case PPTX::Logic::UniFill::blipFill:
		case PPTX::Logic::UniFill::gradFill:
		case PPTX::Logic::UniFill::pattFill:
		case PPTX::Logic::UniFill::solidFill:
			Parse(oParam, pOutput, &ooxShape->spPr.Fill); 
			break;
		case PPTX::Logic::UniFill::noFill:			
			pOutput->m_bFilled = false;	break;
		default:
			use_fill_from_style = true; break;
	}
	if ((use_fill_from_style && oox_sp_style) && (oox_sp_style->fillRef.idx.IsInit()))
	{
		Parse(oParam, pOutput, &oox_sp_style->fillRef, 1);
	}

	if ((oox_sp_style) && (oox_sp_style->lnRef.idx.IsInit()))
	{
		Parse(oParam, pOutput, &oox_sp_style->lnRef, 2);
	}
	if (ooxShape->spPr.ln.IsInit())
	{
		Parse(oParam, pOutput, ooxShape->spPr.ln.GetPointer());	
	}
//---------------------------------------------------------------------
	PPTX::Logic::TxBody * text_properties = NULL;
	OOXTextItemReader oTextItemReader;
	
	if (ooxShape->txBody.IsInit())
	{
		for (size_t i=0; i < ooxShape->txBody->Paragrs.size(); i++)
		{
			oTextItemReader.Parse(&ooxShape->txBody->Paragrs[i], oParam );
		}	

		text_properties = ooxShape->txBody.GetPointer();
	}

	if (oTextItemReader.m_oTextItems)
	{
		if (pOutput->m_bGtext == 1)
		{
			RenderParameter oRenderParameter;
			oRenderParameter.nType = RENDER_TO_OOX_PARAM_PLAIN;
			
			pOutput->m_sGtextUNICODE =  oTextItemReader.m_oTextItems->RenderToOOX(oRenderParameter);
		}
		else
		{
			pOutput->m_aTextItems = oTextItemReader.m_oTextItems;
		}

		if (text_properties)
		{
		}
	}

	return true;		
}

bool OOXShapeReader::ParsePic( ReaderParameter oParam, RtfShapePtr& pOutput)
{
	PPTX::Logic::Pic * ooxPic	= dynamic_cast<PPTX::Logic::Pic*>(m_ooxShape);

	if (!ooxPic) return false;

	std::wstring strXml;
	pOutput->m_nShapeType = SimpleTypes::Vml::sptPictureFrame;
	
	Parse(oParam, pOutput, ooxPic->spPr.xfrm.GetPointer());

	PPTX::Logic::ShapeStyle* oox_sp_style = ooxPic->style.GetPointer();
	
	pOutput->m_bFilled	= true;	
	
	Parse(oParam, pOutput, &ooxPic->blipFill);

	if ((oox_sp_style) && (oox_sp_style->lnRef.idx.IsInit()))
	{
		Parse(oParam, pOutput, &oox_sp_style->lnRef, 2);
	}
	if (ooxPic->spPr.ln.IsInit())
	{
		Parse(oParam, pOutput, ooxPic->spPr.ln.GetPointer());	
	}
//scale picture
	if (ooxPic->spPr.xfrm.IsInit() && pOutput->m_oPicture)
	{
		if (ooxPic->spPr.xfrm->extX.IsInit() && ooxPic->spPr.xfrm->extY.IsInit())
		{
			double cx = ooxPic->spPr.xfrm->extX.get();
			double cy = ooxPic->spPr.xfrm->extY.get();

			pOutput->m_oPicture->m_dScaleX = (pOutput->m_nRight - pOutput->m_nLeft) * 100. / pOutput->m_oPicture->m_nWidthGoal;
			pOutput->m_oPicture->m_dScaleY = (pOutput->m_nBottom - pOutput->m_nTop)* 100. / pOutput->m_oPicture->m_nHeightGoal;
		}
	}
	return true;		
}
bool OOXShapeReader::Parse( ReaderParameter oParam, RtfShapePtr& pOutput)
{
	if (!m_vmlElement && !m_arrElement && !m_ooxShape) return false;
	
	if (OOX::CHdrFtr *pHdrFtr = dynamic_cast<OOX::CHdrFtr *>(oParam.oReader->m_currentContainer))
		pOutput->m_nHeader = 1;//shape in header/footer
	else 
		pOutput->m_nHeader = 0;

	if (m_vmlElement ||  m_arrElement)	return ParseVml(oParam , pOutput);

	PPTX::Logic::Shape	* ooxShape	= dynamic_cast<PPTX::Logic::Shape*>	(m_ooxShape);
//	PPTX::Logic::CxnSp	* cxnShape	= dynamic_cast<PPTX::Logic::CxnSp*>	(m_ooxShape);
	PPTX::Logic::Pic	* ooxPic	= dynamic_cast<PPTX::Logic::Pic*>	(m_ooxShape);

	if (ooxShape)	return ParseShape(oParam, pOutput);
	if (ooxPic)		return ParsePic(oParam, pOutput);
	//if (cxnShape)	return ParseConnector(oParam, pOutput);

	return false;
}
void OOXShapeReader::Parse(ReaderParameter oParam, RtfShapePtr& pOutput, PPTX::Logic::UniFill *oox_fill, std::wstring *change_sheme_color)
{
	if (!oox_fill) return;

	switch(oox_fill->m_type)
	{
		case PPTX::Logic::UniFill::blipFill:	Parse(oParam, pOutput, dynamic_cast<PPTX::Logic::BlipFill*>(oox_fill->Fill.operator ->()));	break;
		case PPTX::Logic::UniFill::gradFill:	Parse(oParam, pOutput, dynamic_cast<PPTX::Logic::GradFill*>(oox_fill->Fill.operator ->()), change_sheme_color);	break;
		case PPTX::Logic::UniFill::pattFill:	Parse(oParam, pOutput, dynamic_cast<PPTX::Logic::PattFill*>(oox_fill->Fill.operator ->()), change_sheme_color);	break;
		case PPTX::Logic::UniFill::solidFill:	Parse(oParam, pOutput, dynamic_cast<PPTX::Logic::SolidFill*>(oox_fill->Fill.operator ->()), change_sheme_color);break;
	}
}

bool OOXShapeReader::ParseVml( ReaderParameter oParam , RtfShapePtr& pOutput)
{
	if (m_vmlElement == NULL && m_arrElement)	return false;
	if (m_vmlElement == NULL )					return ParseVmlChild(oParam , pOutput);

	if( m_vmlElement->m_sId.IsInit())
	{
		pOutput->m_nID = oParam.oReader->m_oOOXIdGenerator.GetId( m_vmlElement->m_sId.get());
	}
	
	//pOutput->m_nLeft		= 0; //стили только с widht height (например в Numbering)
	//pOutput->m_nTop		= 0;

// геометрия --------------------------------------------------------------------------------------------------------

	SimpleTypes::Vml::CVmlPath * custom_path = NULL;
	int Width = 0, Height = 0;

	if (OOX::Vml::CShapeType* shape_type = dynamic_cast<OOX::Vml::CShapeType*>(m_vmlElement))
	{
		if (pOutput->m_nShapeType == PROP_DEF)
			pOutput->m_nShapeType = NSOfficeDrawing::sptNotPrimitive;
		
		if (shape_type->m_oSpt.IsInit())
		{
			pOutput->m_nShapeType = shape_type->m_oSpt->GetValue();
		}
		if (shape_type->m_sId.IsInit())
		{
			if (oParam.oReader->m_mapShapeTypes.find(shape_type->m_sId.get()) == 
				oParam.oReader->m_mapShapeTypes.end())
			{
				oParam.oReader->m_mapShapeTypes.insert(oParam.oReader->m_mapShapeTypes.begin(), 
					std::pair<std::wstring, OOX::Vml::CShapeType*>(shape_type->m_sId.get(), shape_type));

				return false;//add type, not add object
			}
		}
		custom_path = shape_type->m_oPath.GetPointer();
		if (shape_type->m_oCoordOrigin.IsInit())
		{
			Width = shape_type->m_oCoordOrigin->GetX();
			Height = shape_type->m_oCoordOrigin->GetY();
		}
	}
	if (OOX::Vml::CShape* shape = dynamic_cast<OOX::Vml::CShape*>(m_vmlElement))
	{
        if (shape->m_sAdj.IsInit())
            ParseAdjustment( *pOutput, shape->m_sAdj.get() );
		
		if (shape->m_oSpt.IsInit())
		{
			pOutput->m_nShapeType = shape->m_oSpt->GetValue();
		}
		if (shape->m_sType.IsInit())
		{
            std::wstring type = shape->m_sType.get().substr(1);//without #
			std::map<std::wstring, OOX::Vml::CShapeType*>::iterator it = oParam.oReader->m_mapShapeTypes.find(type);
			
			if ( it != oParam.oReader->m_mapShapeTypes.end())
			{
				OOXShapeReader sub_reader(it->second);
				sub_reader.Parse(oParam, pOutput);
			}
			if (pOutput->m_nShapeType == PROP_DEF)
			{
                int pos = (int)shape->m_sType->find( L"#_x0000_t" );
				if (pos >= 0)
				{				
                    pOutput->m_nShapeType = _wtoi(shape->m_sType->substr(pos + 9, shape->m_sType->length() - pos - 9).c_str());
				}
			}
		}
		else if (pOutput->m_nShapeType == PROP_DEF)
		{
			pOutput->m_nShapeType = NSOfficeDrawing::sptNotPrimitive;
		}
		custom_path = shape->m_oPath.GetPointer();
		if (shape->m_oCoordOrigin.IsInit())
		{
			Width = shape->m_oCoordOrigin->GetX();
			Height = shape->m_oCoordOrigin->GetY();
		}
	}
	else if (OOX::Vml::CRect* rect = dynamic_cast<OOX::Vml::CRect*>(m_vmlElement))
	{
		pOutput->m_nShapeType	= NSOfficeDrawing::sptRectangle;
	}
	else if (OOX::Vml::COval* oval = dynamic_cast<OOX::Vml::COval*>(m_vmlElement))
	{
		pOutput->m_nShapeType	= NSOfficeDrawing::sptEllipse;
	}
	else if (OOX::Vml::CLine* line = dynamic_cast<OOX::Vml::CLine*>(m_vmlElement))
	{
		pOutput->m_nShapeType	= NSOfficeDrawing::sptLine;
	}
	else if (OOX::Vml::CArc* arc = dynamic_cast<OOX::Vml::CArc*>(m_vmlElement))
	{
		pOutput->m_nShapeType	= NSOfficeDrawing::sptArc;
	}
	else if (OOX::Vml::CCurve* curve= dynamic_cast<OOX::Vml::CCurve*>(m_vmlElement))
	{
		pOutput->m_nShapeType	= NSOfficeDrawing::sptNotPrimitive;
	}
	else if (OOX::Vml::CRoundRect* curve= dynamic_cast<OOX::Vml::CRoundRect*>(m_vmlElement))
	{
		pOutput->m_nShapeType	= NSOfficeDrawing::sptRoundRectangle;
	}
	else if (OOX::Vml::CPolyLine* polyline = dynamic_cast<OOX::Vml::CPolyLine*>(m_vmlElement))
	{
		pOutput->m_nShapeType	= NSOfficeDrawing::sptNotPrimitive;
	}

	if (pOutput->m_nShapeType == NSOfficeDrawing::sptNotPrimitive && custom_path)
	{
		ParseVmlPath(pOutput, custom_path->GetValue());
		
		pOutput->m_nShapePath = 4; //complex
		
		pOutput->m_nGeoLeft		= 0;	
		pOutput->m_nGeoTop		= 0;		
		pOutput->m_nGeoRight	= Width;
		pOutput->m_nGeoBottom	= Height;
	}
//-------------------------------------------------------------------------------------------------------------
	if (m_vmlElement->m_oFilled.IsInit())
		pOutput->m_bFilled = m_vmlElement->m_oFilled->GetValue() ==  SimpleTypes::booleanFalse ? 0 : 1;

	if (m_vmlElement->m_oStroked.IsInit())
		pOutput->m_bLine = m_vmlElement->m_oStroked->GetValue() ==  SimpleTypes::booleanFalse ? 0 : 1;

	if (m_vmlElement->m_oFillColor.IsInit())
		pOutput->m_nFillColor = (m_vmlElement->m_oFillColor->Get_B() << 16) + (m_vmlElement->m_oFillColor->Get_G() << 8) + m_vmlElement->m_oFillColor->Get_R();

	if (m_vmlElement->m_oStrokeColor.IsInit())
		pOutput->m_nLineColor = (m_vmlElement->m_oStrokeColor->Get_B() << 16) + (m_vmlElement->m_oStrokeColor->Get_G() << 8) + m_vmlElement->m_oStrokeColor->Get_R();

	if( m_vmlElement->m_oStrokeWeight.IsInit())
		pOutput->m_nLineWidth = (int)m_vmlElement->m_oStrokeWeight->ToEmu();

	if (m_vmlElement->m_oConnectorType.IsInit())
	{
		switch(m_vmlElement->m_oConnectorType->GetValue())
		{
			case SimpleTypes::connectortypeCurved	: pOutput->m_nConnectionType = 2; break;
			case SimpleTypes::connectortypeElbow	: pOutput->m_nConnectionType = 1; break;
			case SimpleTypes::connectortypeNone		: pOutput->m_nConnectionType = 3; break;
			case SimpleTypes::connectortypeStraight	: pOutput->m_nConnectionType = 0; break;
		}
	}

	pOutput->m_bLayoutInCell = m_vmlElement->m_oAllowInCell.GetValue();
	pOutput->m_bAllowOverlap = m_vmlElement->m_oAllowOverlap.GetValue();

	pOutput->m_nWrapType	= 3; //default (non wrap)
	pOutput->m_eAnchorTypeShape	= RtfShape::st_none; //inline or anchor
	
	if ( m_vmlElement->m_oStyle.IsInit())
	{
		if( false == ParseVmlStyles( pOutput, m_vmlElement->m_oStyle->m_arrProperties ) )
			return false;
	}

	if( m_vmlElement->m_oWrapCoords.IsInit())
	{
		if (pOutput->m_nWrapType == 3 && pOutput->m_nZOrderRelative == PROP_DEF) 
			pOutput->m_nWrapType = 2;
		
		int nPosition = 0;
		std::wstring sPoint =  L"start";
		for (int i =0 ;i < m_vmlElement->m_oWrapCoords->GetSize(); i++)
		{
			pOutput->m_aWrapPoints.push_back( std::pair<int,int>(	m_vmlElement->m_oWrapCoords->GetX(i),
																	m_vmlElement->m_oWrapCoords->GetY(i)));
		}
	}
	if( m_vmlElement->m_oCoordOrigin.IsInit() )
	{
		pOutput->m_nGroupLeft	= m_vmlElement->m_oCoordOrigin->GetX();
		pOutput->m_nGroupTop	= m_vmlElement->m_oCoordOrigin->GetY();
	}

	if( m_vmlElement->m_oCoordSize.IsInit())
	{// shapeType content only size
		pOutput->m_nGroupRight	= (pOutput->m_nGroupLeft != PROP_DEF ? pOutput->m_nGroupLeft : 0)	+ m_vmlElement->m_oCoordSize->GetX();
		pOutput->m_nGroupBottom = (pOutput->m_nGroupTop != PROP_DEF ? pOutput->m_nGroupTop : 0)		+ m_vmlElement->m_oCoordSize->GetY();
	}

	if (m_vmlElement->m_oConnectorType.IsInit())
	{
		switch(m_vmlElement->m_oConnectorType->GetValue())
		{
		case SimpleTypes::connectortypeCurved  : pOutput->m_nConnectorStyle = 2; break;			
		case SimpleTypes::connectortypeElbow   : pOutput->m_nConnectorStyle = 1; break;		
		case SimpleTypes::connectortypeNone    : pOutput->m_nConnectorStyle = 3; break;		
		case SimpleTypes::connectortypeStraight: pOutput->m_nConnectorStyle = 0; break;		
        default: break;
        }
	}

//---------------------
	return ParseVmlChild(oParam, pOutput);
}
bool OOXShapeGroupReader::Parse( ReaderParameter oParam , RtfShapePtr& pOutput)
{
	if (m_vmlGroup == NULL && m_ooxGroup == NULL) return false;

	pOutput->m_bIsGroup = true;

	if (m_vmlGroup )
	{
		if( m_vmlGroup->m_sId.IsInit())
		{
			pOutput->m_nID = oParam.oReader->m_oOOXIdGenerator.GetId( m_vmlGroup->m_sId.get());
		}
		pOutput->m_eAnchorTypeShape	= RtfShape::st_none; //inline or anchor
		
		pOutput->m_bLayoutInCell	= m_vmlGroup->m_oAllowInCell.GetValue();
		pOutput->m_bAllowOverlap	= m_vmlGroup->m_oAllowOverlap.GetValue();

		pOutput->m_nZOrderRelative	= 0;
		
		pOutput->m_nWrapType		= 3; //def
		
		if ( m_vmlGroup->m_oStyle.IsInit())
		{
			if( false == ParseVmlStyles( pOutput, m_vmlGroup->m_oStyle->m_arrProperties ) )
				return false;
		}

		if( m_vmlGroup->m_oWrapCoords.IsInit())
		{
			if (pOutput->m_nWrapType == 3 && pOutput->m_nZOrderRelative == PROP_DEF) pOutput->m_nWrapType =2;
			int nPosition = 0;
			std::wstring sPoint =  L"start";
			for (int i =0 ;i < m_vmlGroup->m_oWrapCoords->GetSize(); i++)
			{
				pOutput->m_aWrapPoints.push_back( std::pair<int,int>(	m_vmlGroup->m_oWrapCoords->GetX(i),
																		m_vmlGroup->m_oWrapCoords->GetY(i)));
			}
		}

		if( m_vmlGroup->m_oCoordOrigin.IsInit() )
		{
			pOutput->m_nGroupLeft	= m_vmlGroup->m_oCoordOrigin->GetX();
			pOutput->m_nGroupTop	= m_vmlGroup->m_oCoordOrigin->GetY();
		}

		if( m_vmlGroup->m_oCoordSize.IsInit())
		{
			pOutput->m_nGroupRight = (pOutput->m_nGroupLeft != PROP_DEF  ? pOutput->m_nGroupLeft : 0)	+ m_vmlGroup->m_oCoordSize->GetX();
			pOutput->m_nGroupBottom =(pOutput->m_nGroupTop != PROP_DEF  ? pOutput->m_nGroupTop : 0)		+ m_vmlGroup->m_oCoordSize->GetY();
		}

        for (std::vector<OOX::WritingElement*>::iterator	it = m_vmlGroup->m_arrItems.begin(); it != m_vmlGroup->m_arrItems.end(); ++it)
		{
			if (*it == NULL) continue;

			if ((*it)->getType() == OOX::et_v_group)
			{
				RtfShapePtr pNewShape( new RtfShape() );
				
				OOXShapeGroupReader oShapeReader(dynamic_cast<OOX::Vml::CGroup*>(*it));
				
				if( true == oShapeReader.Parse( oParam, pNewShape ) )
					 pOutput->AddItem( pNewShape );
			}
			else if (	(*it)->getType() == OOX::et_v_arc		||
						(*it)->getType() == OOX::et_v_line		||
						(*it)->getType() == OOX::et_v_oval		||
						(*it)->getType() == OOX::et_v_shape		||
						(*it)->getType() == OOX::et_v_rect		||
						(*it)->getType() == OOX::et_v_roundrect ||
						(*it)->getType() == OOX::et_v_polyline	||
						(*it)->getType() == OOX::et_v_shapetype)
			{
				RtfShapePtr pNewShape ( new RtfShape() );//set type .. .todooo
				
				OOXShapeReader oShapeReader(dynamic_cast<OOX::Vml::CVmlCommonElements*>(*it));
				
				pNewShape->m_bInGroup = true;
				if( true == oShapeReader.Parse( oParam, pNewShape ) )
					 pOutput->AddItem( pNewShape );
			}
			else
			{
				//??? todooo
				//shapetype как минимум нужен !!!
			}
		}
	}
	if (m_ooxGroup)
	{
		pOutput->m_nZOrderRelative	= 0;
		
		pOutput->m_nWrapType		= 3; //def

		if( m_ooxGroup->grpSpPr.xfrm.IsInit())
		{
			double rot = m_ooxGroup->grpSpPr.xfrm->rot.get() / 60000.;
			if (rot > 0.01)
				pOutput->m_nRotation = (int)(rot * 65535);		
			
			if (m_ooxGroup->grpSpPr.xfrm->flipH.get_value_or(false))	pOutput->m_bFlipH = 1;	
			if (m_ooxGroup->grpSpPr.xfrm->flipV.get_value_or(false))	pOutput->m_bFlipV = 1;
			
			if( m_ooxGroup->grpSpPr.xfrm->chOffX.IsInit() && m_ooxGroup->grpSpPr.xfrm->chOffY.IsInit())
			{
				pOutput->m_nGroupLeft	= m_ooxGroup->grpSpPr.xfrm->chOffX.get();
				pOutput->m_nGroupTop	= m_ooxGroup->grpSpPr.xfrm->chOffY.get();
			}
			if (m_ooxGroup->grpSpPr.xfrm->chExtX.IsInit() && m_ooxGroup->grpSpPr.xfrm->chExtY.IsInit())
			{
				pOutput->m_nGroupRight	= (pOutput->m_nGroupLeft != PROP_DEF  ? pOutput->m_nGroupLeft : 0) + (int)m_ooxGroup->grpSpPr.xfrm->chExtX.get();
				pOutput->m_nGroupBottom = (pOutput->m_nGroupTop != PROP_DEF  ? pOutput->m_nGroupTop : 0) + (int)m_ooxGroup->grpSpPr.xfrm->chExtY.get();
			}
			if (pOutput->m_bInGroup)
			{
				if (m_ooxGroup->grpSpPr.xfrm->offX.IsInit() && m_ooxGroup->grpSpPr.xfrm->offY.IsInit())
				{
					pOutput->m_nRelLeft	= m_ooxGroup->grpSpPr.xfrm->offX.get();
					pOutput->m_nRelTop	= m_ooxGroup->grpSpPr.xfrm->offY.get();
				}
				else
				{
					pOutput->m_nRelLeft	= 0;
					pOutput->m_nRelTop	= 0;
				}
				if (m_ooxGroup->grpSpPr.xfrm->extX.IsInit() && m_ooxGroup->grpSpPr.xfrm->extY.IsInit())
				{
					pOutput->m_nRelRight	= (int)pOutput->m_nRelLeft + m_ooxGroup->grpSpPr.xfrm->extX.get();
					pOutput->m_nRelBottom	= (int)pOutput->m_nRelTop + m_ooxGroup->grpSpPr.xfrm->extY.get();
				}
				else
				{
					pOutput->m_nRelRight	= 0;
					pOutput->m_nRelBottom	= 0;
				}
				pOutput->m_nRelRotation = pOutput->m_nRotation;
			}
		}

		for (size_t i = 0; i < m_ooxGroup->SpTreeElems.size() ; i++ )
		{
			if (m_ooxGroup->SpTreeElems[i].getType() == OOX::et_p_ShapeTree)
			{
				RtfShapePtr pNewShape( new RtfShape() );
				pNewShape->m_bIsGroup = true;

                smart_ptr<PPTX::Logic::SpTree> e = m_ooxGroup->SpTreeElems[i].GetElem().smart_dynamic_cast<PPTX::Logic::SpTree>();
				
				OOXShapeGroupReader oShapeReader(e.operator->());
				
				pNewShape->m_bInGroup = true;
				if( true == oShapeReader.Parse( oParam, pNewShape ) )
					 pOutput->AddItem( pNewShape );
			}
			else
			{
				RtfShapePtr pNewShape ( new RtfShape() );
				
                smart_ptr<OOX::WritingElement> e = m_ooxGroup->SpTreeElems[i].GetElem().smart_dynamic_cast<OOX::WritingElement>();
				OOXShapeReader oShapeReader(e.operator->());
				
				pNewShape->m_bInGroup = true;
				if( true == oShapeReader.Parse( oParam, pNewShape ) )
					 pOutput->AddItem( pNewShape );
			}
		}
	}
	return true;
}

void OOXShapeReader::ParseVmlPath (RtfShapePtr& pOutput, const std::wstring &custom_path)
{
	std::vector<svg_path::_polyline> o_Polyline;
	
	bool res = svg_path::parseVml(o_Polyline, custom_path);

	int val = 0;
	for (size_t i = 0; i < o_Polyline.size(); i++)
	{
		if (o_Polyline[i].command == L"m")
		{
			val = 0x4000;
		}
		else if (o_Polyline[i].command == L"l")	
		{
			val = 0x0000 | 1;
		}
		else if (o_Polyline[i].command == L"c")
		{
			val = 0x2000 | 1;
		}	
		
		pOutput->m_aPSegmentInfo.push_back(val);

		for (size_t j = 0; j < o_Polyline[i].points.size(); j++)
		{
			pOutput->m_aPVerticles.push_back(std::make_pair((int)(o_Polyline[i].points[j].x.get_value_or(0)/* / 10000. * W*/),
															(int)(o_Polyline[i].points[j].y.get_value_or(0)/* / 10000. * H*/)));
		}
	}

	pOutput->m_aPSegmentInfo.push_back(0x6001);
	pOutput->m_aPSegmentInfo.push_back(0x8000);
}

void OOXShapeReader::ParseAdjustment (RtfShape& oShape, std::wstring sAdjustment)
{
	std::vector< std::wstring > splitted;
	
    boost::algorithm::split(splitted, sAdjustment, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

	for (size_t i = 0; i < splitted.size(); i++)
	{
		if (!splitted[i].empty())
		{
			try
			{
				oShape.m_nAdjustValue[i] = _wtoi(splitted[i].c_str());
			}
			catch(...)
			{
				oShape.m_nAdjustValue[i] = 0;
			}
		}
	}
 }


bool OOXBackgroundReader::Parse( ReaderParameter oParam , RtfShapePtr& pOutput)
{
	if (!m_ooxBackground) return false;

	pOutput->m_nShapeType	= 1;
	pOutput->m_bBackground	= true;
	pOutput->m_bFilled		= true;

	RtfColor rtfColor;
	bool result = false;
	if (m_ooxBackground->m_oThemeColor.IsInit())
	{
		oParam.oRtf->m_oColorTable.GetColor( m_ooxBackground->m_oThemeColor->GetValue() , rtfColor );
		if (rtfColor.m_eTheme != RtfColor::TC_NONE) result = true;
	}
	if (m_ooxBackground->m_oThemeShade.IsInit())
	{
		rtfColor.SetShade( m_ooxBackground->m_oThemeShade->GetValue() );
		result = true;
	}
	if (m_ooxBackground->m_oThemeTint.IsInit())
	{
		rtfColor.SetTint( m_ooxBackground->m_oThemeTint->GetValue() );
		result = true;
	}
	if (m_ooxBackground->m_oColor.IsInit())
	{		
		if (m_ooxBackground->m_oColor->GetValue() == SimpleTypes::hexcolorRGB)
		{
			rtfColor.SetRGB(m_ooxBackground->m_oColor->Get_R(), m_ooxBackground->m_oColor->Get_G(), m_ooxBackground->m_oColor->Get_B());
		
			unsigned char opacity = m_ooxBackground->m_oColor->Get_A();
			if (opacity != 0xff)
				pOutput->m_nFillOpacity = (int)(opacity / 255. * 100);	
		}
		if (m_ooxBackground->m_oColor->GetValue() == SimpleTypes::colormodeAuto)
			rtfColor.m_bAuto = true;
		
		result = true;
	}

	if (result == true)
	{
		pOutput->m_nFillColor = rtfColor.GetRGB();

	}
	if (m_ooxBackground->m_oBackground.IsInit())
	{
		OOXShapeReader sub_reader(m_ooxBackground->m_oBackground.GetPointer());
		sub_reader.Parse(oParam, pOutput);
	}	
	else if (m_ooxBackground->m_oDrawing.IsInit())
	{
		OOXDrawingReader oDrawingReader(m_ooxBackground->m_oDrawing.GetPointer());
		
		OOXDrawingReader sub_reader(m_ooxBackground->m_oDrawing.GetPointer());
		sub_reader.Parse(oParam, pOutput);
		//if( false == oDrawingReader.Parse( oParam, *pOutput ) )
		//{			 
		//	if (!m_ooxBackground->m_oDrawing->m_sXml.IsInit())
		//	{	
		//		OOXDrawingGraphicReader oGraphiceReader(*m_ooxBackground->m_oDrawing->m_sXml);
		//		OOX::Logic::e *ooxPicture = oGraphiceReader.Parse( oParam, *pOutput );
		//		if (ooxPicture)
		//		{
		//			OOXShapeReader sub_reader(ooxPicture);
		//			sub_reader.Parse(oParam, pOutput);
		//			
		//			delete ooxPicture;
		//		}
		//	}
		//}
	}
	return true;
}
bool OOXShapeReader::WriteDataToPicture( std::wstring sPath, RtfPicture& pOutput, std::wstring sTempPath)
{
	OOX::CPath ooxPath = sPath;	//для target 

	if (!sTempPath.empty())
		ooxPath = sTempPath + FILE_SEPARATOR_STR;

	pOutput.m_dScaleX = 100;
	pOutput.m_dScaleY = 100;

	//Выставляем тип картинки
	pOutput.eDataType = RtfPicture::GetPictureType( sPath );

//ecли тип не поддерживается rtf конвертируем в png
	if( RtfPicture::dt_none == pOutput.eDataType )
	{
		//в туже папку что и исходная картинка
		CBgraFrame image;
		if (image.OpenFile(sPath) == FALSE ) return false;
		
		//правильно выставляем размеры
		pOutput.m_nWidthGoal	= image.get_Width()	* 15;  //pixels to twip (  1440 / 96 )
		pOutput.m_nHeightGoal	= image.get_Height()* 15;  //pixels to twip (  1440 / 96 )
	
        std::wstring sTargetFile = NSDirectory::CreateTempFileWithUniqueName(ooxPath.GetDirectory(), L"img");
		
		if (image.SaveFile(sTargetFile, 4 /*_CXIMAGE_FORMAT_PNG*/) )
		{
			pOutput.eDataType = RtfPicture::dt_png;
			//Запоминаем имя
			pOutput.m_sPicFilename = sTargetFile;
			pOutput.m_bIsCopy = true;//выставляем флаг чтобы потом удалить файл
		}
	}
	else if( RtfPicture::dt_apm ==  pOutput.eDataType )
	{
		//убираем заголовок apm (22 byte)
		CFile file_inp; //mpa
		CFile file_out;//wmf

        std::wstring sTargetFile = NSDirectory::CreateTempFileWithUniqueName(ooxPath.GetDirectory(), L"img");

		int res = file_inp.OpenFile(sPath);
		if (res != S_OK) return false;

        res = file_out.CreateFile(sTargetFile);
		if (res != S_OK) return false;

		DWORD dwBytesRead = 0;
		DWORD dwBytesWrite = 0;
		BYTE pBuffer[1024];
		DWORD nHeaderLen = 22;

		dwBytesRead = file_inp.GetPosition();
		file_inp.ReadFile(pBuffer, 1024);
		dwBytesRead = file_inp.GetPosition() - dwBytesRead;
			
		while( 0 != dwBytesRead )
		{
			if( nHeaderLen > 0 )
			{
				if(dwBytesRead > nHeaderLen )
				{
					file_out.WriteFile(pBuffer + nHeaderLen, dwBytesRead - nHeaderLen);
					nHeaderLen = 0;
				}
				else
					nHeaderLen -= dwBytesRead;
			}
			else
				file_out.WriteFile( pBuffer, dwBytesRead);

			dwBytesRead = file_inp.GetPosition();
			file_inp.ReadFile(pBuffer, 1024);
			dwBytesRead = file_inp.GetPosition() - dwBytesRead;
		}
		file_inp.CloseFile();
		file_out.CloseFile();

		pOutput.eDataType = RtfPicture::dt_wmf;
		//Запоминаем имя
		pOutput.m_sPicFilename = sTargetFile;
		pOutput.m_bIsCopy = true;
	}
	else
	{
		if (pOutput.eDataType == RtfPicture::dt_emf || pOutput.eDataType == RtfPicture::dt_wmf)
		{
			MetaFile::CMetaFile meta(NULL);
			if (meta.LoadFromFile(sPath.c_str()))
			{
				double dX, dY, dW, dH;
				meta.GetBounds(&dX, &dY, &dW, &dH);
				meta.Close();
				
				pOutput.m_nWidthGoal	= (int)(dW * 15);  //pixels to twip 
				pOutput.m_nHeightGoal	= (int)(dH * 15);  //pixels to twip;
			}
			//Запоминаем только имя
			pOutput.m_sPicFilename = sPath;
			pOutput.m_bIsCopy = false; //не удалять 
		}
		else
		{//png, jpeg
			CBgraFrame image;
		
			if (image.OpenFile(sPath, 0) == FALSE ) return false;
			//правильно выставляем размеры
			pOutput.m_nWidthGoal	= image.get_Width()		* 15;  //pixels to twip 
			pOutput.m_nHeightGoal	= image.get_Height()	* 15;  //pixels to twip;

			//Запоминаем только имя
			pOutput.m_sPicFilename = sPath;
			pOutput.m_bIsCopy = false; //не удалять 
		}
	}
	return true;
}
