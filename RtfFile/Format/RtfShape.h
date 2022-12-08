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
#pragma once
#include "RtfPicture.h"

class RtfShape;
typedef boost::shared_ptr<RtfShape> RtfShapePtr;

class RtfOle;
typedef boost::shared_ptr<RtfOle> RtfOlePtr;

class RtfShape: public IRenderableProperty, public ItemContainer<RtfShapePtr>
{
private:
	bool m_bInsert;
	bool m_bDelete;
	
public: 
	bool m_bBackground;
	bool m_bIsGroup;
	bool m_bInGroup; //local anchor

	std::wstring	m_sOle;
	bool			m_bIsOle;
	RtfOlePtr		m_pOleObject;
//-----------------------------
	enum _AnchorTypeShape { st_none, st_inline, st_anchor };
	enum _AnchorX
	{
		ax_none,
		//ax_ignore,			//shpbxignore	Ignore \shpbxpage, \shpbxmargin, and \shpbxcolumn, in favor of the posrelh property. The ignored properties will be written for backward compatibility with older readers that do not understand \posrelh.
		ax_page,				//shpbxpage 	The shape is positioned relative to the page in the x (horizontal) direction. 
		ax_margin,				//shpbxmargin 	The shape is positioned relative to the margin in the x (horizontal) direction.
		ax_column				//shpbxcolumn	The shape is positioned relative to the column in the x (horizontal) direction.
	};
	
	enum _AnchorY
	{ 
		ay_none,
		//ay_ignore,			//shpbyignore	Ignore \shpbypage, \shpbymargin, and \shpbxpara, in favor of the posrelh property. The ignored properties will be written for backward compatibility with older readers that do not understand the posrelh property.
		ay_page,				//shpbypage 	The shape is positioned relative to the page in the y (vertical) direction. 
		ay_margin,				//shpbymargin 	The shape is positioned relative to the margin in the y (vertical) direction.
		ay_Para					//shpbypara	The shape is positioned relative to the paragraph in the y (vertical) direction.
	};

	_AnchorTypeShape m_eAnchorTypeShape;

    std::wstring m_sName;
    std::wstring m_sDescription;

	int m_nWidth;
	int m_nHeight;

	int m_nLeft;					//shpleftN	Specifies position of shape from the left of the anchor. The value N is in twips.
	int m_nTop;						//shptopN	Specifies position of shape from the top of the anchor. The value N is in twips.
	int m_nBottom;					//shpbottomN	Specifies position of shape from the bottom of the anchor. The value N is in twips.
	int m_nRight;					//shprightN	Specifies position of shape from the right of the anchor. The value N is in twips.
	
	int m_nID;						//shplidN	A number that is unique to each shape. This keyword is primarily used for linked text boxes. The value N is a long integer.
	int m_nZOrder;					//shpzN	Describes the z-order of the shape. It starts at 0 for the shape that is furthest from the top, and proceeds to the top most shape (N). The shapes that appear inside the header document will have a separate z-order, compared to the z-order of the shapes in the main document. For instance, both the back-most shape in the header and the back-most main-document shape will have a z-order of 0.
	int m_nHeader;					//shpfhdrN	Set to 0 if the shape is in the main document. Set to 1 if the shape is in the header document.
	int m_nWrapType;				//shpwrN	Describes the type of wrap for the shape:
	int m_nWrapSideType;			//shpwrkN	Wrap on side (for types 2 and 4 for \shpwrN ):
	int m_nZOrderRelative;			//shpfblwtxtN	Describes relative z-ordering:
	int m_bLockAnchor;				//shplockanchor	Lock anchor for a shape.
	int m_nLockPosition;
	int m_nLockRotation;
	
