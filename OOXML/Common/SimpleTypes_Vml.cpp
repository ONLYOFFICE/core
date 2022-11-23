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
#include "SimpleTypes_Vml.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "../Base/Unit.h"

// Here are simple types Vml from spec Office Open Xml Part4 14
namespace SimpleTypes
{
	EEditAs CEditAs::FromString(const std::wstring &sValue)
	{
		this->m_eValue = editasCanvas;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'b':
			if      ( L"bullseye" == sValue ) this->m_eValue = editasBullseye;
			break;
		case 'c':
			if      ( L"canvas"   == sValue ) this->m_eValue = editasCanvas;
			else if ( L"cycle"    == sValue ) this->m_eValue = editasCycle;
			break;
		case 'o':
			if      ( L"orgchart" == sValue ) this->m_eValue = editasOrgchart;
			break;
		case 'r':
			if      ( L"radial"   == sValue ) this->m_eValue = editasRadial;
			break;
		case 's':
			if      ( L"stacked"  == sValue ) this->m_eValue = editasStacked;
			break;
		case 'v':
			if      ( L"venn"     == sValue ) this->m_eValue = editasVenn;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CEditAs::ToString() const
	{
		switch(this->m_eValue)
		{
		case editasBullseye : return L"bullseye";
		case editasCanvas   : return L"canvas";
		case editasCycle    : return L"cycle";
		case editasOrgchart : return L"orgchart";
		case editasRadial   : return L"radial";
		case editasStacked  : return L"stacked";
		case editasVenn     : return L"venn";
		default             : return L"canvas";
		}
	}

	//--------------------------------------------------------------------------------
	// Ext 14.1.3.2 (Part 4)
	//--------------------------------------------------------------------------------

	EExt CExt::FromString(const std::wstring &sValue)
	{
		this->m_eValue = extView;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'b':
			if      ( L"backwardCompatible" == sValue ) this->m_eValue = extBackwardCompatible;
			break;
		case 'e':
			if      ( L"edit"				== sValue ) this->m_eValue = extEdit;
			break;
		case 'v':
			if      ( L"view"				== sValue ) this->m_eValue = extView;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CExt::ToString  () const
	{
		switch(this->m_eValue)
		{
		case extBackwardCompatible : return L"backwardCompatible";
		case extEdit				: return L"edit";
		case extView				: return L"view";
		default						: return L"view";
		}
	}

	//--------------------------------------------------------------------------------
	// FillMethod 14.1.3.3 (Part 4)
	//--------------------------------------------------------------------------------

	EFillMethod CFillMethod::FromString(const std::wstring& sValue)
	{
		if		(L"any"		== sValue)	this->m_eValue = fillmethodAny;
		else if	(L"linear"	== sValue)	this->m_eValue = fillmethodLinear;
		else if	(L"linear sigma"== sValue)	this->m_eValue = fillmethodLinearSigma;
		else if (L"sigma"	== sValue)	this->m_eValue = fillmethodSigma;
		else if (L"none"	== sValue)	this->m_eValue = fillmethodNone;
		else				this->m_eValue = fillmethodNone;

		return this->m_eValue;
	}

	std::wstring CFillMethod::ToString() const
	{
		switch (this->m_eValue)
		{
		case fillmethodAny	        : return L"any";
		case fillmethodLinear	    : return L"linear";
		case fillmethodLinearSigma	: return L"linearSigma";
		case fillmethodSigma		: return L"sigma";
		case fillmethodNone			: return L"none";
		default						: return L"none";
		}
	}

	//--------------------------------------------------------------------------------
	// FillType 14.1.3.4;14.2.3.14 (Part 4)
	//--------------------------------------------------------------------------------

	void    CFillType::SetValue  (EFillType eValue)
	{
		if ( filltypeFrame == eValue || filltypeGradient == eValue || eValue == filltypeGradientRadial ||
			 filltypePattern == eValue || filltypeSolid == eValue || filltypeTile == eValue )
			this->m_eValue = eValue;
		else
			this->m_eValue = filltypeSolid;
	}

	EFillType CFillType::FromString(const std::wstring &sValue)
	{
		if      (L"frame"			== sValue)	this->m_eValue = filltypeFrame;
		else if (L"gradient"		== sValue)	this->m_eValue = filltypeGradient;
		else if (L"gradientRadial"	== sValue)	this->m_eValue = filltypeGradientRadial;
		else if (L"pattern"			== sValue)	this->m_eValue = filltypePattern;
		else if (L"solid"			== sValue)	this->m_eValue = filltypeSolid;
		else if (L"tile"			== sValue)	this->m_eValue = filltypeTile;
		else						this->m_eValue = filltypeSolid;

		return this->m_eValue;
	}

	std::wstring CFillType::ToString() const
	{
		switch (this->m_eValue)
		{
		case filltypeFrame			: return L"frame";
		case filltypeGradient		: return L"gradient";
		case filltypeGradientRadial	: return L"gradientRadial";
		case filltypePattern		: return L"pattern";
		case filltypeSolid			: return L"solid";
		case filltypeTile			: return L"tile";
		default						: return L"solid";
		}
	}

	//--------------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------------

	EFillType CFillType1::FromString(const std::wstring &sValue)
	{
		if      (L"background"		== sValue)	this->m_eValue = filltypeBackground;
		else if (L"frame"			== sValue)	this->m_eValue = filltypeFrame;
		else if (L"gradient"		== sValue)	this->m_eValue = filltypeGradient;
		else if (L"gradientCenter"  == sValue)	this->m_eValue = filltypeGradientCenter;
		else if (L"gradientRadial"	== sValue)	this->m_eValue = filltypeGradientRadial;
		else if (L"gradientUnscaled" == sValue)	this->m_eValue = filltypeGradientUnscaled;
		else if (L"pattern"			== sValue)	this->m_eValue = filltypePattern;
		else if (L"solid"			== sValue)	this->m_eValue = filltypeSolid;
		else if (L"tile"			== sValue)	this->m_eValue = filltypeTile;
		else						this->m_eValue = filltypeSolid;

		return this->m_eValue;
	}

	std::wstring CFillType1::ToString() const
	{
		switch (this->m_eValue)
		{
		case filltypeBackground		  : return L"background";
		case filltypeFrame			  : return L"frame";
		case filltypeGradient		  : return L"gradient";
		case filltypeGradientCenter	  : return L"gradientCenter";
		case filltypeGradientRadial	  : return L"gradientRadial";
		case filltypeGradientUnscaled : return L"gradientUnscaled";
		case filltypePattern		  : return L"pattern";
		case filltypeSolid			  : return L"solid";
		case filltypeTile			  : return L"tile";
		default						: return L"solid";
		}
	}

	//--------------------------------------------------------------------------------
	// ImageAspect 14.1.3.5 (Part 4)
	//--------------------------------------------------------------------------------

	EImageAspect CImageAspect::FromString(const std::wstring &sValue)
	{
		this->m_eValue = imageaspectIgnore;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'a':
			if      ( L"atLeast" == sValue ) this->m_eValue = imageaspectAtLeast;
			else if ( L"atMost"  == sValue ) this->m_eValue = imageaspectAtMost;
			break;
		case 'i':
			if      ( L"ignore"  == sValue ) this->m_eValue = imageaspectIgnore;
			break;
		}

		return this->m_eValue;
	}

	std::wstring  CImageAspect::ToString  () const
	{
		switch(this->m_eValue)
		{
		case imageaspectAtLeast : return L"atLeast";
		case imageaspectAtMost  : return L"atMost";
		case imageaspectIgnore  : return L"ignore";
		default					: return L"ignore";
		}
	}

	// -------------------------------------------------------------------------------
	// ShadowType 14.1.3.6 (Part 4)
	// -------------------------------------------------------------------------------

	EShadowType CShadowType::FromString(const std::wstring &sValue)
	{
		if      (L"double"		== sValue)	this->m_eValue = shadowtypeDouble;
		else if (L"emboss"		== sValue)	this->m_eValue = shadowtypeEmboss;
		else if (L"perspective"	== sValue)	this->m_eValue = shadowtypePerspective;
		else if (L"single"		== sValue)	this->m_eValue = shadowtypeSingle;
		else								this->m_eValue = shadowtypeSingle;

		return this->m_eValue;
	}

	std::wstring CShadowType::ToString() const
	{
		switch (this->m_eValue)
		{
		case shadowtypeDouble	    : return L"double";
		case shadowtypeEmboss	    : return L"emboss";
		case shadowtypePerspective	: return L"perspective";
		case shadowtypeSingle		: return L"single";
		default						: return L"single";
		}
	}

	//--------------------------------------------------------------------------------
	// StrokeArrowLength 14.1.3.7 (Part 4)
	//--------------------------------------------------------------------------------

	EStrokeArrowLength CStrokeArrowLength::FromString(const std::wstring &sValue)
	{
		this->m_eValue = strokearrowlengthMedium;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'l':
			if ( L"long"   == sValue ) this->m_eValue = strokearrowlengthLong;
			break;
		case 'm':
			if ( L"medium" == sValue ) this->m_eValue = strokearrowlengthMedium;
			break;
		case 's':
			if ( L"short"  == sValue ) this->m_eValue = strokearrowlengthShort;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CStrokeArrowLength::ToString() const
	{
		switch(this->m_eValue)
		{
		case strokearrowlengthLong   : return L"long";
		case strokearrowlengthMedium : return L"medium";
		case strokearrowlengthShort  : return L"short";
		default					: return L"medium";
		}
	}

	//--------------------------------------------------------------------------------
	// StrokeArrowType 14.1.3.8 (Part 4)
	//--------------------------------------------------------------------------------

	EStrokeArrowType CStrokeArrowType::FromString(const std::wstring &sValue)
	{
		this->m_eValue = strokearrowtypeNone;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'b':
			if      ( L"block"   == sValue ) this->m_eValue = strokearrowtypeBlock;
			break;
		case 'c':
			if      ( L"classic" == sValue ) this->m_eValue = strokearrowtypeClassic;
			break;
		case 'd':
			if      ( L"diamond" == sValue ) this->m_eValue = strokearrowtypeDiamond;
			break;
		case 'n':
			if      ( L"none"    == sValue ) this->m_eValue = strokearrowtypeNone;
			break;
		case 'o':
			if      ( L"open"    == sValue ) this->m_eValue = strokearrowtypeOpen;
			else if ( L"oval"    == sValue ) this->m_eValue = strokearrowtypeOval;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CStrokeArrowType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case strokearrowtypeBlock   : return L"block";
		case strokearrowtypeClassic : return L"classic";
		case strokearrowtypeDiamond : return L"diamond";
		case strokearrowtypeNone    : return L"none";
		case strokearrowtypeOpen    : return L"open";
		case strokearrowtypeOval    : return L"oval";
		default					: return L"none";
		}
	}

	//--------------------------------------------------------------------------------
	// StrokeArrowWidth 14.1.3.9 (Part 4)
	//--------------------------------------------------------------------------------

	EStrokeArrowWidth CStrokeArrowWidth::FromString(const std::wstring &sValue)
	{
		this->m_eValue = strokearrowwidthMedium;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'm':
			if ( L"medium" == sValue ) this->m_eValue = strokearrowwidthMedium;
			break;
		case 'n':
			if ( L"narrow" == sValue ) this->m_eValue = strokearrowwidthNarrow;
			break;
		case 'w':
			if ( L"wide"   == sValue ) this->m_eValue = strokearrowwidthWide;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CStrokeArrowWidth::ToString() const
	{
		switch(this->m_eValue)
		{
		case strokearrowwidthMedium : return L"medium";
		case strokearrowwidthNarrow : return L"narrow";
		case strokearrowwidthWide   : return L"wide";
		default					: return L"medium";
		}
	}

	//--------------------------------------------------------------------------------
	// StrokeEndCap 14.1.3.10 (Part 4)
	//--------------------------------------------------------------------------------

	EStrokeEndCap CStrokeEndCap::FromString(const std::wstring &sValue)
	{
		this->m_eValue = strokeendcapRound;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'f':
			if ( L"flat"   == sValue ) this->m_eValue = strokeendcapFlat;
			break;
		case 'r':
			if ( L"round"  == sValue ) this->m_eValue = strokeendcapRound;
			break;
		case 's':
			if ( L"square" == sValue ) this->m_eValue = strokeendcapSqaure;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CStrokeEndCap::ToString  () const
	{
		switch(this->m_eValue)
		{
		case strokeendcapFlat   : return L"flat";
		case strokeendcapRound  : return L"round";
		case strokeendcapSqaure : return L"square";
		default					: return L"round";
		}
	}

	//--------------------------------------------------------------------------------
	// StrokeJoinStyle 14.1.3.11 (Part 4)
	//--------------------------------------------------------------------------------

	EStrokeJoinStyle CStrokeJoinStyle::FromString(const std::wstring &sValue)
	{
		if      (L"round"	== sValue)	this->m_eValue = strokejoinstyleRound;
		else if (L"miter"	== sValue)	this->m_eValue = strokejoinstyleMiter;
		else if (L"bevel"	== sValue)	this->m_eValue = strokejoinstyleMiter;
		else							this->m_eValue = strokejoinstyleRound;

		return this->m_eValue;
	}

	std::wstring CStrokeJoinStyle::ToString() const
	{
		switch (this->m_eValue)
		{
		case strokejoinstyleRound	: return L"round";
		case strokejoinstyleMiter	: return L"miter";
		case strokejoinstyleBevel	: return L"bevel";
		default						: return L"round";
		}
	}

	//--------------------------------------------------------------------------------
	// StrokeLineStyle 14.1.3.12 (Part 4)
	//--------------------------------------------------------------------------------

	EStrokeLineStyle CStrokeLineStyle::FromString(const std::wstring &sValue)
	{
		this->m_eValue = strokelinestyleSingle;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 's':
			if      ( L"single"           == sValue ) this->m_eValue = strokelinestyleSingle;
			break;
		case 't':
			if      ( L"thickBetweenThin" == sValue ) this->m_eValue = strokelinestyleThickBetweenThin;
			else if ( L"thickThin"        == sValue ) this->m_eValue = strokelinestyleThickThin;
			else if ( L"thinThick"        == sValue ) this->m_eValue = strokelinestyleThinThick;
			else if ( L"thinThin"         == sValue ) this->m_eValue = strokelinestyleThinThin;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CStrokeLineStyle::ToString  () const
	{
		switch(this->m_eValue)
		{
		case strokelinestyleSingle            : return L"single";
		case strokelinestyleThickBetweenThin  : return L"thickBetweenThin";
		case strokelinestyleThickThin         : return L"thickThin";
		case strokelinestyleThinThick         : return L"thinThick";
		case strokelinestyleThinThin          : return L"thinThin";
		default					      : return L"single";
		}
	}
}
namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// AlternateMathContentType 14.2.3.1 (Part 4)
	//--------------------------------------------------------------------------------

	EAlternateMathContentType CAlternateMathContentType::FromString(const std::wstring &sValue)
	{
		this->m_eValue = alternatemathcontenttypeOfficeOpenXmlMath;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'o':
			if ( L"officeopenxmlmath" == sValue ) this->m_eValue = alternatemathcontenttypeOfficeOpenXmlMath;
			break;
		case 'm':
			if ( L"mathml"            == sValue ) this->m_eValue = alternatemathcontenttypeMathMl;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CAlternateMathContentType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case alternatemathcontenttypeOfficeOpenXmlMath: return L"officeopenxmlmath";
		case alternatemathcontenttypeMathMl:            return L"mathml";
		default :										return L"officeopenxmlmath";
		}
	}

	//--------------------------------------------------------------------------------
	// VmlAngle 14.2.3.2 (Part 4)
	//--------------------------------------------------------------------------------

	EVmlAngle CVmlAngle::FromString(const std::wstring &sValue)
	{
		this->m_eValue = vmlangleAuto;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case '3':
			if      ( L"30"   == sValue ) this->m_eValue = vmlangle30;
			break;
		case '4':
			if      ( L"45"   == sValue ) this->m_eValue = vmlangle45;
			break;
		case '6':
			if      ( L"60"   == sValue ) this->m_eValue = vmlangle60;
			break;
		case '9':
			if      ( L"90"   == sValue ) this->m_eValue = vmlangle90;
			break;
		case 'a':
			if      ( L"any"  == sValue ) this->m_eValue = vmlangleAny;
			else if ( L"auto" == sValue ) this->m_eValue = vmlangleAuto;
			break;

		}

		return this->m_eValue;
	}

	std::wstring CVmlAngle::ToString() const
	{
		switch(this->m_eValue)
		{
		case vmlangle30:   return L"30";
		case vmlangle45:   return L"45";
		case vmlangle60:   return L"60";
		case vmlangle90:   return L"90";
		case vmlangleAny:  return L"any";
		case vmlangleAuto: return L"auto";
		default :          return L"auto";
		}
	}

	//--------------------------------------------------------------------------------
	// BWMode 14.2.3.3 (Part 4)
	//--------------------------------------------------------------------------------

	EBWMode CBWMode::FromString(const std::wstring &sValue)
	{
		this->m_eValue = bwmodeAuto;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'a':
			if      ( L"auto"              == sValue ) this->m_eValue = bwmodeAuto;
			break;
		case 'b':
			if      ( L"black"             == sValue ) this->m_eValue = bwmodeBlack;
			else if ( L"blackTextAndLines" == sValue ) this->m_eValue = bwmodeBlackTextAndLines;
			break;
		case 'c':
			if      ( L"color"             == sValue ) this->m_eValue = bwmodeColor;
			break;
		case 'g':
			if      ( L"grayOutline"       == sValue ) this->m_eValue = bwmodeGrayOutline;
			else if ( L"grayScale"         == sValue ) this->m_eValue = bwmodeGrayScale;
			break;
		case 'h':
			if      ( L"hide"              == sValue ) this->m_eValue = bwmodeHide;
			else if ( L"highContrast"      == sValue ) this->m_eValue = bwmodeHighContrast;
			break;
		case 'i':
			if      ( L"inverseGray"       == sValue ) this->m_eValue = bwmodeInverseGray;
			break;
		case 'l':
			if      ( L"lightGrayscale"    == sValue ) this->m_eValue = bwmodeLightGrayscale;
			break;
		case 'u':
			if      ( L"undrawn"           == sValue ) this->m_eValue = bwmodeUndrawn;
			break;
		case 'w':
			if      ( L"white"             == sValue ) this->m_eValue = bwmodeWhite;
			break;

		}

		return this->m_eValue;
	}

