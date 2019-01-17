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

namespace ASCDocFileFormat
{
	class GroupShapeBooleans
	{
	public:

		GroupShapeBooleans();
		unsigned int Get() const;

	public:

		bool	fPrint;
		bool	fHidden;
		bool	fOneD;
		bool	fIsButton;

		bool	fOnDblClickNotify;
		bool	fBehindDocument;
		bool	fEditedWrap;
		bool	fScriptAnchor;

		bool	fReallyHidden;
		bool	fAllowOverlap;
		bool	fUserDrawn;
		bool	fHorizRule;

		bool	fNoshadeHR;
		bool	fStandardHR;
		bool	fIsBullet;
		bool	fLayoutInCell;

		bool	fUsefPrint;
		bool	fUsefHidden;
		bool	fUsefOneD;
		bool	fUsefIsButton;

		bool	fUsefOnDblClickNotify;
		bool	fUsefBehindDocument;
		bool	fUsefEditedWrap;
		bool	fUsefScriptAnchor;

		bool	fUsefReallyHidden;
		bool	fUsefAllowOverlap;
		bool	fUsefUserDrawn;
		bool	fUsefHorizRule;

		bool	fUsefNoshadeHR;
		bool	fUsefStandardHR;
		bool	fUsefIsBullet;
		bool	fUsefLayoutInCell;
	};

	class GeometryBooleans
	{
	public:

		GeometryBooleans();
		unsigned int Get() const;

	public:

		bool fFillOK;
		bool fFillShadeShapeOK;
		bool fGtextOK;
		bool fLineOK;
		bool f3DOK;
		bool fShadowOK;

		bool fUsefFillOK;
		bool fUsefFillShadeShapeOK;
		bool fUsefGtextOK;
		bool fUsefLineOK;
		bool fUsef3DOK;
		bool fUsefShadowOK;
	};

	class FillStyleBooleans
	{
	public:

		FillStyleBooleans();
		unsigned int Get() const;

	public:

		bool fNoFillHitTest;
		bool fillUseRect;
		bool fillShape;
		bool fHitTestFill;
		bool fFilled;
		bool fUseShapeAnchor;
		bool fRecolorFillAsPicture;

		bool fUsefNoFillHitTest;
		bool fUsefillUseRect;
		bool fUsefillShape;
		bool fUseHitTestFill;
		bool fUsefFilled;
		bool fUsefUseShapeAnchor;
		bool fUsefRecolorFillAsPicture;
	};

	class LineStyleBooleans
	{
	public:

		LineStyleBooleans ();
		unsigned int Get() const;
	public:

		bool fNoLineDrawDash;
		bool fLineFillShape;
		bool fHitTestLine;
		bool fLine;
		bool fArrowheadsOK;
		bool fInsetPenOK;
		bool fInsetPen;
		bool fLineOpaqueBackColor;

		bool fUsefNoLineDrawDash;
		bool fUsefLineFillShape;
		bool fUsefHitTestLine;
		bool fUsefLine;
		bool fUsefArrowheadsOK;
		bool fUsefInsetPenOK;
		bool fUsefInsetPen;
		bool fUsefLineOpaqueBackColor;
	};

	class ShadowBooleans
	{
	public:

		ShadowBooleans ();
		unsigned int Get() const;
	public:

		bool fUsefShadow;
		bool fUsefShadowObscured;

		bool fShadow;
		bool fShadowObscured;
	};

	class TextBooleans
	{
	public:

		TextBooleans ();
		unsigned int Get() const;
		bool IsValid ();
	public:

		bool fFitShapeToText;
		bool fAutoTextMargin;
		bool fSelectText;

		bool fUsefFitShapeToText;
		bool fUsefAutoTextMargin;
		bool fUsefSelectText;
	};

	class BlipBooleans
	{
	public:

		BlipBooleans ();
		unsigned int Get() const;
	public:

		bool fPictureActive;
		bool fPictureBiLevel;
		bool fPictureGray;
		bool fNoHitTestPicture;
		bool fLooping;
		bool fRewind;
		bool fPicturePreserveGrays;

		bool fUsefPictureActive;
		bool fUsefPictureBiLevel;
		bool fUsefPictureGray;
		bool fUsefNoHitTestPicture;
		bool fUsefLooping;
		bool fUsefRewind;
		bool fUsefPicturePreserveGrays;
	};
}