	int m_eXAnchor;
	int m_eYAnchor;

//----------------ShapeProperty--------------------
//Position absolute
	int m_nPositionH;				//posh	Horizontal alignment
	int m_nPositionHRelative;		//posrelh	Position horizontally relative to:
	int m_nPositionV;				//posv	Vertical alignment:
	int m_nPositionVRelative;		//posrelv Position horizontally relative to:
	int m_bLayoutInCell;			//fLayoutInCell	Allows shape to anchor and position inside table cells.
	int m_bAllowOverlap;			//fAllowOverlap	Allows shape to overlap other shapes unless it is a shape with None wrapping (\shpwr3), in which case it can always overlap an object with other types of wrapping and vice-versa. 
//Position relative
	int m_nPositionHPct;			//pctHorizPos Percentage horizontal position for a shape
	int m_nPositionVPct;			//pctVert Percentage vertical position for a shape
	int m_nPctWidth;				//pctHoriz Percentage width for a shape
	int m_nPctHeight;				//pctVert Percentage height for a shape
	int m_nPctWidthRelative;		//sizerelh Relative size horizontal relation
	int m_nPctHeightRelative;		//sizerelv Relative size vertical relation
	int m_nColStart;				//colStart Starting column
	int m_nColSpan;					//colSpan Number of columns to span
//Rehydration
    std::wstring m_sMetroBlob;			// metroBlob Specifies application-specific data used to convert a shape to other formats. It is an encoded TBYTE stream.
    std::wstring m_sMetroBlobRels;
    std::wstring m_sMetroBlobData;
//Object Type
	int m_bIsBullet;				//fIsBullet Indicates whether a picture was inserted as a picture bullet.
	int m_nRotation;
	int m_bFlipV;		
	int m_bFlipH;	
	int m_nShapeType;				// 0 indicates user-drawn freeforms and polygons
	
	int m_nWrapDistLeft;
	int m_nWrapDistTop;		
	int m_nWrapDistRight;
	int m_nWrapDistBottom;	
	int m_bHidden;					//Do not display or print 

//Text box
	int m_nAnchorText;
	int m_nTexpLeft;				//in EMU
	int m_nTexpTop;	
	int m_nTexpRight;
	int m_nTexpBottom;	
	int m_bFitShapeToText;
	int m_bFitTextToShape;
	int m_nCcol;					//columns
	int m_nTxdir;					//bidi
	int m_nWrapText;
	int m_nTxflTextFlow;
	int m_fAutoTextMargin;
	int m_fRotateText;
	int m_nScaleText;
	int m_CdirFont;
//Geometry
	int m_nAdjustValue[10];	
//Custom
	int m_nGeoLeft;	
	int m_nGeoTop;	
	int m_nGeoRight;
	int m_nGeoBottom;	
	int	m_nShapePath;
	std::vector< std::pair<int, int> >	m_aPVerticles;
	std::vector< int>					m_aPSegmentInfo;
//Connectors
	int m_nConnectionType;			//cxk Connection site type
	int m_nConnectorStyle;			//cxstyle Connector style

//Picture Effects
	int m_nCropFromTop;				//cropFromTop	Top cropping percentage. Измеряется в fraction 1 /65536
	int m_nCropFromBottom;			//cropFromBottom	Bottom cropping percentage.
	int m_nCropFromLeft;			//cropFromLeft	Left cropping percentage.
	int m_nCropFromRight;			//cropFromRight	Right cropping percentage.
//Grouped Shapes
	int m_nGroupBottom;				//groupBottom	Defines the height of the group rectangle, but does not necessarily indicate position on the page. The difference between groupBottom and groupTop should match the dimensions specified by \shptopN and \shpbottomN.
	int m_nGroupLeft;				//groupLeft	Defines the width of the group rectangle, but does not necessarily indicate position on the page. The difference between groupLeft and groupRight should match the dimensions specified by \shpleftN and \shprightN.
	int m_nGroupRight;				//groupRight	See meaning for groupLeft.
	int m_nGroupTop;				//groupTop	See meaning for groupBottom.
	int m_nRelBottom;				//relBottom	Defines the bottom of a shape within its parent shape (used for shapes in a group). The measurement is relative to the position of the parent group or drawing.
	int m_nRelLeft;					//relLeft	Defines the left of a shape within its parent shape (used for shapes in a group). The measurement is relative to the position of the parent group or drawing.
	int m_nRelRight;				//relRight	Defines the right of a shape within its parent shape (used for shapes in a group). The measurement is relative to the position of the parent group or drawing.
	int m_nRelTop;					//relTop	Defines the top of a shape within its parent shape (used for shapes in a group). The measurement is relative to the position of the parent group or drawing.
	int m_nRelRotation;				//relRotation	Represents the information stored in the site of a shape, which defines the size and location of the shape in the parent group or drawing. The coordinates are relative to the position of the parent group or drawing. The units are relative to the m_rcg of the parent.
	int m_nRelZOrder;				//dhgt	Word 2007 Z-order position of shape(s) on a page. Shapes with small dhgts are further back than shapes with large dhgts.

//Fill
	int m_bFilled;					//fFilled The shape is filled.
	int m_nFillColor;				//fillColor
	int m_nFillColor2;				//fillBackColor
	int m_nFillType;
	int m_nFillOpacity;
	int m_nFillOpacity2;
	int m_nFillFocus;
	int m_nFillAngle;
	int m_nFillToBottom;
	int m_nFillToTop;
	int m_nFillToRight;
	int m_nFillToLeft;
	int m_nFillShadeType;
	std::vector< std::pair<int, int> >	m_aFillShadeColors;
	//int m_bFillShape;				//есть копия заливки картинкой	
//Line
	int m_bLine;					//fLine Has a line
	int m_nLineColor;
	int m_nLineStartArrow;			//lineStartArrowhead Start arrow type:
	int m_nLineEndArrow;			//lineEndArrowhead	End arrow type (for acceptable values see meaning for lineStartArrowhead).
	int m_nLineStartArrowWidth;		//lineStartArrowWidth	Start arrow width:
	int m_nLineStartArrowLength;	//lineStartArrowLength	Start arrow length:
	int m_nLineEndArrowWidth;		//lineEndArrowWidth	End arrow width (for acceptable values see meaning for lineStartArrowWidth).
	int m_nLineEndArrowLength;		//lineEndArrowLength	End arrow length (for acceptable values see meaning for lineStartArrowLength).
	int m_nLineWidth;				//lineWidth Width of the line.
	int m_nLineDashing;

//WordArt
    std::wstring	m_sGtextUNICODE;
    std::wstring	m_sGtextFont;
	int				m_nGtextSize;
	int				m_bGtext;
	int				m_bGtextFVertical;
	int				m_bGtextFKern;
	int				m_bGtextFStretch;
	int				m_bGtextFShrinkFit;
	int				m_bGtextFBestFit;

