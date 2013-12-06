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
		BOOL IsValid ();
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