	std::wstring CBWMode::ToString() const
	{
		switch(this->m_eValue)
		{
		case bwmodeAuto:              return L"auto";
		case bwmodeBlack:             return L"black";
		case bwmodeBlackTextAndLines: return L"blackTextAndLines";
		case bwmodeColor:             return L"color";
		case bwmodeGrayOutline:       return L"grayOutline";
		case bwmodeGrayScale:         return L"grayScale";
		case bwmodeHide:              return L"hide";
		case bwmodeHighContrast:      return L"highContrast";
		case bwmodeInverseGray:       return L"inverseGray";
		case bwmodeLightGrayscale:    return L"lightGrayscale";
		case bwmodeUndrawn:           return L"undrawn";
		case bwmodeWhite:             return L"white";
		default :                     return L"auto";
		}
	}

	//--------------------------------------------------------------------------------
	// CalloutDrop 14.2.3.4 (Part 4)
	//--------------------------------------------------------------------------------

	CCalloutDrop::CCalloutDrop() {}

	std::wstring CCalloutDrop::GetValue() const
	{
		return m_sValue;
	}

	void CCalloutDrop::SetValue(std::wstring &sValue)
	{
		m_sValue = sValue;
	}


	std::wstring CCalloutDrop::FromString(const std::wstring &sValue)
	{
		m_sValue = sValue;

		return m_sValue;
	}

	std::wstring CCalloutDrop::ToString  () const
	{
		return m_sValue;
	}

	//--------------------------------------------------------------------------------
	// ColorMode 14.2.3.6 (Part 4)
	//--------------------------------------------------------------------------------