	int				m_bIsSignatureLine;
	int				m_bSigSetupAllowComments;
	std::wstring	m_sSigSetupId;
	std::wstring	m_sSigSetupProvId;
	std::wstring	m_sSigSetupSuggSigner;
	std::wstring	m_sSigSetupSuggSigner2;
	std::wstring	m_sSigSetupSuggSignerEmail;

	RtfCharProperty	m_oCharProperty; // тут могут быть track changes ....

//pWrapPolygonVertices	Points of the text wrap polygon.
	std::vector< std::pair<__int64, __int64> >	m_aWrapPoints;
//textbox
	TextItemContainerPtr	m_aTextItems;
	RtfPicturePtr			m_oPicture;
//------------------------------------------------------------------------------------------------------
	RtfShape();

	bool IsValid();

	void SetDefaultRtf();
	void SetDefaultOOX();
	void SetDefault();

	void SetNotSupportShape();

    std::wstring RenderToRtf		(RenderParameter oRenderParameter);
    std::wstring RenderToOOX		(RenderParameter oRenderParameter);

    std::wstring RenderToOOXBegin	(RenderParameter oRenderParameter);
    std::wstring RenderToOOXEnd		(RenderParameter oRenderParameter);
	
	void ToRtfRotation( int nAngel , int &nLeft, int &nTop, int& nRight, int& nBottom );

    std::wstring RenderToRtfShapeProperty(RenderParameter oRenderParameter);
    std::wstring GetShapeNodeName();
   
	std::wstring GroupRenderToRtf(RenderParameter oRenderParameter);
    std::wstring GroupRenderToOOX(RenderParameter oRenderParameter);

	bool GetPictureResolution(RenderParameter oRenderParameter, int& Width, int& Height);
};