	EColorMode CColorMode::FromString(const std::wstring &sValue)
	{
		this->m_eValue = colormodeAuto;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'a':
			if ( L"auto"   == sValue ) this->m_eValue = colormodeAuto;
			break;
		case 'c':
			if ( L"custom" == sValue ) this->m_eValue = colormodeCustom;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CColorMode::ToString  () const
	{
		switch(this->m_eValue)
		{
		case colormodeAuto:   return L"auto";
		case colormodeCustom: return L"custom";
		default :             return L"auto";
		}
	}

	//--------------------------------------------------------------------------------
	// ConnectorType 14.2.3.7 (Part 4)
	//--------------------------------------------------------------------------------

	EConnectorType CConnectorType::FromString(const std::wstring &sValue)
	{
		this->m_eValue = connectortypeNone;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'c':
			if ( L"curved"   == sValue ) this->m_eValue = connectortypeCurved;
			break;
		case 'e':
			if ( L"elbow"    == sValue ) this->m_eValue = connectortypeElbow;
			break;
		case 'n':
			if ( L"none"     == sValue ) this->m_eValue = connectortypeNone;
			break;
		case 's':
			if ( L"straight" == sValue ) this->m_eValue = connectortypeStraight;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CConnectorType::ToString() const
	{
		switch(this->m_eValue)
		{
		case connectortypeCurved  : return L"curved";
		case connectortypeElbow   : return L"elbow";
		case connectortypeNone    : return L"none";
		case connectortypeStraight: return L"straight";
		default					  : return L"none";
		}
	}

	//--------------------------------------------------------------------------------
	// ConnectType 14.2.3.8 (Part 4)
	//--------------------------------------------------------------------------------

	EConnectType CConnectType::FromString(const std::wstring &sValue)
	{
		this->m_eValue = connecttypeNone;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'c':
			if ( L"custom"   == sValue ) this->m_eValue = connecttypeCustom;
			break;
		case 'n':
			if ( L"none"     == sValue ) this->m_eValue = connecttypeNone;
			break;
		case 'r':
			if ( L"rect"     == sValue ) this->m_eValue = connecttypeRect;
			break;
		case 's':
			if ( L"segments" == sValue ) this->m_eValue = connecttypeSegments;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CConnectType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case connecttypeCustom  : return L"custom";
		case connecttypeNone    : return L"none";
		case connecttypeRect    : return L"rect";
		case connecttypeSegments: return L"segments";
		default					: return L"none";
		}
	}

	//--------------------------------------------------------------------------------
	// ContentType 14.2.3.9 (Part 4)
	//--------------------------------------------------------------------------------

	CContentType::CContentType() {}

	std::wstring CContentType::GetValue() const
	{
		return m_sValue;
	}

	void CContentType::SetValue(std::wstring &sValue)
	{
		m_sValue = sValue;
	}

	std::wstring CContentType::FromString(const std::wstring &sValue)
	{
		m_sValue = sValue;

		return m_sValue;
	}

	std::wstring CContentType::ToString  () const
	{
		return m_sValue;
	}

	//--------------------------------------------------------------------------------
	// DiagramLayout 14.2.3.10 (Part 4)
	//--------------------------------------------------------------------------------

	EDiagramLayout CDiagramLayout::FromString(const std::wstring &sValue)
	{
		this->m_eValue = diagramlayout0;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case '0':
			if ( L"0" == sValue ) this->m_eValue = diagramlayout0;
			break;
		case '1':
			if ( L"1" == sValue ) this->m_eValue = diagramlayout1;
			break;
		case '2':
			if ( L"2" == sValue ) this->m_eValue = diagramlayout2;
			break;
		case '3':
			if ( L"3" == sValue ) this->m_eValue = diagramlayout3;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CDiagramLayout::ToString  () const
	{
		switch(this->m_eValue)
		{
		case diagramlayout0 : return L"0";
		case diagramlayout1 : return L"1";
		case diagramlayout2 : return L"2";
		case diagramlayout3 : return L"3";
		default             : return L"0";
		}
	}

	//--------------------------------------------------------------------------------
	// ExtrusionPlane 14.2.3.11 (Part 4)
	//--------------------------------------------------------------------------------

	EExtrusionPlane CExtrusionPlane::FromString(const std::wstring &sValue)
	{
		this->m_eValue = extrusionplaneXY;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'X':
			if ( L"XY" == sValue ) this->m_eValue = extrusionplaneXY;
			break;
		case 'Y':
			if ( L"YZ" == sValue ) this->m_eValue = extrusionplaneYZ;
			break;
		case 'Z':
			if ( L"ZX" == sValue ) this->m_eValue = extrusionplaneZX;
			break;
		case 'x':
			if ( L"xy" == sValue ) this->m_eValue = extrusionplaneXY;
			break;
		case 'y':
			if ( L"yz" == sValue ) this->m_eValue = extrusionplaneYZ;
			break;
		case 'z':
			if ( L"zx" == sValue ) this->m_eValue = extrusionplaneZX;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CExtrusionPlane::ToString  () const
	{
		switch(this->m_eValue)
		{
		case extrusionplaneXY: return L"XY";
		case extrusionplaneYZ: return L"YZ";
		case extrusionplaneZX: return L"ZX";
		default :				return L"XY";
		}
	}

	//--------------------------------------------------------------------------------
	// ExtrusionRender 14.2.3.12 (Part 4)
	//--------------------------------------------------------------------------------

	EExtrusionRender CExtrusionRender::FromString(const std::wstring &sValue)
	{
		this->m_eValue = extrusionrenderSolid;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'b':
			if ( L"boundingCube" == sValue ) this->m_eValue = extrusionrenderBoundingCube;
			break;
		case 's':
			if ( L"solid"        == sValue ) this->m_eValue = extrusionrenderSolid;
			break;
		case 'w':
			if ( L"wireFrame"    == sValue ) this->m_eValue = extrusionrenderWireFrame;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CExtrusionRender::ToString  () const
	{
		switch(this->m_eValue)
		{
		case extrusionrenderBoundingCube: return L"boundingCube";
		case extrusionrenderSolid:        return L"solid";
		case extrusionrenderWireFrame:    return L"wireFrame";
		default :					return L"solid";
		}
	}

	//--------------------------------------------------------------------------------
	// ExtructionType 14.2.3.13 (Part 4)
	//--------------------------------------------------------------------------------

	EExtrusionType CExtrusionType::FromString(const std::wstring& sValue)
	{
		if		(L"parallel"    == sValue ) this->m_eValue = extrusiontypeParallel;
		else if	(L"perspective" == sValue ) this->m_eValue = extrusiontypePerspective;
		else								this->m_eValue = extrusiontypeParallel;

		return this->m_eValue;
	}

	std::wstring CExtrusionType::ToString() const
	{
		switch (this->m_eValue)
		{
		case extrusiontypeParallel		: return L"parallel";
		case extrusiontypePerspective	: return L"perspective";
		default							: return L"parallel";
		}
	}

	//--------------------------------------------------------------------------------
	// How 14.2.3.15 (Part 4)
	//--------------------------------------------------------------------------------

	EHow CHow::FromString(const std::wstring &sValue)
	{
		this->m_eValue = howTop;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'b':
			if ( L"bottom" == sValue ) this->m_eValue = howBottom;
			break;
		case 'c':
			if ( L"center" == sValue ) this->m_eValue = howCenter;
			break;
		case 'l':
			if ( L"left"   == sValue ) this->m_eValue = howLeft;
			break;
		case 'm':
			if ( L"middle" == sValue ) this->m_eValue = howMiddle;
			break;
		case 'r':
			if ( L"right"  == sValue ) this->m_eValue = howRight;
			break;
		case 't':
			if ( L"top"    == sValue ) this->m_eValue = howTop;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CHow::ToString  () const
	{
		switch(this->m_eValue)
		{
		case howBottom: return L"bottom";
		case howCenter: return L"center";
		case howLeft  : return L"left";
		case howMiddle: return L"middle";
		case howRight : return L"right";
		case howTop   : return L"top";
		default       : return L"top";
		}
	}

	//--------------------------------------------------------------------------------
	// HrAlign 14.2.3.16 (Part 4)
	//--------------------------------------------------------------------------------

	EHrAlign CHrAlign::FromString(const std::wstring &sValue)
	{
		this->m_eValue = hralignLeft;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'c':
			if ( L"center" == sValue ) this->m_eValue = hralignCenter;
			break;
		case 'l':
			if ( L"left"   == sValue ) this->m_eValue = hralignLeft;
			break;
		case 'r':
			if ( L"right"  == sValue ) this->m_eValue = hralignRight;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CHrAlign::ToString  () const
	{
		switch(this->m_eValue)
		{
		case hralignCenter: return L"center";
		case hralignLeft  : return L"left";
		case hralignRight : return L"right";
		default           : return L"left";
		}
	}

	//--------------------------------------------------------------------------------
	// InsetMode 14.2.3.17 (Part 4)
	//--------------------------------------------------------------------------------

	EInsetMode CInsetMode::FromString(const std::wstring &sValue)
	{
		this->m_eValue = insetmodeAuto;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'a':
			if ( L"auto"   == sValue ) this->m_eValue = insetmodeAuto;
			break;
		case 'c':
			if ( L"custom" == sValue ) this->m_eValue = insetmodeCustom;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CInsetMode::ToString  () const
	{
		switch(this->m_eValue)
		{
		case insetmodeAuto   : return L"auto";
		case insetmodeCustom : return L"custom";
		default					: return L"auto";
		}
	}

	//--------------------------------------------------------------------------------
	// OLEDrawAspect 14.2.3.18 (Part 4)
	//--------------------------------------------------------------------------------

	EOLEDrawAspect COLEDrawAspect::FromString(const std::wstring &sValue)
	{
		if      (L"Content"	== sValue)	this->m_eValue = oledrawaspectContent;
		else if (L"Icon"	== sValue)	this->m_eValue = oledrawaspectIcon;
		else							this->m_eValue = oledrawaspectIcon;

		return this->m_eValue;
	}

	std::wstring COLEDrawAspect::ToString() const
	{
		switch (this->m_eValue)
		{
		case oledrawaspectContent	: return L"Content";
		case oledrawaspectIcon		: return L"Icon";
		default						: return L"Content";
		}
	}

	//--------------------------------------------------------------------------------
	// OLELinkType 14.2.3.19 (Part 4)

	EOLELinkType COLELinkType::FromString(const std::wstring &sValue)
	{
		this->m_eValue = olelinktypeBitmap;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'B':
			if      ( L"Bitmap"           == sValue ) this->m_eValue = olelinktypeBitmap;
			break;
		case 'E':
			if      ( L"EnhancedMetaFile" == sValue ) this->m_eValue = olelinktypeEmf;
			break;
		case 'J':
			if      ( L"Jpeg"             == sValue ) this->m_eValue = olelinktypeJpeg;
			break;
		case 'P':
			if      ( L"Picture"          == sValue ) this->m_eValue = olelinktypePicture;
			else if ( L"Png"			  == sValue ) this->m_eValue = olelinktypePng;
			break;
		}

		return this->m_eValue;
	}

	std::wstring COLELinkType::ToString() const
	{
		switch(this->m_eValue)
		{
		case olelinktypeBitmap:  return L"Bitmap";
		case olelinktypeEmf:     return L"EnhancedMetaFile";
		case olelinktypeJpeg:    return L"Jpeg";
		case olelinktypePicture: return L"Picture";
		case olelinktypePng:     return L"Png";
		default :				 return L"Bitmap";
		}
	}

	//--------------------------------------------------------------------------------
	// OleType 14.2.3.20 (Part 4)
	//--------------------------------------------------------------------------------

	EOLEType COLEType::FromString(const std::wstring &sValue)
	{
		if      (L"embed"	== sValue)	this->m_eValue = oletypeEmbed;
		else if (L"link"	== sValue)	this->m_eValue = oletypeLink;
		else							this->m_eValue = oletypeEmbed;

		return this->m_eValue;
	}

	std::wstring COLEType::ToString() const
	{
		switch (this->m_eValue)
		{
		case oletypeEmbed	: return L"embed";
		case oletypeLink	: return L"link";
		default				: return L"embed";
		}
	}

	//--------------------------------------------------------------------------------
	// OLEUpdateMode 14.2.3.21 (Part 4)
	//--------------------------------------------------------------------------------

	EOLEUpdateMode COLEUpdateMode::FromString(const std::wstring &sValue)
	{
		if      (L"Always"	== sValue)	this->m_eValue = oleupdatemodeAlways;
		else if (L"OnCall"	== sValue)	this->m_eValue = oleupdatemodeOnCall;
		else							this->m_eValue = oleupdatemodeAlways;

		return this->m_eValue;
	}

	std::wstring COLEUpdateMode::ToString() const
	{
		switch (this->m_eValue)
		{
		case oleupdatemodeAlways : return L"Always";
		case oleupdatemodeOnCall : return L"OnCall";
		default				     : return L"Always";
		}
	}

	//--------------------------------------------------------------------------------
	// RType 14.2.3.22 (Part 4)
	//--------------------------------------------------------------------------------

	ERType CRType::FromString(const std::wstring &sValue)
	{
		this->m_eValue = rtypeAlign;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'a':
			if      ( L"align"     == sValue ) this->m_eValue = rtypeAlign;
			else if ( L"arc"       == sValue ) this->m_eValue = rtypeArc;
			break;
		case 'c':
			if      ( L"callout"   == sValue ) this->m_eValue = rtypeCallout;
			else if ( L"connector" == sValue ) this->m_eValue = rtypeConnector;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CRType::ToString() const
	{
		switch(this->m_eValue)
		{
		case rtypeAlign    : return L"align";
		case rtypeArc      : return L"arc";
		case rtypeCallout  : return L"callout";
		case rtypeConnector: return L"connector";
		default            : return L"align";
		}
	}

	//--------------------------------------------------------------------------------
	// ScreenSize 14.2.3.23 (Part 4)
	//--------------------------------------------------------------------------------

	EScreenSize CScreenSize::FromString(const std::wstring &sValue)
	{
		this->m_eValue = screensize640x480;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case '1':
			if      ( L"1024,768"  == sValue ) this->m_eValue = screensize1024x768;
			else if ( L"1152,862"  == sValue ) this->m_eValue = screensize1152x862;
			break;
		case '5':
			if      ( L"544,376"   == sValue ) this->m_eValue = screensize544x376;
			break;
		case '6':
			if      ( L"640,480"   == sValue ) this->m_eValue = screensize640x480;
			break;
		case '7':
			if      ( L"720,512"   == sValue ) this->m_eValue = screensize720x512;
			break;
		case '8':
			if      ( L"800,600"   == sValue ) this->m_eValue = screensize800x600;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CScreenSize::ToString  () const
	{
		switch(this->m_eValue)
		{
		case screensize1024x768: return L"1024,768";
		case screensize1152x862: return L"1152,862";
		case screensize544x376 : return L"544,376";
		case screensize640x480 : return L"640,480";
		case screensize720x512 : return L"720,512";
		case screensize800x600 : return L"800,600";
		default					: return L"640,480";
		}
	}

} // SimpleTypes


namespace SimpleTypes
{
	//--------------------------------------------------------------------------------
	// BorderShadow 14.3.3.1 (Part 4)
	//--------------------------------------------------------------------------------

	EBorderShadow CBorderShadow::FromString(const std::wstring &sValue)
	{
		this->m_eValue = bordershadowFalse;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'f':
			if ( L"f" == sValue || L"false" == sValue ) this->m_eValue = bordershadowFalse;
			break;
		case 't':
			if ( L"t" == sValue || L"true" == sValue  ) this->m_eValue = bordershadowTrue;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CBorderShadow::ToString  () const
	{
		switch(this->m_eValue)
		{
		case bordershadowTrue:  return L"t";
		case bordershadowFalse: return L"f";
		default :				return L"f";
		}
	}

	//--------------------------------------------------------------------------------
	// BorderType 14.3.3.2 (Part 4)
	//--------------------------------------------------------------------------------

	EBorderType CBorderType::FromString(const std::wstring &sValue)
	{
		this->m_eValue = bordertypeNone;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'd':
			if      ( L"dash"           == sValue ) this->m_eValue = bordertypeDash;
			else if ( L"dashDotDot"     == sValue ) this->m_eValue = bordertypeDashDotDot;
			else if ( L"dashDotStroked" == sValue ) this->m_eValue = bordertypeDashDotStroked;
			else if ( L"dashedSmall"    == sValue ) this->m_eValue = bordertypeDashedSmall;
			else if ( L"dot"            == sValue ) this->m_eValue = bordertypeDot;
			else if ( L"dotDash"        == sValue ) this->m_eValue = bordertypeDotDash;
			else if ( L"double"         == sValue ) this->m_eValue = bordertypeDouble;
			else if ( L"doubleWave"     == sValue ) this->m_eValue = bordertypeDoubleWave;
			break;
		case 'h':
			if      ( L"hairline"      == sValue ) this->m_eValue = bordertypeHairline;
			break;

		case 'H':
			if      ( L"HTMLInset"     == sValue ) this->m_eValue = bordertypeHTMLInset;
			else if ( L"HTMLOutset"    == sValue ) this->m_eValue = bordertypeHTMLOutset;
			break;

		case 'n':
			if      ( L"none"          == sValue ) this->m_eValue = bordertypeNone;
			break;

		case 's':
			if      ( L"single"        == sValue ) this->m_eValue = bordertypeSingle;
			break;

		case 't':
			if      ( L"thick"					== sValue ) this->m_eValue = bordertypeThick;
			else if ( L"thickBetweenThin"      == sValue ) this->m_eValue = bordertypeThickBetweenThin;
			else if ( L"thickBetweenThinLarge" == sValue ) this->m_eValue = bordertypeThickBetweenThinLarge;
			else if ( L"thickBetweenThinSmall" == sValue ) this->m_eValue = bordertypeThickBetweenThinSmall;
			else if ( L"thickThin"             == sValue ) this->m_eValue = bordertypeThickThin;
			else if ( L"thickThinLarge"        == sValue ) this->m_eValue = bordertypeThickThinLarge;
			else if ( L"thickThinSmall"        == sValue ) this->m_eValue = bordertypeThickThinSmall;
			else if ( L"thinThick"             == sValue ) this->m_eValue = bordertypeThinThick;
			else if ( L"thinThickLarge"        == sValue ) this->m_eValue = bordertypeThinThickLarge;
			else if ( L"thinThickSmall"        == sValue ) this->m_eValue = bordertypeThinThickSmall;
			else if ( L"threeDEmboss"          == sValue ) this->m_eValue = bordertypeThreeDEmboss;
			else if ( L"threeDEngrave"         == sValue ) this->m_eValue = bordertypeThreeDEngrave;
			else if ( L"triple"					== sValue ) this->m_eValue = bordertypeTriple;
			break;

		case 'w':
			if      ( L"wave"					 == sValue ) this->m_eValue = bordertypeWave;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CBorderType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case bordertypeDash:				return L"dash";
		case bordertypeDashDotDot:          return L"dashDotDot";
		case bordertypeDashDotStroked:      return L"dashDotStroked";
		case bordertypeDashedSmall:         return L"dashedSmall";
		case bordertypeDot:					return L"dot";
		case bordertypeDotDash:				return L"dotDash";
		case bordertypeDouble:				return L"double";
		case bordertypeDoubleWave:          return L"doubleWave";
		case bordertypeHairline:			return L"hairline";
		case bordertypeHTMLInset:           return L"HTMLInset";
		case bordertypeHTMLOutset:          return L"HTMLOutset";
		case bordertypeNone:				return L"none";
		case bordertypeSingle:				return L"single";
		case bordertypeThick:				return L"thick";
		case bordertypeThickBetweenThin:      return L"thickBetweenThin";
		case bordertypeThickBetweenThinLarge: return L"thickBetweenThinLarge";
		case bordertypeThickBetweenThinSmall: return L"thickBetweenThinSmall";
		case bordertypeThickThin:             return L"thickThin";
		case bordertypeThickThinLarge:        return L"thickThinLarge";
		case bordertypeThickThinSmall:        return L"thickThinSmall";
		case bordertypeThinThick:             return L"thinThick";
		case bordertypeThinThickLarge:        return L"thinThickLarge";
		case bordertypeThinThickSmall:        return L"thinThickSmall";
		case bordertypeThreeDEmboss:          return L"threeDEmboss";
		case bordertypeThreeDEngrave:         return L"threeDEngrave";
		case bordertypeTriple:				 return L"triple";
		case bordertypeWave:				 return L"wave";
		default :					    return L"none";
		}
	}

	//--------------------------------------------------------------------------------
	// HorizontalAnchor 14.3.3.3 (Part 4)
	//--------------------------------------------------------------------------------

	EHorizontalAnchor CHorizontalAnchor::FromString(const std::wstring &sValue)
	{
		this->m_eValue = horizontalanchorChar;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'c':
			if ( L"char"   == sValue ) this->m_eValue = horizontalanchorChar;
			break;
		case 'm':
			if ( L"margin" == sValue ) this->m_eValue = horizontalanchorMargin;
			break;

		case 'p':
			if ( L"page"   == sValue ) this->m_eValue = horizontalanchorPage;
			break;

		case 't':
			if ( L"text"   == sValue ) this->m_eValue = horizontalanchorText;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CHorizontalAnchor::ToString  () const
	{
		switch(this->m_eValue)
		{
		case horizontalanchorChar:   return L"char";
		case horizontalanchorMargin: return L"margin";
		case horizontalanchorPage:   return L"page";
		case horizontalanchorText:   return L"text";
		default :					   return L"char";
		}
	}

	//--------------------------------------------------------------------------------
	// VerticalAnchor 14.3.3.4 (Part 4)
	//--------------------------------------------------------------------------------

	EVerticalAnchor CVerticalAnchor::FromString(const std::wstring &sValue)
	{
		this->m_eValue = verticalanchorLine;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'l':
			if ( L"line"   == sValue ) this->m_eValue = verticalanchorLine;
			break;
		case 'm':
			if ( L"margin" == sValue ) this->m_eValue = verticalanchorMargin;
			break;

		case 'p':
			if ( L"page"   == sValue ) this->m_eValue = verticalanchorPage;
			break;

		case 't':
			if ( L"text"   == sValue ) this->m_eValue = verticalanchorText;
			break;
		}

		return this->m_eValue;
	}

	std::wstring CVerticalAnchor::ToString  () const
	{
		switch(this->m_eValue)
		{
		case verticalanchorLine:   return L"line";
		case verticalanchorMargin: return L"margin";
		case verticalanchorPage:   return L"page";
		case verticalanchorText:   return L"text";
		default :					 return L"line";
		}
	}

	//--------------------------------------------------------------------------------
	// WrapSide 14.3.3.5 (Part 4)
	//--------------------------------------------------------------------------------

	EWrapSide CWrapSide::FromString(const std::wstring &sValue)
	{
		this->m_eValue = wrapsideBoth;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'b':
			if      ( L"both"    == sValue ) this->m_eValue = wrapsideBoth;
			break;
		case 'l':
			if      ( L"largest" == sValue ) this->m_eValue = wrapsideLargest;
			else if ( L"left"    == sValue ) this->m_eValue = wrapsideLeft;
			break;

		case 'r':
			if      ( L"right"   == sValue ) this->m_eValue = wrapsideRight;
			break;

		}

		return this->m_eValue;
	}

	std::wstring CWrapSide::ToString  () const
	{
		switch(this->m_eValue)
		{
		case wrapsideBoth:    return L"both";
		case wrapsideLargest: return L"largest";
		case wrapsideLeft:    return L"left";
		case wrapsideRight:   return L"right";
		default :             return L"both";
		}
	}

	//--------------------------------------------------------------------------------
	// WrapType 14.3.3.6 (Part 4)
	//--------------------------------------------------------------------------------

	EWrapType CWrapType::FromString(const std::wstring &sValue)
	{
		this->m_eValue = wraptypeNone;

		if ( sValue.empty() )
			return this->m_eValue;

		wchar_t wChar = sValue[0];
		switch ( wChar )
		{
		case 'n':
			if      ( L"none"         == sValue ) this->m_eValue = wraptypeNone;
			break;
		case 's':
			if      ( L"square"       == sValue ) this->m_eValue = wraptypeSquare;
			break;

		case 't':
			if      ( L"through"      == sValue ) this->m_eValue = wraptypeThrough;
			else if ( L"tight"        == sValue ) this->m_eValue = wraptypeTight;
			else if ( L"topAndBottom" == sValue ) this->m_eValue = wraptypeTopAndBottom;
			break;

		}

		return this->m_eValue;
	}

	std::wstring CWrapType::ToString  () const
	{
		switch(this->m_eValue)
		{
		case wraptypeNone:         return L"none";
		case wraptypeSquare:       return L"square";
		case wraptypeThrough:      return L"through";
		case wraptypeTight:        return L"tight";
		case wraptypeTopAndBottom: return L"topAndBottom";
		default :					 return L"none";
		}
	}

} // SimpleTypes

// Дополнительные простые типы, не входящие в спецификацию
namespace SimpleTypes
{
	namespace Vml
	{
		EVmlClientDataObjectType CVmlClientDataObjectType::FromString(const std::wstring &sValue)
		{
			this->m_eValue = vmlclientdataobjecttypeButton;

			if ( sValue.empty() )
				return this->m_eValue;

			if      ( L"Button" == sValue ) this->m_eValue = vmlclientdataobjecttypeButton;
			else if ( L"Checkbox" == sValue ) this->m_eValue = vmlclientdataobjecttypeCheckbox;
			else if ( L"Dialog" == sValue ) this->m_eValue = vmlclientdataobjecttypeDialog;
			else if ( L"Drop" == sValue ) this->m_eValue = vmlclientdataobjecttypeDrop;
			else if ( L"Edit" == sValue ) this->m_eValue = vmlclientdataobjecttypeEdit;
			else if ( L"GBox" == sValue ) this->m_eValue = vmlclientdataobjecttypeGBox;
			else if ( L"Group" == sValue ) this->m_eValue = vmlclientdataobjecttypeGroup;
			else if ( L"Label" == sValue ) this->m_eValue = vmlclientdataobjecttypeLabel;
			else if ( L"LineA" == sValue ) this->m_eValue = vmlclientdataobjecttypeLineA;
			else if ( L"List" == sValue ) this->m_eValue = vmlclientdataobjecttypeList;
			else if ( L"Movie" == sValue ) this->m_eValue = vmlclientdataobjecttypeMovie;
			else if ( L"Note" == sValue ) this->m_eValue = vmlclientdataobjecttypeNote;
			else if ( L"Pict" == sValue ) this->m_eValue = vmlclientdataobjecttypePict;
			else if ( L"Radio" == sValue ) this->m_eValue = vmlclientdataobjecttypeRadio;
			else if ( L"Rect" == sValue ) this->m_eValue = vmlclientdataobjecttypeRect;
			else if ( L"RectA" == sValue ) this->m_eValue = vmlclientdataobjecttypeRectA;
			else if ( L"Scroll" == sValue ) this->m_eValue = vmlclientdataobjecttypeScroll;
			else if ( L"Shape" == sValue ) this->m_eValue = vmlclientdataobjecttypeShape;
			else if ( L"Spin" == sValue ) this->m_eValue = vmlclientdataobjecttypeSpin;
			return this->m_eValue;
		}

		std::wstring CVmlClientDataObjectType::ToString  () const
		{
			switch(this->m_eValue)
			{
			case vmlclientdataobjecttypeButton:		return L"Button";
			case vmlclientdataobjecttypeCheckbox:	return L"Checkbox";
			case vmlclientdataobjecttypeDialog:		return L"Dialog";
			case vmlclientdataobjecttypeDrop:		return L"Drop";
			case vmlclientdataobjecttypeEdit:		return L"Edit";
			case vmlclientdataobjecttypeGBox:		return L"GBox";
			case vmlclientdataobjecttypeGroup:      return L"Group";
			case vmlclientdataobjecttypeLabel:      return L"Label";
			case vmlclientdataobjecttypeLineA:      return L"LineA";
			case vmlclientdataobjecttypeList:		return L"List";
			case vmlclientdataobjecttypeMovie:      return L"Movie";
			case vmlclientdataobjecttypeNote:		return L"Note";
			case vmlclientdataobjecttypePict:		return L"Pict";
			case vmlclientdataobjecttypeRadio:      return L"Radio";
			case vmlclientdataobjecttypeRect:		return L"Rect";
			case vmlclientdataobjecttypeRectA:      return L"RectA";
			case vmlclientdataobjecttypeScroll:		return L"Scroll";
			case vmlclientdataobjecttypeShape:      return L"Shape";
			case vmlclientdataobjecttypeSpin:		return L"Spin";
			default :								return L"Button";
			}
		}

		//--------------------------------------------------------------------------------
		// VmlCalloutType 14.2.2.2 (type) (Part 4)
		//--------------------------------------------------------------------------------

		EVmlCalloutType CVmlCalloutType::FromString(const std::wstring &sValue)
		{
			this->m_eValue = vmlcallouttypeRectangle;

			if ( sValue.empty() )
				return this->m_eValue;

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 'r':
				if      ( L"rectangle"        == sValue ) this->m_eValue = vmlcallouttypeRectangle;
				else if ( L"roundedrectangle" == sValue ) this->m_eValue = vmlcallouttypeRoundRectangle;
				break;
			case 'o':
				if      ( L"oval"             == sValue ) this->m_eValue = vmlcallouttypeOval;
				break;
			case 'c':
				if      ( L"cloud"            == sValue ) this->m_eValue = vmlcallouttypeCloud;
				break;
			}

			return this->m_eValue;
		}

		std::wstring CVmlCalloutType::ToString  () const
		{
			switch(this->m_eValue)
			{
			case vmlcallouttypeRectangle:      return L"rectangle";
			case vmlcallouttypeRoundRectangle: return L"roundedrectangle";
			case vmlcallouttypeOval:           return L"oval";
			case vmlcallouttypeCloud:          return L"cloud";
			default :					 return L"rectangle";
			}
		}

		//--------------------------------------------------------------------------------
		// VmlPath 14.2.2.3 (v) (Part 4)
		//--------------------------------------------------------------------------------

		CVmlPath::CVmlPath() {}

		std::wstring CVmlPath::GetValue() const
		{
			return m_sValue;
		}

		void CVmlPath::SetValue(std::wstring &sValue)
		{
			m_sValue = sValue;
		}

		std::wstring CVmlPath::FromString(const std::wstring &sValue)
		{
			// TO DO: Сделать парсер пата Part4. 14.2.2.3
			m_sValue = sValue;

			return m_sValue;
		}

		std::wstring CVmlPath::ToString  () const
		{
			return m_sValue;
		}

		//--------------------------------------------------------------------------------
		// VmlDashStyle 14.2.2.1 (dashstyle) (Part 4)
		//--------------------------------------------------------------------------------

		EVmlDashStyle CVmlDashStyle::FromString(const std::wstring &sValueInput)
		{
			m_nCount    = 0;

			this->m_eValue = vmldashstyleSolid;

			if ( sValueInput.empty() )
				return this->m_eValue;

			std::wstring sValue = sValueInput;
			sValue = XmlUtils::GetLower(sValue);

			wchar_t wChar = sValue[0];
			switch ( wChar )
			{
			case 's':
				if      ( L"solid"           == sValue ) this->m_eValue = vmldashstyleSolid;
				else if ( L"shortdash"       == sValue ) this->m_eValue = vmldashstyleShortDash;
				else if ( L"shortdot"        == sValue ) this->m_eValue = vmldashstyleShortDot;
				else if ( L"shortdashdot"    == sValue ) this->m_eValue = vmldashstyleShortDashDot;
				else if ( L"shortdashdotdot" == sValue ) this->m_eValue = vmldashstyleShortDashDotDot;
				break;
			case 'd':
				if      ( L"dot"             == sValue ) this->m_eValue = vmldashstyleDot;
				else if ( L"dash"            == sValue ) this->m_eValue = vmldashstyleDash;
				else if ( L"dashdot"         == sValue ) this->m_eValue = vmldashstyleDashDot;
				break;
			case 'l':
				if      ( L"longdash"        == sValue ) this->m_eValue = vmldashstyleLongDash;
				else if ( L"longdashdot"     == sValue ) this->m_eValue = vmldashstyleLongDashDot;
				else if ( L"longdashdotdot"  == sValue ) this->m_eValue = vmldashstyleLongDashDotDot;
				break;

			default:
			{
				this->m_eValue = vmldashstyleCustom;

				std::wstring sTemp = sValue;
				for ( size_t nIndex = 0; nIndex < sValue.length(); nIndex++ )
				{
					int nChar = sValue[nIndex ];
					if ( ' ' == nChar )
						continue;

					int nEndPos = (int)sValue.find( L" ", nIndex );
					if ( -1 == nEndPos )
						nEndPos = (int)sValue.length();

					int nLen = (int)(nEndPos - nIndex);

					if ( nLen <= 0 )
						continue;

					std::wstring sTemp = sValue.substr( nIndex, nLen );
					double dVal = XmlUtils::GetDouble(sTemp);

					if ( m_nCount >= 32 )
						break;

					m_arrValues[m_nCount++] = dVal;
				}
			}
			}

			return this->m_eValue;
		}

		std::wstring CVmlDashStyle::ToString  () const
		{
			switch(this->m_eValue)
			{
			case vmldashstyleSolid          :  return L"solid";
			case vmldashstyleShortDash      :  return L"shortdash";
			case vmldashstyleShortDot       :  return L"shortdot";
			case vmldashstyleShortDashDot   :  return L"shortdashdot";
			case vmldashstyleShortDashDotDot:  return L"shortdashdotdot";
			case vmldashstyleDot            :  return L"dot";
			case vmldashstyleDash           :  return L"dash";
			case vmldashstyleLongDash       :  return L"longdash";
			case vmldashstyleDashDot        :  return L"dashdot";
			case vmldashstyleLongDashDot    :  return L"longdashdot";
			case vmldashstyleLongDashDotDot :  return L"longdashdotdot";
			case vmldashstyleCustom         :
			{
				std::wstring sResult;

				for (int nIndex = 0; nIndex < m_nCount; nIndex++)
				{
					sResult += XmlUtils::ToString( m_arrValues[nIndex] );
					sResult += L" ";
				}
				return sResult;
			}
			default: return L"solid";
			}
		}

		//--------------------------------------------------------------------------------
		// Vml_1_65536 14.2.2.11 (brightness)
		//--------------------------------------------------------------------------------

		CVml_1_65536::CVml_1_65536()
		{
			m_dValue = 0;
		}

		double CVml_1_65536::GetValue() const
		{
			return m_dValue;
		}

		void CVml_1_65536::SetValue(double dValue)
		{
			m_dValue = (std::max)( 0.0, (std::min)( 1.0, dValue) );
		}

		void CVml_1_65536::SetValue(int nValue)
		{
			m_dValue = (std::max)( 0.0, (std::min)( 65536.0, (double) nValue) ) / 65536.0;
		}

		double CVml_1_65536::FromString(const std::wstring &sValue)
		{
			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			bool bFraction = ( 'f' == sValue[ nLen - 1 ] );

			if ( bFraction )
			{
				std::wstring strValue = sValue.substr( 0, nLen - 1 );
				int nValue = strValue.empty() ? 0 : XmlUtils::GetInteger(strValue);

				SetValue( nValue );
			}
			else
			{
				double dValue = XmlUtils::GetDouble( sValue);
				SetValue( dValue );
			}

			return m_dValue;
		}

		std::wstring CVml_1_65536::ToString  () const
		{
			std::wstring sResult = boost::lexical_cast<std::wstring>( m_dValue );

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// Vml_Vector3D_65536 14.2.2.11 (lightposition)
		//--------------------------------------------------------------------------------

		CVml_Vector3D_65536::CVml_Vector3D_65536()
		{
			m_nX = 0;
			m_nY = 0;
			m_nZ = 0;
		}

		double CVml_Vector3D_65536::GetX() const
		{
			return (double)(m_nX / 65536.0);
		}
		double CVml_Vector3D_65536::GetY() const
		{
			return (double)(m_nY / 65536.0);
		}
		double CVml_Vector3D_65536::GetZ() const
		{
			return (double)(m_nZ / 65536.0);
		}

		void CVml_Vector3D_65536::SetValue(int nX, int nY, int nZ)
		{
			m_nX = nX;
			m_nY = nY;
			m_nZ = nZ;
		}

		void CVml_Vector3D_65536::SetValue(double dX, double dY, double dZ)
		{
			m_nX = (int)(dX * 65536);
			m_nY = (int)(dY * 65536);
			m_nZ = (int)(dZ * 65536);
		}

		double CVml_Vector3D_65536::FromString(const std::wstring &sValue)
		{
			m_nX = 0;
			m_nY = 0;
			m_nZ = 0;

			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			int nPos = (int)sValue.find( L"," );
			if ( -1 == nPos )
				return 0;

			std::wstring strX = sValue.substr( 0, nPos );
			XmlUtils::replace_all(strX, L"@", L"");

			m_nX = strX.empty() ? 0 : XmlUtils::GetInteger(strX);

			int nPos2 = (int)sValue.find( L",", nPos + 1 );
			if ( -1 == nPos2 )
				return 0;

			std::wstring strY = sValue.substr( nPos + 1, nPos2 - nPos - 1)  ;
			std::wstring strZ = sValue.substr( nPos2 + 1, nLen - nPos2 - 1 );

			XmlUtils::replace_all(strY, L"@", L"");
			XmlUtils::replace_all(strZ, L"@", L"");

			m_nY = strY.empty() ? 0 : XmlUtils::GetInteger(strY);
			m_nZ = strZ.empty() ? 0 : XmlUtils::GetInteger(strZ);

			return 0;
		}

		std::wstring CVml_Vector3D_65536::ToString  () const
		{
			return std::to_wstring(m_nX) + L"," + std::to_wstring(m_nX) + std::to_wstring(m_nY) + L"," + std::to_wstring(m_nZ);
		}

		//--------------------------------------------------------------------------------
		// Vml_Vector3D 14.2.2.11 (orientation)
		//--------------------------------------------------------------------------------

		CVml_Vector3D::CVml_Vector3D()
		{
			m_nX = 0;
			m_nY = 0;
			m_nZ = 0;
		}

		int CVml_Vector3D::GetX() const
		{
			return m_nX;
		}
		int CVml_Vector3D::GetY() const
		{
			return m_nY;
		}
		int CVml_Vector3D::GetZ() const
		{
			return m_nZ;
		}

		void CVml_Vector3D::SetValue(int nX, int nY, int nZ)
		{
			m_nX = nX;
			m_nY = nY;
			m_nZ = nZ;
		}

		double CVml_Vector3D::FromString(const std::wstring &sValue)
		{
			m_nX = 0;
			m_nY = 0;
			m_nZ = 0;

			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			int nPos = (int)sValue.find( L"," );
			if ( -1 == nPos )
			{//only x position
				std::wstring strX = sValue;
				XmlUtils::replace_all(strX, L"@", L"");

				m_nX = strX.empty() ? 0 : XmlUtils::GetInteger(strX);
				return 0;
			}
			std::wstring strX = sValue.substr( 0, nPos );
			XmlUtils::replace_all(strX, L"@", L"");

			m_nX = strX.empty() ? 0 : XmlUtils::GetInteger(strX);

			int nPos2 = (int)sValue.find( L",", nPos + 1 );
			if ( -1 == nPos2 )
			{// only x, y position
				std::wstring strY = sValue.substr( nPos + 1);
				XmlUtils::replace_all(strY, L"@", L"");
				m_nY = strY.empty() ? 0 : XmlUtils::GetInteger(strY);
				return 0;
			}

			std::wstring strY = sValue.substr( nPos + 1, nPos2 - nPos - 1);
			std::wstring strZ = sValue.substr( nPos2 + 1, nLen - nPos2 - 1 ) ;

			XmlUtils::replace_all(strY, L"@", L"");
			XmlUtils::replace_all(strZ, L"@", L"");

			m_nY = strY.empty() ? 0 : XmlUtils::GetInteger(strY);
			m_nZ = strZ.empty() ? 0 : XmlUtils::GetInteger(strZ);

			return 0;
		}

		std::wstring CVml_Vector3D::ToString  () const
		{
			return std::to_wstring(m_nX) + L"," + std::to_wstring(m_nY) + L"," + std::to_wstring(m_nZ);
		}

		//--------------------------------------------------------------------------------
		// Vml_Vector2D 14.2.2.11 (rotationangle)
		//--------------------------------------------------------------------------------

		CVml_Vector2D::CVml_Vector2D()
		{
			m_nX = 0;
			m_nY = 0;
		}

		int CVml_Vector2D::GetX() const
		{
			return m_nX;
		}
		int CVml_Vector2D::GetY() const
		{
			return m_nY;
		}

		void CVml_Vector2D::SetValue(int nX, int nY)
		{
			m_nX = nX;
			m_nY = nY;
		}

		double CVml_Vector2D::FromString(const std::wstring &sValue)
		{
			m_nX = 0;
			m_nY = 0;

			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			int nPos = (int)sValue.find( L"," );

			std::wstring strX, strY;
			if ( -1 == nPos )
			{//only x coord
				strX = sValue;
			}
			else
			{
				strX = sValue.substr( 0, nPos );
				strY = sValue.substr( nPos + 1, nLen - nPos - 1 ) ;
			}
			XmlUtils::replace_all(strY, L"@", L"");
			XmlUtils::replace_all(strX, L"@", L"");

			m_nX = strX.empty() ? 0 : XmlUtils::GetInteger(strX);
			m_nY = strY.empty() ? 0 : XmlUtils::GetInteger(strY);

			return 0;
		}

		std::wstring CVml_Vector2D::ToString  () const
		{
			return std::to_wstring(m_nX) + L"," + std::to_wstring(m_nY);
		}

		//--------------------------------------------------------------------------------
		// Vml_Vector2D_F 14.2.2.11 (viewpointorigin)
		//--------------------------------------------------------------------------------

		CVml_Vector2D_F::CVml_Vector2D_F()
		{
			m_dX = 0;
			m_dY = 0;
		}

		double CVml_Vector2D_F::GetX() const
		{
			return m_dX;
		}
		double CVml_Vector2D_F::GetY() const
		{
			return m_dY;
		}

		void CVml_Vector2D_F::SetValue(double dX, double dY)
		{
			m_dX = dX;
			m_dY = dY;
		}

		double CVml_Vector2D_F::FromString(const std::wstring &sValue)
		{
			m_dX = 0;
			m_dY = 0;

			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			size_t nPos = sValue.find( L"," );

			std::wstring strX, strY;
			if (std::wstring::npos == nPos)
			{
				strX = sValue;// .substr(0, nPos);
			}
			else
			{
				strX = sValue.substr( 0, nPos );
				strY = sValue.substr( nPos + 1, nLen - nPos - 1 ) ;
			}

			m_dX = XmlUtils::GetDouble(strX);
			m_dY = XmlUtils::GetDouble(strY);

			return 0;
		}

		std::wstring CVml_Vector2D_F::ToString  () const
		{
			return boost::lexical_cast<std::wstring>(m_dX) + L"," + boost::lexical_cast<std::wstring>(m_dY);
		}

		//--------------------------------------------------------------------------------
		// Vml_Polygon2D 14.1.2.1 (wrapcoords)
		//--------------------------------------------------------------------------------

		CVml_Polygon2D::CVml_Polygon2D()
		{
		}
		int CVml_Polygon2D::GetSize() const
		{
			return (int)m_arrPoints.size();
		}

		int CVml_Polygon2D::GetX(int nIndex) const
		{
			if ( nIndex < 0 || nIndex >= (int)m_arrPoints.size()  )
				return 0;

			return m_arrPoints[nIndex].nX;
		}
		int CVml_Polygon2D::GetY(int nIndex) const
		{
			if ( nIndex < 0 || nIndex >= (int)m_arrPoints.size()  )
				return 0;

			return m_arrPoints[nIndex].nY;
		}

		void CVml_Polygon2D::AddPoint(int nX, int nY)
		{
			TPoint oPt( nX, nY );
			m_arrPoints.push_back( oPt );
		}

		double CVml_Polygon2D::FromString(const std::wstring &sValue)
		{
			m_arrPoints.clear();

			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			int nStartPos = 0;
			while ( true )
			{
				int nMidPos = (int)sValue.find( L",", nStartPos );
				int nEndPos = (int)sValue.find( L",", nMidPos + 1 );

				if ( -1 == nMidPos )
					break;

				if ( -1 == nEndPos )
					nEndPos = nLen;

				std::wstring strX = sValue.substr( nStartPos, nMidPos - nStartPos );
				std::wstring strY = sValue.substr( nStartPos, nMidPos - nStartPos );

				XmlUtils::replace_all(strX, L"@", L"");
				XmlUtils::replace_all(strY, L"@", L"");

				int nX = strX.empty() ? 0 : XmlUtils::GetInteger(strX);
				int nY = strY.empty() ? 0 : XmlUtils::GetInteger(strY);

				m_arrPoints.push_back( TPoint( nX, nY ) );

				nStartPos = nEndPos + 1;
			}


			return 0;
		}

		std::wstring CVml_Polygon2D::ToString  () const
		{
			std::wstring sResult;

			for ( size_t nIndex = 0; nIndex < m_arrPoints.size(); nIndex++ )
			{
				std::wstring sTemp = std::to_wstring(m_arrPoints[nIndex].nX) + L"," + std::to_wstring(m_arrPoints[nIndex].nY);
				if ( nIndex < m_arrPoints.size() - 1 ) sTemp += L",";
				sResult += sTemp;
			}

			return sResult;
		}

		//------------------------------------------------------------------------------------------
		// CCssProperty
		//------------------------------------------------------------------------------------------

		std::wstring RemoveWhiteSpaces(const std::wstring &sText)
		{
			std::wstring result;
			for (size_t i = 0 ; i < sText.length(); i++)
			{
				WCHAR wChar = sText[i];
				if (wChar > 0x20)
				{
					result += wChar;
				}
			}
			return result;
		}

		CCssProperty::CCssProperty()
		{
			m_eType = Vml::cssptUnknown;
		}

		CCssProperty::CCssProperty(std::wstring sBuffer)
		{
			Parse(sBuffer);
		}
		CCssProperty::~CCssProperty()
		{
		}

		const UCssValue& CCssProperty::get_Value() const
		{
			return m_oValue;
		}

		const ECssPropertyType& CCssProperty::get_Type() const
		{
			return m_eType;
		}

		void CCssProperty::Parse(const std::wstring &sBuffer)
		{
			int nPos = (int)sBuffer.find( ':' );
			std::wstring sValue;

			if ( -1 == nPos )
			{
				m_eType = cssptUnknown;
			}
			else
			{
				std::wstring sProperty = sBuffer.substr( 0, nPos );
				sValue = sBuffer.substr( nPos + 1, sBuffer.length() - (nPos + 1) );

				sProperty = RemoveWhiteSpaces( sProperty );

				if ( sProperty.length() <= 2 )
				{
					m_eType = cssptUnknown;
					return;
				}

				// Чтобы избежать большого количества сравнения строк проверим для начала по первым двум символам
				int nChar1 = sProperty[ 0 ];
				int nChar2 = sProperty[ 1 ];

				switch( nChar1 )
				{
				case 'd':
				{
					if      ( L"direction" == sProperty )    m_eType = cssptDirection;
					else										m_eType = cssptUnknown;

					break;
				}
				case 'f':
				{
					switch( nChar2 )
					{
					case 'l':
					{
						if ( L"flip" == sProperty )  m_eType = cssptFlip;
						else                            m_eType = cssptUnknown;

						break;
					}
					case 'o':
					{
						if      ( L"font"         == sProperty ) m_eType = cssptFont;
						else if ( L"font-family"  == sProperty ) m_eType = cssptFontFamily;
						else if ( L"font-size"    == sProperty ) m_eType = cssptFontSize;
						else if ( L"font-style"   == sProperty ) m_eType = cssptFontStyle;
						else if ( L"font-variant" == sProperty ) m_eType = cssptFontVariant;
						else if ( L"font-weight"  == sProperty ) m_eType = cssptFontWeight;
						else										 m_eType = cssptUnknown;

						break;
					}
					default:
					{
						m_eType = cssptUnknown;
						break;
					}
					}
					break;
				}
				case 'h':
				{
					if      ( L"height" == sProperty )	m_eType = cssptHeight;
					else                                    m_eType = cssptUnknown;

					break;
				}
				case 'l':
				{
					if      ( L"layout-flow" == sProperty )		m_eType = cssptLayoutFlow;
					else if ( L"left"       == sProperty )		m_eType = cssptLeft;
					else											m_eType = cssptUnknown;

					break;
				}
				case 'm':
				{
					switch( nChar2 )
					{
					case 'a':
					{
						if      ( L"margin-bottom" == sProperty )	m_eType = cssptMarginBottom;
						else if ( L"margin-left"   == sProperty )	m_eType = cssptMarginLeft;
						else if ( L"margin-right"  == sProperty )	m_eType = cssptMarginRight;
						else if ( L"margin-top"    == sProperty )	m_eType = cssptMarginTop;
						else											m_eType = cssptUnknown;

						break;
					}
					case 's':
					{
						if      ( L"mso-direction-alt"					== sProperty ) m_eType = cssptMsoDirectionAlt;
						else if ( L"mso-fit-shape-to-text"				== sProperty ) m_eType = cssptMsoFitShapeToText;
						else if ( L"mso-fit-text-to-shape"				== sProperty ) m_eType = cssptMsoFitTextToShape;
						else if ( L"mso-layout-flow-alt"				== sProperty ) m_eType = cssptMsoLayoutFlowAlt;
						else if ( L"mso-next-textbox"					== sProperty ) m_eType = cssptMsoNextTextbox;
						else if ( L"mso-position-horizontal"			== sProperty ) m_eType = cssptMsoPositionHorizontal;
						else if ( L"mso-position-horizontal-relative"	== sProperty ) m_eType = cssptMsoPositionHorizontalRelative;
						else if ( L"mso-position-vertical"				== sProperty ) m_eType = cssptMsoPositionVertical;
						else if ( L"mso-position-vertical-relative"		== sProperty ) m_eType = cssptMsoPositionVerticalRelative;
						else if ( L"mso-rotate"							== sProperty ) m_eType = cssptMsoRotate;
						else if ( L"mso-text-scale"						== sProperty ) m_eType = cssptMsoTextScale;
						else if ( L"mso-text-shadow"					== sProperty ) m_eType = cssptMsoTextShadow;
						else if ( L"mso-wrap-distance-bottom"			== sProperty ) m_eType = cssptMsoWrapDistanceBottom;
						else if ( L"mso-wrap-distance-left"				== sProperty ) m_eType = cssptMsoWrapDistanceLeft;
						else if ( L"mso-wrap-distance-right"			== sProperty ) m_eType = cssptMsoWrapDistanceRight;
						else if ( L"mso-wrap-distance-top"				== sProperty ) m_eType = cssptMsoWrapDistanceTop;
						else if ( L"mso-wrap-edited"					== sProperty ) m_eType = cssptMsoWrapEdited;
						else if ( L"mso-wrap-style"						== sProperty ) m_eType = cssptMsoWrapStyle;
						else if ( L"mso-height-percent"					== sProperty ) m_eType = csspctMsoHeightPercent;
						else if ( L"mso-width-percent"					== sProperty ) m_eType = csspctMsoWidthPercent;
						else if ( L"mso-top-percent"					== sProperty ) m_eType = csspctMsoTopPercent;
						else if ( L"mso-left-percent"					== sProperty ) m_eType = csspctMsoLeftPercent;

						else															m_eType = cssptUnknown;

						break;
					}
					default:
					{
						m_eType = cssptUnknown;
						break;
					}
					}
					break;
				}
				case 'p':
				{
					if      ( L"position" == sProperty )		m_eType = cssptPosition;
					else										m_eType = cssptUnknown;

					break;
				}
				case 'r':
				{
					if      ( L"rotation" == sProperty )		m_eType = cssptRotation;
					else										m_eType = cssptUnknown;

					break;
				}
				case 't':
				{
					if      ( L"text-decoration" == sProperty )	m_eType = cssptTextDecoration;
					else if ( L"top"             == sProperty )	m_eType = cssptTop;
					else if ( L"text-align"		== sProperty )	m_eType = cssptHTextAlign;
					else										m_eType = cssptUnknown;

					break;
				}
				case 'v':
				{
					if      ( L"visibility"				== sProperty )	m_eType = cssptVisibility;
					else if ( L"v-rotate-letters"		== sProperty )	m_eType = cssptVRotateLetters;
					else if ( L"v-same-letter-heights"	== sProperty )	m_eType = cssptVSameLetterHeights;
					else if ( L"v-text-align"			== sProperty )	m_eType = cssptVTextAlign;
					else if ( L"v-text-anchor"			== sProperty )	m_eType = cssptVTextAnchor;
					else if ( L"v-text-kern"			== sProperty )	m_eType = cssptVTextKern;
					else if ( L"v-text-reverse"			== sProperty )	m_eType = cssptVTextReverse;
					else if ( L"v-text-spacing-mode"	== sProperty )	m_eType = cssptVTextSpacingMode;
					else if ( L"v-text-spacing"			== sProperty )	m_eType = cssptVTextSpacing;
					else											m_eType = cssptUnknown;

					break;
				}
				case 'w':
				{
					if ( L"width"  == sProperty )	m_eType = cssptWidth;
					else							m_eType = cssptUnknown;

					break;
				}
				case 'z':
				{
					if   ( L"z-index" == sProperty )		m_eType = cssptZIndex;
					else									m_eType = cssptUnknown;

					break;
				}
				default:
				{
					m_eType = cssptUnknown;
					break;
				}
				}
			}

			switch ( m_eType )
			{
			case cssptUnknown						: ReadValue_Unknown( sValue ); break;

			case cssptFlip							: ReadValue_Flip( sValue ); break;
			case cssptHeight						: ReadValue_Units( sValue ); break;
			case cssptLeft							: ReadValue_Units( sValue ); break;
			case cssptMarginBottom 					: ReadValue_Units( sValue ); break;
			case cssptMarginLeft					: ReadValue_Units( sValue ); break;
			case cssptMarginRight					: ReadValue_Units( sValue ); break;
			case cssptMarginTop						: ReadValue_Units( sValue ); break;
			case cssptMsoPositionHorizontal			: ReadValue_MsoPosHor( sValue ); break;
			case cssptMsoPositionHorizontalRelative	: ReadValue_MsoPosHorRel( sValue ); break;
			case cssptMsoPositionVertical			: ReadValue_MsoPosVer( sValue ); break;
			case cssptMsoPositionVerticalRelative	: ReadValue_MsoPosVerRel( sValue ); break;
			case cssptMsoWrapDistanceBottom			: ReadValue_Units( sValue ); break;
			case cssptMsoWrapDistanceLeft			: ReadValue_Units( sValue ); break;
			case cssptMsoWrapDistanceRight			: ReadValue_Units( sValue ); break;
			case cssptMsoWrapDistanceTop			: ReadValue_Units( sValue ); break;
			case cssptMsoWrapEdited					: ReadValue_Boolean( sValue ); break;
			case cssptMsoWrapStyle					: ReadValue_MsoWrapStyle( sValue ); break;
			case cssptPosition						: ReadValue_Position( sValue ); break;
			case cssptRotation						: ReadValue_Rotation( sValue ); break;
			case cssptTop							: ReadValue_Units( sValue ); break;
			case cssptVisibility					: ReadValue_Visibility( sValue ); break;
			case cssptWidth							: ReadValue_Units( sValue ); break;
			case cssptZIndex						: ReadValue_ZIndex( sValue ); break;

			case cssptDirection						: ReadValue_Direction( sValue ); break;
			case cssptLayoutFlow					: ReadValue_LayoutFlow( sValue ); break;
			case cssptMsoDirectionAlt				: ReadValue_DirectionAlt( sValue ); break;
			case cssptMsoFitShapeToText				: ReadValue_Boolean( sValue ); break;
			case cssptMsoFitTextToShape				: ReadValue_Boolean( sValue ); break;
			case cssptMsoLayoutFlowAlt				: ReadValue_LayoutFlowAlt( sValue ); break;
			case cssptMsoNextTextbox				: ReadValue_String( sValue ); break;
			case cssptMsoRotate						: ReadValue_MsoRotate( sValue ); break;
			case cssptMsoTextScale					: ReadValue_Units( sValue ); break;
			case cssptVTextAnchor					: ReadValue_VTextAnchor( sValue ); break;

			case cssptFont							: ReadValue_String( sValue ); break;
			case cssptFontFamily					: ReadValue_String( sValue ); break;
			case cssptFontSize						: ReadValue_Units( sValue ); break;
			case cssptFontStyle						: ReadValue_FontStyle( sValue ); break;
			case cssptFontVariant					: ReadValue_FontVariant( sValue ); break;
			case cssptFontWeight					: ReadValue_FontWeight( sValue ); break;
			case cssptMsoTextShadow					: ReadValue_Boolean( sValue ); break;
			case cssptTextDecoration				: ReadValue_TextDecoration( sValue ); break;
			case cssptVRotateLetters				: ReadValue_Boolean( sValue ); break;
			case cssptVSameLetterHeights			: ReadValue_Boolean( sValue ); break;
			case cssptVTextAlign					: ReadValue_VTextAlign( sValue ); break;
			case cssptVTextKern						: ReadValue_Boolean( sValue ); break;
			case cssptVTextReverse					: ReadValue_Boolean( sValue ); break;
			case cssptVTextSpacingMode				: ReadValue_VTextSpacingMode( sValue ); break;
			case cssptVTextSpacing					: ReadValue_Units( sValue ); break;
			case csspctMsoWidthPercent				: ReadValue_Units( sValue ); break;
			case csspctMsoHeightPercent				: ReadValue_Units( sValue ); break;
			case csspctMsoLeftPercent				: ReadValue_Units( sValue); break;
			case csspctMsoTopPercent				: ReadValue_Units( sValue); break;
			case cssptHTextAlign					: ReadValue_VTextAlign( sValue ); break;
			};
		}

		void CCssProperty::ReadValue_Unknown(std::wstring& sValue)
		{
			// Ничего не делаем
		}
		void CCssProperty::ReadValue_Flip(std::wstring& sValue)
		{
			if      ( L"x"  == sValue ) m_oValue.eFlip = cssflipX;
			else if ( L"y"  == sValue ) m_oValue.eFlip = cssflipY;
			else if ( L"xy" == sValue ) m_oValue.eFlip = cssflipXY;
			else if ( L"yx" == sValue ) m_oValue.eFlip = cssflipYX;
			else
				m_eType = cssptUnknown;
		}
		void CCssProperty::ReadValue_Units(std::wstring& sValue)
		{
			int nPos = -1;
			if ( -1 != ( nPos = (int)sValue.find(L"auto") ) )
			{
				m_oValue.oValue.eType = cssunitstypeAuto;
			}
			else if ( -1 != ( nPos = (int)sValue.find( L"in" ) ) )
			{
				m_oValue.oValue.eType = cssunitstypeUnits;

				std::wstring strValue = sValue.substr( 0, nPos );
				double dValue = XmlUtils::GetDouble(sValue);

				m_oValue.oValue.dValue = Inch_To_Pt(dValue);
			}
			else if ( -1 != ( nPos = (int)sValue.find( L"cm" ) ) )
			{
				m_oValue.oValue.eType = cssunitstypeUnits;

				std::wstring strValue = sValue.substr( 0, nPos );
				double dValue = XmlUtils::GetDouble(sValue);

				m_oValue.oValue.dValue = Cm_To_Pt(dValue);
			}
			else if ( -1 != ( nPos = (int)sValue.find( L"mm" ) ) )
			{
				m_oValue.oValue.eType = cssunitstypeUnits;

				std::wstring strValue = sValue.substr( 0, nPos );
				double dValue = XmlUtils::GetDouble(sValue);

				m_oValue.oValue.dValue = Mm_To_Pt(dValue);
			}
			else if ( -1 != ( nPos = (int)sValue.find( L"em" ) ) )
			{
				// TO DO: Реализовать единицы 'em'
			}
			else if ( -1 != ( nPos = (int)sValue.find( L"ex" ) ) )
			{
				// TO DO: Реализовать единицы 'ex'
			}
			else if ( -1 != ( nPos = (int)sValue.find( L"pt" ) ) )
			{
				m_oValue.oValue.eType = cssunitstypeUnits;

				std::wstring strValue = sValue.substr( 0, nPos );
				double dValue = XmlUtils::GetDouble(sValue);

				m_oValue.oValue.dValue = dValue;
			}
			else if ( -1 != ( nPos = (int)sValue.find( L"pc" ) ) )
			{
				m_oValue.oValue.eType = cssunitstypeUnits;

				std::wstring strValue = sValue.substr( 0, nPos );
				double dValue = XmlUtils::GetDouble(sValue);

				m_oValue.oValue.dValue = dValue * 12;
			}
			else if ( -1 != ( nPos = (int)sValue.find( L"%" ) ) )
			{
				m_oValue.oValue.eType = cssunitstypePerc;

				std::wstring strValue = sValue.substr( 0, nPos );
				m_oValue.oValue.dValue = XmlUtils::GetDouble(strValue);
			}
			else if ( -1 != ( nPos = (int)sValue.find( L"px" ) ) )
			{
				m_oValue.oValue.eType = cssunitstypeUnits;

				std::wstring strValue = sValue.substr( 0, nPos );
				double dValue = XmlUtils::GetDouble(sValue);

				m_oValue.oValue.dValue = Px_To_Pt(dValue);
			}
			else
			{
				m_oValue.oValue.eType = cssunitstypeAbsolute;
				try
				{
					m_oValue.oValue.dValue = XmlUtils::GetDouble(sValue);
				}
				catch(...)
				{
					m_oValue.oValue.dValue = 0;
				}
			}
		}

		void CCssProperty::ReadValue_MsoPosHor(std::wstring& sValue)
		{
			if      ( L"absolute" == sValue ) m_oValue.eMsoPosHor = cssmsoposhorAbsolute;
			else if ( L"left"     == sValue ) m_oValue.eMsoPosHor = cssmsoposhorLeft;
			else if ( L"center"   == sValue ) m_oValue.eMsoPosHor = cssmsoposhorCenter;
			else if ( L"right"    == sValue ) m_oValue.eMsoPosHor = cssmsoposhorRight;
			else if ( L"inside"   == sValue ) m_oValue.eMsoPosHor = cssmsoposhorInside;
			else if ( L"outside"  == sValue ) m_oValue.eMsoPosHor = cssmsoposhorOutside;
			else
				m_oValue.eMsoPosHor = cssmsoposhorAbsolute;
		}
		void CCssProperty::ReadValue_MsoPosHorRel(std::wstring& sValue)
		{
			if      ( L"left-margin-area"	== sValue )	m_oValue.eMsoPosHorRel = cssmsoposhorrelLeftMargin;
			else if ( L"right-margin-area"	== sValue )	m_oValue.eMsoPosHorRel = cssmsoposhorrelRightMargin;
			else if ( L"margin"				== sValue ) m_oValue.eMsoPosHorRel = cssmsoposhorrelMargin;
			else if ( L"page"				== sValue ) m_oValue.eMsoPosHorRel = cssmsoposhorrelPage;
			else if ( L"text"				== sValue ) m_oValue.eMsoPosHorRel = cssmsoposhorrelText;
			else if ( L"char"				== sValue ) m_oValue.eMsoPosHorRel = cssmsoposhorrelChar;
			else
				m_oValue.eMsoPosHorRel = cssmsoposhorrelText;
		}
		void CCssProperty::ReadValue_MsoPosVer(std::wstring& sValue)
		{
			if      ( L"absolute" == sValue ) m_oValue.eMsoPosVer = cssmsoposverAbsolute;
			else if ( L"top"      == sValue ) m_oValue.eMsoPosVer = cssmsoposverTop;
			else if ( L"center"   == sValue ) m_oValue.eMsoPosVer = cssmsoposverCenter;
			else if ( L"bottom"   == sValue ) m_oValue.eMsoPosVer = cssmsoposverBottom;
			else if ( L"inside"   == sValue ) m_oValue.eMsoPosVer = cssmsoposverInside;
			else if ( L"outside"  == sValue ) m_oValue.eMsoPosVer = cssmsoposverOutside;
			else
				m_oValue.eMsoPosVer = cssmsoposverAbsolute;
		}
		void CCssProperty::ReadValue_MsoPosVerRel(std::wstring& sValue)
		{
			if      ( L"bottom-margin-area" == sValue )	m_oValue.eMsoPosVerRel = cssmsoposverrelBottomMargin;
			else if ( L"top-margin-area" == sValue )	m_oValue.eMsoPosVerRel = cssmsoposverrelTopMargin;
			else if ( L"margin" == sValue )				m_oValue.eMsoPosVerRel = cssmsoposverrelMargin;
			else if ( L"page"   == sValue )				m_oValue.eMsoPosVerRel = cssmsoposverrelPage;
			else if ( L"text"   == sValue )				m_oValue.eMsoPosVerRel = cssmsoposverrelText;
			else if ( L"line"   == sValue )				m_oValue.eMsoPosVerRel = cssmsoposverrelLine;
			else
				m_oValue.eMsoPosVerRel = cssmsoposverrelText;
		}

		void CCssProperty::ReadValue_Rotation(std::wstring& sValue)
		{
			m_oValue.oValue.eType = cssunitstypeAbsolute;
			m_oValue.oValue.dValue = XmlUtils::GetDouble( sValue );

			if (sValue.find(L"fd") != std::wstring::npos)
			{
				m_oValue.oValue.dValue /= 6000.;
			}
			else if (sValue.find(L"f") == sValue.length() - 1)
			{
				m_oValue.oValue.dValue /= 65536.;
			}
		}
		void CCssProperty::ReadValue_Boolean(std::wstring& sValue)
		{
			if ( L"true" == sValue || L"t" == sValue || L"1" == sValue )
				m_oValue.bValue = true;
			else
				m_oValue.bValue = false;
		}
		void CCssProperty::ReadValue_MsoWrapStyle(std::wstring& sValue)
		{
			if      ( L"square" == sValue ) m_oValue.eMsoWrapStyle = cssmsowrapstyleSqaure;
			else if ( L"none"   == sValue ) m_oValue.eMsoWrapStyle = cssmsowrapstyleNone;
			else
				m_oValue.eMsoWrapStyle = cssmsowrapstyleSqaure;
		}
		void CCssProperty::ReadValue_Position(std::wstring& sValue)
		{
			if      ( L"static"   == sValue ) m_oValue.ePosition = csspositionStatic;
			else if ( L"absolute" == sValue ) m_oValue.ePosition = csspositionAbsolute;
			else if ( L"relative" == sValue ) m_oValue.ePosition = csspositionRelative;
			else
				m_oValue.ePosition = csspositionAbsolute;
		}
		void CCssProperty::ReadValue_Visibility(std::wstring& sValue)
		{
			if      ( L"hidden"  == sValue ) m_oValue.eVisibility = cssvisibilityHidden;
			else if ( L"inherit" == sValue ) m_oValue.eVisibility = cssvisibilityInherit;
			else
				m_oValue.eVisibility = cssvisibilityInherit;
		}
		void CCssProperty::ReadValue_ZIndex(std::wstring& sValue)
		{
			if      ( L"auto"  == sValue ) m_oValue.oZIndex.eType = csszindextypeAuto;
			else
			{
				m_oValue.oZIndex.eType  = csszindextypeOrder;
				m_oValue.oZIndex.nOrder = XmlUtils::GetInteger(sValue);

			}
		}
		void CCssProperty::ReadValue_Direction(std::wstring& sValue)
		{
			if      ( L"ltr" == sValue ) m_oValue.eDirection = cssdirectionLTR;
			else if ( L"rtl" == sValue ) m_oValue.eDirection = cssdirectionRTL;
			else
				m_oValue.eDirection = cssdirectionLTR;
		}
		void CCssProperty::ReadValue_LayoutFlow(std::wstring& sValue)
		{
			if      ( L"horizontal"             == sValue ) m_oValue.eLayoutFlow = csslayoutflowHorizontal;
			else if ( L"vertical"				== sValue ) m_oValue.eLayoutFlow = csslayoutflowVertical;
			else if ( L"vertical-ideographic"   == sValue ) m_oValue.eLayoutFlow = csslayoutflowVerticalIdeographic;
			else if ( L"horizontal-ideographic" == sValue ) m_oValue.eLayoutFlow = csslayoutflowHorizontalIdeographic;
			else
				m_oValue.eLayoutFlow = csslayoutflowHorizontal;
		}
		void CCssProperty::ReadValue_DirectionAlt(std::wstring& sValue)
		{
			m_oValue.eDirectionAlt = cssdirectionaltContext;
		}
		void CCssProperty::ReadValue_LayoutFlowAlt(std::wstring& sValue)
		{
			m_oValue.eLayoutFlowAlt = csslayoutflowaltBottomToTop;
		}
		void CCssProperty::ReadValue_String(std::wstring& sValue)
		{
			int nLen = (int)sValue.length();
			if ( nLen > 127 )
				return;

			::memcpy( m_oValue.wsValue, sValue.c_str(), nLen * sizeof( wchar_t ) );
			m_oValue.wsValue[nLen] = '\0';
		}

		void CCssProperty::ReadValue_MsoRotate(std::wstring& sValue)
		{
			if      ( L"0"   == sValue ) m_oValue.eRotate = cssmsorotate0;
			else if ( L"90"  == sValue ) m_oValue.eRotate = cssmsorotate90;
			else if ( L"180" == sValue ) m_oValue.eRotate = cssmsorotate180;
			else if ( L"-90" == sValue ) m_oValue.eRotate = cssmsorotate270;
			else
				m_oValue.eRotate = cssmsorotate0;
		}
		void CCssProperty::ReadValue_VTextAnchor(std::wstring& sValue)
		{
			if      ( L"top"					== sValue ) m_oValue.eVTextAnchor = cssvtextanchorTop;
			else if ( L"middle"					== sValue ) m_oValue.eVTextAnchor = cssvtextanchorMiddle;
			else if ( L"bottom"					== sValue ) m_oValue.eVTextAnchor = cssvtextanchorBottom;
			else if ( L"top-center"             == sValue ) m_oValue.eVTextAnchor = cssvtextanchorTopCenter;
			else if ( L"middle-center"          == sValue ) m_oValue.eVTextAnchor = cssvtextanchorMiddleCenter;
			else if ( L"bottom-center"          == sValue ) m_oValue.eVTextAnchor = cssvtextanchorBottomCenter;
			else if ( L"top-baseline"           == sValue ) m_oValue.eVTextAnchor = cssvtextanchorTopBaseline;
			else if ( L"bottom-baseline"        == sValue ) m_oValue.eVTextAnchor = cssvtextanchorBottomBaseline;
			else if ( L"top-center-baseline"    == sValue ) m_oValue.eVTextAnchor = cssvtextanchorTopCenterBaseline;
			else if ( L"bottom-center-baseline" == sValue ) m_oValue.eVTextAnchor = cssvtextanchorBottomCenterBaseline;
			else
				m_oValue.eVTextAnchor = cssvtextanchorTop;
		}
		void CCssProperty::ReadValue_FontStyle(std::wstring& sValue)
		{
			if      ( L"normal"  == sValue ) m_oValue.eFontStyle = cssfontstyleNormal;
			else if ( L"italic"  == sValue ) m_oValue.eFontStyle = cssfontstyleItalic;
			else if ( L"oblique" == sValue ) m_oValue.eFontStyle = cssfontstyleOblique;
			else
				m_oValue.eFontStyle = cssfontstyleNormal;
		}
		void CCssProperty::ReadValue_FontVariant(std::wstring& sValue)
		{
			if      ( L"normal"     == sValue ) m_oValue.eFontVariant = cssfontvariantNormal;
			else if ( L"small-caps" == sValue ) m_oValue.eFontVariant = cssfontvariantSmallCaps;
			else
				m_oValue.eFontVariant = cssfontvariantNormal;
		}
		void CCssProperty::ReadValue_FontWeight(std::wstring& sValue)
		{
			if      ( L"normal"  == sValue ) m_oValue.eFontWeight = cssfontweightNormal;
			else if ( L"lighter" == sValue ) m_oValue.eFontWeight = cssfontweightLighter;
			else if ( L"100"     == sValue ) m_oValue.eFontWeight = cssfontweight100;
			else if ( L"200"     == sValue ) m_oValue.eFontWeight = cssfontweight200;
			else if ( L"300"     == sValue ) m_oValue.eFontWeight = cssfontweight300;
			else if ( L"400"     == sValue ) m_oValue.eFontWeight = cssfontweight400;
			else if ( L"bold"    == sValue ) m_oValue.eFontWeight = cssfontweightBold;
			else if ( L"bolder"  == sValue ) m_oValue.eFontWeight = cssfontweightBolder;
			else if ( L"500"     == sValue ) m_oValue.eFontWeight = cssfontweight500;
			else if ( L"600"     == sValue ) m_oValue.eFontWeight = cssfontweight600;
			else if ( L"700"     == sValue ) m_oValue.eFontWeight = cssfontweight700;
			else if ( L"800"     == sValue ) m_oValue.eFontWeight = cssfontweight800;
			else if ( L"900"     == sValue ) m_oValue.eFontWeight = cssfontweight900;
			else
				m_oValue.eFontWeight = cssfontweightNormal;
		}
		void CCssProperty::ReadValue_TextDecoration(std::wstring& sValue)
		{
			if      ( L"none"         == sValue ) m_oValue.eTextDecoration = csstextdecorationNone;
			else if ( L"underline"    == sValue ) m_oValue.eTextDecoration = csstextdecorationUnderline;
			else if ( L"overline"     == sValue ) m_oValue.eTextDecoration = csstextdecorationOverline;
			else if ( L"line-through" == sValue ) m_oValue.eTextDecoration = csstextdecorationLineThrough;
			else if ( L"blink"        == sValue ) m_oValue.eTextDecoration = csstextdecorationBlink;
			else
				m_oValue.eTextDecoration = csstextdecorationNone;
		}
		void CCssProperty::ReadValue_VTextAlign(std::wstring& sValue)
		{
			if      ( L"left"            == sValue ) m_oValue.eVTextAlign = cssvtextalignLeft;
			else if ( L"right"           == sValue ) m_oValue.eVTextAlign = cssvtextalignRight;
			else if ( L"center"          == sValue ) m_oValue.eVTextAlign = cssvtextalignCenter;
			else if ( L"justify"         == sValue ) m_oValue.eVTextAlign = cssvtextalignJustify;
			else if ( L"letter-justify"  == sValue ) m_oValue.eVTextAlign = cssvtextalignLetterJustify;
			else if ( L"stretch-justify" == sValue ) m_oValue.eVTextAlign = cssvtextalignStretchJustify;
			else
				m_oValue.eVTextAlign = cssvtextalignLeft;
		}
		void CCssProperty::ReadValue_VTextSpacingMode(std::wstring& sValue)
		{
			if      ( L"tightening" == sValue ) m_oValue.eVTextSpacingMode = cssvtextspacingmodeTightening;
			else if ( L"tracking"   == sValue ) m_oValue.eVTextSpacingMode = cssvtextspacingmodeTracking;
			else
				m_oValue.eVTextSpacingMode = cssvtextspacingmodeTightening;
		}

		//--------------------------------------------------------------------------------
		// CCssStyle 14.1.2.1 (style) (Part 1)
		//--------------------------------------------------------------------------------

		CCssStyle::CCssStyle()
		{
		}
		CCssStyle::~CCssStyle()
		{
			Clear();
		}
		void CCssStyle::Clear()
		{
			m_arrProperties.clear();
		}

		std::wstring CCssStyle::FromString(const std::wstring &sValue)
		{
			Clear();

			m_sCss = sValue;
			ParseProperties();

			return m_sCss;
		}

		std::wstring CCssStyle::ToString  () const
		{
			return m_sCss;
		}

		void CCssStyle::mergeFrom(CCssStyle* parent)
		{
			if (!parent) return;

			for (std::map<ECssPropertyType, size_t>::iterator it = parent->m_mapProperties.begin(); it != parent->m_mapProperties.end(); ++it)
			{
				std::map<ECssPropertyType, size_t>::iterator pFind = m_mapProperties.find(it->first);
				if (pFind == m_mapProperties.end())
				{
					m_arrProperties.push_back(parent->m_arrProperties[it->second]);
					m_mapProperties.insert(std::make_pair(it->first, m_arrProperties.size() - 1));
				}
			}
		}

		bool CCssStyle::ParseProperties()
		{
			std::wstring sTemp = m_sCss;
			while ( false == sTemp.empty() )
			{
				size_t nPos = (int)sTemp.find( ';' );
				if ( std::wstring::npos == nPos )
				{
					CCssPropertyPtr pProperty = CCssPropertyPtr(new CCssProperty(sTemp));
					if ((pProperty) && (cssptUnknown != pProperty->get_Type()) )
					{
						std::map<ECssPropertyType, size_t>::iterator pFind = m_mapProperties.find(pProperty->get_Type());
						if (pFind != m_mapProperties.end())
						{
							m_arrProperties[pFind->second] = pProperty;
						}
						else
						{
							m_arrProperties.push_back(pProperty);
							m_mapProperties.insert(std::make_pair(pProperty->get_Type(), m_arrProperties.size() - 1));
						}
					}

					sTemp.clear();
					continue;
				}
				else
				{
					CCssPropertyPtr pProperty = CCssPropertyPtr(new  CCssProperty( sTemp.substr( 0, nPos )));

					if ((pProperty) &&  (cssptUnknown != pProperty->get_Type()) )
					{
						std::map<ECssPropertyType, size_t>::iterator pFind = m_mapProperties.find(pProperty->get_Type());
						if (pFind != m_mapProperties.end())
						{
							m_arrProperties[pFind->second] = pProperty;
						}
						else
						{
							m_arrProperties.push_back(pProperty);
							m_mapProperties.insert(std::make_pair(pProperty->get_Type(), m_arrProperties.size() - 1));
						}
					}

					sTemp = sTemp.substr( nPos + 1, sTemp.length() - nPos - 1 );
				}
			}

			return true;
		}

		//--------------------------------------------------------------------------------
		// Vml_Vector2D_Units 14.1.2.3 (from, control1, control2, to)
		//--------------------------------------------------------------------------------

		CVml_Vector2D_Units::CVml_Vector2D_Units()
		{
			m_dX = 0;
			m_dY = 0;
		}

		double CVml_Vector2D_Units::GetX() const
		{
			return m_dX;
		}
		double CVml_Vector2D_Units::GetY() const
		{
			return m_dY;
		}

		void CVml_Vector2D_Units::SetValue(double dX, double dY)
		{
			m_dX = dX;
			m_dY = dY;
		}

		double CVml_Vector2D_Units::FromString(const std::wstring &sValue)
		{
			m_dX = 0;
			m_dY = 0;

			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			int nPos = (int)sValue.find( L"," );
			if ( -1 == nPos )
			{//only x position
				SimpleTypes::CPoint oPt1 = sValue;
				m_dX = oPt1.GetValue();
				return 0;
			}

			SimpleTypes::CPoint oPt1 = sValue.substr( 0, nPos );
			m_dX = oPt1.GetValue();

			SimpleTypes::CPoint oPt2 = sValue.substr( nPos + 1, nLen - nPos - 1 );
			m_dY = oPt2.GetValue();

			return 0;
		}

		std::wstring CVml_Vector2D_Units::ToString  () const
		{
			return boost::lexical_cast<std::wstring>(m_dX) + L"," + boost::lexical_cast<std::wstring>(m_dY);
		}

		//--------------------------------------------------------------------------------
		// Vml_Vector2D_Percentage
		//--------------------------------------------------------------------------------

		CVml_Vector2D_Percentage::CVml_Vector2D_Percentage()
		{
			m_dX = 0;
			m_dY = 0;
		}

		double CVml_Vector2D_Percentage::GetX() const
		{
			return m_dX;
		}
		double CVml_Vector2D_Percentage::GetY() const
		{
			return m_dY;
		}

		void CVml_Vector2D_Percentage::SetValue(double dX, double dY)
		{
			m_dX = dX;
			m_dY = dY;
		}

		double CVml_Vector2D_Percentage::FromString(const std::wstring &sValue)
		{
			m_dX = 0;
			m_dY = 0;

			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			int nPos = (int)sValue.find( L"," );
			if ( -1 == nPos )
			{//only x position
				SimpleTypes::CPercentage oPerc1 = sValue;
				m_dX = oPerc1.GetValue();
				return 0;
			}

			SimpleTypes::CPercentage oPerc1 = sValue.substr( 0, nPos );
			m_dX = oPerc1.GetValue();

			SimpleTypes::CPercentage oPerc2 = sValue.substr( nPos + 1, nLen - nPos - 1 );
			m_dY = oPerc2.GetValue();

			return 0;
		}

		std::wstring CVml_Vector2D_Percentage::ToString  () const
		{
			return boost::lexical_cast<std::wstring>(m_dX) + L"%," + boost::lexical_cast<std::wstring>(m_dY) + L"%";
		}

		//--------------------------------------------------------------------------------
		// Vml_Vector2D_1_65536
		//--------------------------------------------------------------------------------

		CVml_Vector2D_1_65536::CVml_Vector2D_1_65536()
		{
			m_dX = 0;
			m_dY = 0;
		}

		double CVml_Vector2D_1_65536::GetX() const
		{
			return m_dX;
		}
		double CVml_Vector2D_1_65536::GetY() const
		{
			return m_dY;
		}

		void CVml_Vector2D_1_65536::SetValue(double dX, double dY)
		{
			m_dX = dX;
			m_dY = dY;
		}

		double CVml_Vector2D_1_65536::FromString(const std::wstring &sValue)
		{
			m_dX = 0;
			m_dY = 0;

			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			int nPos = (int)sValue.find( L"," );
			if ( -1 == nPos )
			{//only x position
				SimpleTypes::Vml::CVml_1_65536 oFraction1 = sValue;
				m_dX = oFraction1.GetValue();
				return 0;
			}

			SimpleTypes::Vml::CVml_1_65536 oFraction1 = sValue.substr( 0, nPos );
			m_dX = oFraction1.GetValue();

			SimpleTypes::Vml::CVml_1_65536 oFraction2 = sValue.substr( nPos + 1, nLen - nPos - 1 );
			m_dY = oFraction2.GetValue();

			return 0;
		}

		std::wstring CVml_Vector2D_1_65536::ToString  () const
		{
			return boost::lexical_cast<std::wstring>(m_dX) + L"," + boost::lexical_cast<std::wstring>(m_dY);
		}

		//--------------------------------------------------------------------------------
		// CVml_TableLimits
		//--------------------------------------------------------------------------------

		CVml_TableLimits::CVml_TableLimits()
		{
		}

		unsigned int CVml_TableLimits::GetSize() const
		{
			return (unsigned int)m_arrLimits.size();
		}
		double CVml_TableLimits::GetAt(int nIndex) const
		{
			if ( nIndex < 0 || nIndex >= (int)m_arrLimits.size() )
				return 0;

			return m_arrLimits[nIndex];
		}

		void CVml_TableLimits::AddValue(double dValue)
		{
			m_arrLimits.push_back( dValue );
		}

		int CVml_TableLimits::FromString(const std::wstring &sValue)
		{
			int nPos = 0;
			int nLen = (int)sValue.length();

			int nSpacePos = 0;
			wchar_t wChar;
			while ( nPos < nLen )
			{
				while ( ' ' == ( wChar = sValue[nPos] ) )
				{
					nPos++;
					if ( nPos >= nLen )
						return 0;
				}

				nSpacePos = (int)sValue.find( L" ", nPos );
				if ( -1 == nSpacePos )
					nSpacePos = nLen;

				SimpleTypes::CPoint oPoint = sValue.substr( nPos, nSpacePos - nPos );
				nPos = nSpacePos + 1;

				m_arrLimits.push_back( oPoint.ToPoints() );
			}

			return 0;
		}

		std::wstring CVml_TableLimits::ToString  () const
		{
			std::wstring sResult;

			for ( unsigned int nIndex = 0; nIndex < m_arrLimits.size(); nIndex++ )
			{
				sResult += boost::lexical_cast<std::wstring>(m_arrLimits[nIndex]) + L"pt ";
			}

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// CVml_TableProperties
		//--------------------------------------------------------------------------------

		int CVml_TableProperties::FromString(const std::wstring &sValue)
		{
			this->m_eValue = XmlUtils::GetInteger(sValue);

			return this->m_eValue;
		}

		std::wstring CVml_TableProperties::ToString  () const
		{
			std::wstring sResult = std::to_wstring( this->m_eValue);

			return sResult;
		}

		bool CVml_TableProperties::IsTable() const
		{
			return ( this->m_eValue & 1 ? true : false );
		}

		bool CVml_TableProperties::IsPlaceholder() const
		{
			return ( this->m_eValue & 2 ? true : false );
		}

		bool CVml_TableProperties::IsBiDirectionalText() const
		{
			return ( this->m_eValue & 4 ? true : false );
		}

		//--------------------------------------------------------------------------------
		// Vml_Vector2D_Position 14.1.2.8 (position)
		//--------------------------------------------------------------------------------

		CVml_Vector2D_Position::CVml_Vector2D_Position()
		{

		}

		EVml_Vector2D_Position CVml_Vector2D_Position::GetTypeX() const
		{
			return m_eTypeX;
		}
		EVml_Vector2D_Position CVml_Vector2D_Position::GetTypeY() const
		{
			return m_eTypeY;
		}
		std::wstring CVml_Vector2D_Position::GetIdX() const
		{
			return m_sIdX;
		}
		std::wstring CVml_Vector2D_Position::GetIdY() const
		{
			return m_sIdY;
		}
		double					CVml_Vector2D_Position::GetX() const
		{
			return m_dX;
		}
		double					CVml_Vector2D_Position::GetY() const
		{
			return m_dY;
		}
		void   CVml_Vector2D_Position::SetConstantX(double dX)
		{
			m_eTypeX = vmlvector2dposConstant;
			m_dX     = dX;
		}

		void   CVml_Vector2D_Position::SetConstantY(double dY)
		{
			m_eTypeY = vmlvector2dposConstant;
			m_dY     = dY;
		}

		double CVml_Vector2D_Position::FromString(const std::wstring &sValue)
		{
			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			int nPos = (int)sValue.find( L"," );
			if ( -1 == nPos )
				return 0;

			std::wstring sFirst  = sValue.substr( 0, nPos );
			std::wstring sSecond = sValue.substr( nPos + 1, nLen - nPos - 1 );

			Parse( sFirst, true );
			Parse( sSecond, false );

			return 0;
		}

		std::wstring CVml_Vector2D_Position::ToString  () const
		{
			std::wstring sResult;

			switch ( m_eTypeX )
			{
			case vmlvector2dposConstant    : sResult = boost::lexical_cast<std::wstring>( m_dX ); break;
			case vmlvector2dposFormula     : sResult = L"@" + m_sIdX; break;
			case vmlvector2dposAdjValue    : sResult = L"#" + m_sIdX; break;
			case vmlvector2dposCenter      : sResult = L"center"; break;
			case vmlvector2dposTopLeft     : sResult = L"topleft"; break;
			case vmlvector2dposBottomRight : sResult = L"bottomright"; break;
			};

			sResult += L",";

			switch ( m_eTypeY )
			{
			case vmlvector2dposConstant    :
			{
				std::wstring sTemp = boost::lexical_cast<std::wstring>( m_dY );
				sResult += sTemp;
				break;
			}
			case vmlvector2dposFormula     : sResult += L"@" + m_sIdY; break;
			case vmlvector2dposAdjValue    : sResult += L"#" + m_sIdY; break;
			case vmlvector2dposCenter      : sResult += L"center"; break;
			case vmlvector2dposTopLeft     : sResult += L"topleft"; break;
			case vmlvector2dposBottomRight : sResult += L"bottomright"; break;
			};

			return sResult;
		}

		void CVml_Vector2D_Position::Parse(std::wstring &sValue, bool bFirst)
		{
			EVml_Vector2D_Position eValue = vmlvector2dposConstant;
			double dValue = 0.0;
			std::wstring sId;

			XmlUtils::replace_all( sValue, L" ", L"" );

			int nLen = (int)sValue.length();
			if ( nLen > 0 )
			{

				wchar_t wChar = sValue[0];
				switch ( wChar )
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case '.':

					eValue = vmlvector2dposConstant;
					dValue = XmlUtils::GetDouble(sValue);
					break;

				case 'c':

					if ( L"center" == sValue )
						eValue = vmlvector2dposCenter;

					break;

				case 't':

					if ( L"topleft" == sValue )
						eValue = vmlvector2dposTopLeft;

					break;

				case 'b':

					if ( L"bottomright" == sValue )
						eValue = vmlvector2dposBottomRight;

					break;

				case '@':

					eValue = vmlvector2dposFormula;
					sId    = sValue.substr( 1, nLen - 1 );
					break;

				case '#':

					eValue = vmlvector2dposAdjValue;
					sId    = sValue.substr( 1, nLen - 1 );
					break;
				}

			}

			if ( bFirst )
			{
				m_eTypeX = eValue;
				m_sIdX   = sId;
				m_dX     = dValue;
			}
			else
			{
				m_eTypeY = eValue;
				m_sIdY   = sId;
				m_dY     = dValue;
			}
		}

		//--------------------------------------------------------------------------------
		// Vml_Polygon2D_Units 14.1.2.15 (points)
		//--------------------------------------------------------------------------------

		CVml_Polygon2D_Units::CVml_Polygon2D_Units()
		{
			m_wcDelimiter = 0x20;
		}
		CVml_Polygon2D_Units::CVml_Polygon2D_Units(const CVml_Polygon2D_Units &oOther)
		{
			m_wcDelimiter = 0x20;
			FromString( oOther.ToString() );
		}
		CVml_Polygon2D_Units::CVml_Polygon2D_Units(const std::wstring &sValue)
		{
			m_wcDelimiter = 0x20;
			FromString( sValue );
		}
		CVml_Polygon2D_Units& CVml_Polygon2D_Units::operator =(const std::wstring &sValue)
		{
			FromString( sValue );
			return *this;
		}

		void CVml_Polygon2D_Units::SetDelimiter(wchar_t wcNew)
		{
			m_wcDelimiter = wcNew;
		}
		double CVml_Polygon2D_Units::GetX(int nIndex) const
		{
			if ( nIndex < 0 || nIndex >= (int)m_arrPoints.size()  )
				return 0;

			return m_arrPoints[nIndex].dX;
		}
		double CVml_Polygon2D_Units::GetY(int nIndex) const
		{
			if ( nIndex < 0 || nIndex >= (int)m_arrPoints.size()  )
				return 0;

			return m_arrPoints[nIndex].dY;
		}

		void CVml_Polygon2D_Units::AddPoint(double dX, double dY)
		{
			TPoint oPt( dX, dY );
			m_arrPoints.push_back( oPt );
		}

		double CVml_Polygon2D_Units::FromString(const std::wstring &sValue)
		{
			m_arrPoints.clear();

			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			int nStartPos = 0;
			while ( true )
			{
				int nMidPos = (int)sValue.find( L",", nStartPos );
				int nEndPos = (int)sValue.find( m_wcDelimiter, nMidPos + 1 );

				if ( -1 == nMidPos )
					break;

				if ( -1 == nEndPos )
					nEndPos = nLen;

				std::wstring strX = sValue.substr( nStartPos, nMidPos - nStartPos ) ;
				std::wstring strY = sValue.substr( nMidPos + 1, nEndPos - nMidPos - 1 );

				XmlUtils::replace_all(strX, L"@", L"");
				XmlUtils::replace_all(strY, L"@", L"");

				int nX = (int)XmlUtils::GetDouble(strX);
				int nY = (int)XmlUtils::GetDouble(strY );

				m_arrPoints.push_back( TPoint( nX, nY ) );

				nStartPos = nEndPos + 1;
			}


			return 0;
		}

		std::wstring CVml_Polygon2D_Units::ToString  () const
		{
			std::wstring sResult;

			for ( size_t nIndex = 0; nIndex < m_arrPoints.size(); nIndex++ )
			{
				sResult += boost::lexical_cast<std::wstring>(m_arrPoints[nIndex].dX) + L"," + boost::lexical_cast<std::wstring>(m_arrPoints[nIndex].dY);
				if ( nIndex < m_arrPoints.size() - 1 )
					sResult += m_wcDelimiter;
			}

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// Vml_1_65536_Or_Percentage 14.2.2.11 (brightness)
		//--------------------------------------------------------------------------------
		CVml_1_65536_Or_Percentage::CVml_1_65536_Or_Percentage()
		{
			m_dValue = 0;
		}

		double CVml_1_65536_Or_Percentage::GetValue() const
		{
			return m_dValue;
		}

		void CVml_1_65536_Or_Percentage::SetValue(double dValue)
		{
			m_dValue = (std::max)( 0.0, (std::min)( 1.0, dValue) );
		}

		void CVml_1_65536_Or_Percentage::SetValue(int nValue)
		{
			m_dValue = (std::max)( 0.0, (std::min)( 65536.0, (double) nValue) ) / 65536.0;
		}
		void CVml_1_65536_Or_Percentage::SetPercentage(double dValue)
		{
			m_dValue = (std::max)( 0.0, (std::min)( 100.0, dValue ) ) / 100.0;
		}

		double CVml_1_65536_Or_Percentage::FromString(const std::wstring &sValue)
		{
			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			bool bFraction   = ( 'f' == sValue[ nLen - 1] );
			bool bPercentage = ( '%' == sValue[ nLen - 1] );

			if ( bFraction )
			{
				std::wstring strValue = sValue.substr( 0, nLen - 1 );
				int nValue = strValue.empty() ? 0 : XmlUtils::GetInteger(strValue);

				SetValue( nValue );
			}
			else if ( bPercentage )
			{
				std::wstring strValue = sValue.substr( 0, nLen - 1 );
				double dValue = XmlUtils::GetDouble(strValue);

				SetPercentage( dValue );
			}
			else
			{
				double dValue = XmlUtils::GetDouble(sValue);
				SetValue( dValue );
			}

			return m_dValue;
		}

		std::wstring CVml_1_65536_Or_Percentage::ToString  () const
		{
			std::wstring sResult = boost::lexical_cast<std::wstring>( m_dValue );

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// Vml_Matrix 14.1.2.18 (matrix)
		//--------------------------------------------------------------------------------
		CVml_Matrix::CVml_Matrix()
		{
			ResetMatrix();
		}
		CVml_Matrix::CVml_Matrix(const CVml_Matrix &oOther)
		{
			ResetMatrix();
			FromString( oOther.ToString() );
		}
		CVml_Matrix::CVml_Matrix(const std::wstring &sValue)
		{
			ResetMatrix();
			FromString( sValue );
		}
		CVml_Matrix& CVml_Matrix::operator =(const std::wstring &sValue)
		{
			FromString( sValue );
			return *this;
		}
		void CVml_Matrix::ResetMatrix()
		{
			m_dSxx = 1; m_dSxy = 0;
			m_dSyx = 0; m_dSyy = 1;
			m_dPx  = 0; m_dPy  = 0;
		}
		void CVml_Matrix::SetMatrix(double dSxx, double dSxy, double dSyx, double dSyy, double dPx, double dPy)
		{
			m_dSxx = dSxx; m_dSxy = dSxy;
			m_dSyx = dSyx; m_dSyy = dSyy;
			m_dPx  =  dPx; m_dPy  =  dPy;
		}

		double CVml_Matrix::Get_Sxx() const
		{
			return m_dSxx;
		}
		double CVml_Matrix::Get_Sxy() const
		{
			return m_dSxy;
		}
		double CVml_Matrix::Get_Syx() const
		{
			return m_dSyx;
		}
		double CVml_Matrix::Get_Syy() const
		{
			return m_dSyy;
		}

		double CVml_Matrix::Get_Px() const
		{
			return m_dPx;
		}
		double CVml_Matrix::Get_Py() const
		{
			return m_dPy;
		}

		double CVml_Matrix::FromString(const std::wstring &sValue)
		{
			ResetMatrix();

			int nLen = (int)sValue.length();
			if ( nLen <= 0 )
				return 0;

			// Sxx
			int nStartPos = 0;
			int nEndPos = (int)sValue.find( L",", nStartPos );
			if ( -1 == nEndPos )
				nEndPos = nLen;

			if ( nEndPos - nStartPos > 0 )
			{
				std::wstring strValue = sValue.substr( nStartPos, nEndPos - nStartPos );
				m_dSxx = XmlUtils::GetDouble(strValue);
			}

			// Sxy
			nStartPos = nEndPos + 1;
			nEndPos = (int)sValue.find( L",", nStartPos );
			if ( -1 == nEndPos )
				nEndPos = nLen;

			if ( nEndPos - nStartPos > 0 )
			{
				std::wstring strValue = sValue.substr( nStartPos, nEndPos - nStartPos );
				m_dSxy = XmlUtils::GetDouble(strValue);
			}

			// Syx
			nStartPos = nEndPos + 1;
			nEndPos = (int)sValue.find( L",", nStartPos );
			if ( -1 == nEndPos )
				nEndPos = nLen;

			if ( nEndPos - nStartPos > 0 )
			{
				std::wstring strValue = sValue.substr( nStartPos, nEndPos - nStartPos );
				m_dSyx = XmlUtils::GetDouble(strValue);
			}

			// Syy
			nStartPos = nEndPos + 1;
			nEndPos = (int)sValue.find( L",", nStartPos );
			if ( -1 == nEndPos )
				nEndPos = nLen;

			if ( nEndPos - nStartPos > 0 )
			{
				std::wstring strValue = sValue.substr( nStartPos, nEndPos - nStartPos );
				m_dSyy = XmlUtils::GetDouble(strValue);
			}

			// Px
			nStartPos = nEndPos + 1;
			nEndPos = (int)sValue.find( L",", nStartPos );
			if ( -1 == nEndPos )
				nEndPos = nLen;

			if ( nEndPos - nStartPos > 0 )
			{
				std::wstring strValue = sValue.substr( nStartPos, nEndPos - nStartPos );
				m_dPx = XmlUtils::GetDouble(strValue);
			}

			// Py
			nStartPos = nEndPos + 1;
			nEndPos = (int)sValue.find( L",", nStartPos );
			if ( -1 == nEndPos )
				nEndPos = nLen;

			if ( nEndPos - nStartPos > 0 )
			{
				std::wstring strValue = sValue.substr( nStartPos, nEndPos - nStartPos );
				m_dPy = XmlUtils::GetDouble(strValue);
			}

			nStartPos = nEndPos + 1;
			return 0;
		}

		std::wstring CVml_Matrix::ToString  () const
		{
			return	boost::lexical_cast<std::wstring>(m_dSxx) + L"," + boost::lexical_cast<std::wstring>(m_dSxy) + L"," +
					boost::lexical_cast<std::wstring>(m_dSyx) + L"," + boost::lexical_cast<std::wstring>(m_dSyy) + L"," +
					boost::lexical_cast<std::wstring>(m_dPx)	+ L"," + boost::lexical_cast<std::wstring>(m_dPy);
		}

		//--------------------------------------------------------------------------------
		// Vml_Vector2D_Units_Or_Percentage
		//--------------------------------------------------------------------------------

		CVml_Vector2D_Units_Or_Percentage::CVml_Vector2D_Units_Or_Percentage()
		{
			m_dX = 0;
			m_dY = 0;
			m_bUnitsX = true;
			m_bUnitsY = true;
		}

		double CVml_Vector2D_Units_Or_Percentage::GetX() const
		{
			return m_dX;
		}
		double CVml_Vector2D_Units_Or_Percentage::GetY() const
		{
			return m_dY;
		}
		bool CVml_Vector2D_Units_Or_Percentage::IsXinPoints() const
		{
			return m_bUnitsX;
		}
		bool CVml_Vector2D_Units_Or_Percentage::IsYinPoints() const
		{
			return m_bUnitsY;
		}
		void CVml_Vector2D_Units_Or_Percentage::SetValue_Points(double dX, double dY)
		{
			m_dX = dX;
			m_dY = dY;

			m_bUnitsX = true;
			m_bUnitsY = true;
		}

		double CVml_Vector2D_Units_Or_Percentage::FromString(const std::wstring &sValue)
		{
			m_dX = 0;
			m_dY = 0;
			m_bUnitsX = true;
			m_bUnitsY = true;

			int nLen = (int)sValue.length();
			if ( nLen < 1)
				return 0;

			int nPos = (int)sValue.find( L"," );
			if ( -1 == nPos )
				return 0;

			std::wstring sTemp = sValue.substr( 0, nPos );
			if ( std::wstring::npos != sTemp.find( '%' ) )
			{
				SimpleTypes::CPercentage oPerc = sTemp;
				m_dX = oPerc.GetValue();
				m_bUnitsX = false;
			}
			else
			{
				SimpleTypes::CPoint oPt = sTemp;
				m_dX = oPt.GetValue();
				m_bUnitsX = true;
			}

			sTemp = sValue.substr( nPos + 1, nLen - nPos - 1 );
			if ( std::wstring::npos != sTemp.find( '%' ) )
			{
				SimpleTypes::CPercentage oPerc = sTemp;
				m_dY = oPerc.GetValue();
				m_bUnitsY = false;
			}
			else
			{
				SimpleTypes::CPoint oPt = sTemp;
				m_dY = oPt.GetValue();
				m_bUnitsY = true;
			}

			return 0;
		}

		std::wstring CVml_Vector2D_Units_Or_Percentage::ToString  () const
		{
			std::wstring sResult = boost::lexical_cast<std::wstring>(m_dX);

			if ( m_bUnitsX )	sResult += L"pt,";
			else				sResult += L"%,";

			sResult += boost::lexical_cast<std::wstring>(m_dY);

			if ( m_bUnitsY )	sResult += L"pt";
			else				sResult += L"%";

			return sResult;
		}

		//--------------------------------------------------------------------------------
		// Vml_TextBoxInset 14.1.2.22 (inset)
		//--------------------------------------------------------------------------------

		CVml_TextBoxInset::CVml_TextBoxInset()
		{
			Set( 0, 0, 0, 0 );
		}

		double CVml_TextBoxInset::GetLeft(int nIndex) const
		{
			return m_dLeft;
		}
		double CVml_TextBoxInset::GetTop(int nIndex) const
		{
			return m_dTop;
		}
		double CVml_TextBoxInset::GetRight(int nIndex) const
		{
			return m_dRight;
		}
		double CVml_TextBoxInset::GetBottom(int nIndex) const
		{
			return m_dBottom;
		}

		void CVml_TextBoxInset::Set(double dL, double dT, double dR, double dB)
		{
			m_dLeft   = dL;
			m_dTop    = dT;
			m_dRight  = dR;
			m_dBottom = dB;
		}

		double CVml_TextBoxInset::FromString(const std::wstring &sValueInput)
		{
			Set( 0, 0, 0, 0 );

			if ( sValueInput.empty())
				return 0;

			std::vector<std::wstring> arSplit;

			std::wstring sValue = sValueInput;

			// Разделителями могут быть запятые и пробелы
			XmlUtils::replace_all(sValue, L"@", L"");
			boost::algorithm::split(arSplit, sValue, boost::algorithm::is_any_of(L", "), boost::algorithm::token_compress_on);

			if (arSplit.size() > 0)
			{
				SimpleTypes::CPoint oPt = arSplit[0];
				m_dLeft = oPt.GetValue();
			}

			if (arSplit.size() > 1)
			{
				SimpleTypes::CPoint oPt = arSplit[1];
				m_dTop = oPt.GetValue();
			}

			if (arSplit.size() > 2)
			{
				SimpleTypes::CPoint oPt = arSplit[2];
				m_dRight = oPt.GetValue();
			}

			if (arSplit.size() > 3)
			{
				SimpleTypes::CPoint oPt = arSplit[3];
				m_dBottom = oPt.GetValue();
			}

			return 0;
		}

		std::wstring CVml_TextBoxInset::ToString  () const
		{
			return boost::lexical_cast<std::wstring>(m_dLeft) + L"pt," + boost::lexical_cast<std::wstring>(m_dTop) + L"pt," + boost::lexical_cast<std::wstring>(m_dRight) + L"pt,"+ boost::lexical_cast<std::wstring>(m_dBottom) + L"pt";
		}
	} // Vml
} // SimpleTypes

